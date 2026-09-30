#ifndef CACHE_H
#define CACHE_H

#include <array>
#include <cstring>
#include <iostream>
#include <type_traits>

#include "main_mem.hpp"
#include "replacement.hpp"

constexpr uint8_t MEMORY_ADDRESS_SIZE = 32;
constexpr uint8_t CACHE_LINE_SIZE = 64;
constexpr uint16_t CACHE_SIZE = 32 * 1024;  // 32kb

constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_SETS = 64;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_WAYS = 4;

constexpr uint8_t SET_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_INDEX_SIZE = 6;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_OFFEST_SIZE = 6;  // 64 byte cache line

constexpr uint8_t DIRECTLY_MAPPED_CACHE_TAG_SIZE = 14;
constexpr uint8_t DIRECTLY_MAPPED_CACHE_INDEX_SIZE = 12;  // 4096 lines
constexpr uint8_t DIRECTLY_MAPPED_CACHE_OFFEST_SIZE = 6;  // 64 byte cache line

constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_OFFEST_SIZE = 12;  // 4096 byte

// template <class CacheType>
template <uint8_t TagSize, uint8_t IndexSize, uint8_t OffsetSize>
struct AddressParts {
    AddressParts( uint32_t address ) {
      offset_ = address & ( ( 1U << OffsetSize ) - 1 );
      index_ = ( address >> OffsetSize ) & ( ( 1U << IndexSize ) - 1 );
      tag_ = ( address >> ( OffsetSize + IndexSize ) );
    }

    uint32_t tag_;
    uint8_t index_;
    uint8_t offset_;
};

template <uint8_t TagSize, uint8_t OffsetSize>
struct AddressParts<TagSize, 0, OffsetSize> {
    AddressParts( uint32_t address ) {
      offset_ = address & ( ( 1U << OffsetSize ) - 1 );
      tag_ = ( address >> OffsetSize );
    }

    uint32_t tag_;
    uint8_t offset_;
};

struct CacheLine {
    uint32_t tag_;
    std::array<uint8_t, CACHE_LINE_SIZE> data_;
    bool valid_;
};

template <class ReplacementPolicy>
class CacheSet {
  public:
    CacheSet();
    CacheLine* find( uint32_t tag );
    CacheLine* replace( uint32_t tag, uint8_t* new_data );

  private:
    std::array<CacheLine, SET_ASSOCIATIVE_CACHE_N_WAYS> set_;
    ReplacementPolicy replacement_;
};

template <class ReplacementPolicy>
class Cache {
  public:
    virtual ~Cache() = default;

    virtual void initialize( MainMemory* memory ) = 0;
    virtual uint32_t read( uint32_t address ) = 0;
    virtual void write( uint32_t address, uint32_t data ) = 0;
};

template <class ReplacementPolicy>
class SetAssociativeCache : public Cache<ReplacementPolicy> {
  public:
    void initialize( MainMemory* memory );
    uint32_t read( uint32_t address );
    void write( uint32_t address, uint32_t data );

  private:
    using ParsedAddress =
        AddressParts<SET_ASSOCIATIVE_CACHE_TAG_SIZE, SET_ASSOCIATIVE_CACHE_INDEX_SIZE,
                     SET_ASSOCIATIVE_CACHE_OFFEST_SIZE>;

    std::array<CacheSet<ReplacementPolicy>, SET_ASSOCIATIVE_CACHE_N_SETS> sets_;
    MainMemory* main_mem_;
};

template <class ReplacementPolicy>
class FullyAssociativeCache : public Cache<ReplacementPolicy> {
  public:
    void initialize( MainMemory* memory );
    uint32_t read( uint32_t address );
    void write( uint32_t address, uint32_t data );

  private:
    MainMemory* main_mem_;
};

template <class ReplacementPolicy>
class DirectlyMappedCache : public Cache<ReplacementPolicy> {
  public:
    void initialize( MainMemory* memory );
    uint32_t read( uint32_t address );
    void write( uint32_t address, uint32_t data );

  private:
    using ParsedAddress =
        AddressParts<DIRECTLY_MAPPED_CACHE_TAG_SIZE, DIRECTLY_MAPPED_CACHE_INDEX_SIZE,
                     DIRECTLY_MAPPED_CACHE_OFFEST_SIZE>;
    MainMemory* main_mem_;
};

// ----------------------------------------------------------------------------
// Template Implementations
// ----------------------------------------------------------------------------

template <class ReplacementPolicy>
CacheSet<ReplacementPolicy>::CacheSet() {
  if constexpr ( requires { replacement_.SetWays( SET_ASSOCIATIVE_CACHE_N_SETS ); } ) {
    replacement_.SetWays( SET_ASSOCIATIVE_CACHE_N_WAYS );
  }
}

template <class ReplacementPolicy>
CacheLine* CacheSet<ReplacementPolicy>::find( uint32_t tag ) {
  for ( uint8_t way{ 0 }; way < SET_ASSOCIATIVE_CACHE_N_WAYS; way++ )
    if ( set_.at( way ).valid_ && set_.at( way ).tag_ == tag ) return &set_.at( way );
  return nullptr;
}

template <class ReplacementPolicy>
CacheLine* CacheSet<ReplacementPolicy>::replace( uint32_t tag, uint8_t* new_data ) {
  uint8_t victim_index = replacement_.select_victim();

  set_.at( victim_index ).valid_ = true;
  set_.at( victim_index ).tag_ = tag;

  std::memcpy( set_.at( victim_index ).data_.data(), new_data, CACHE_LINE_SIZE );

  return &set_.at( victim_index );
}

template <class ReplacementPolicy>
void SetAssociativeCache<ReplacementPolicy>::initialize( MainMemory* memory ) {
  main_mem_ = memory;
}

template <class ReplacementPolicy>
uint32_t SetAssociativeCache<ReplacementPolicy>::read( uint32_t address ) {
  ParsedAddress address_parts{ address };

  CacheSet<ReplacementPolicy>& target_set = sets_.at( address_parts.index_ );
  CacheLine* target_line = target_set.find( address_parts.tag_ );

  uint32_t data;

  if ( target_line ) {  // cache hit
    std::printf( "Cache hit. Retrieving data from Cache...\n" );

    data = *reinterpret_cast<uint32_t*>( &target_line->data_[ address_parts.offset_ ] );
  } else {  // cache miss
    std::printf( "Cache miss, retrieving data from main memory...\n" );

    uint32_t target_line_start = address & ~( CACHE_LINE_SIZE - 1 );
    std::array<uint8_t, CACHE_LINE_SIZE> temp_buffer;
    main_mem_->read( target_line_start, CACHE_LINE_SIZE, temp_buffer.data() );
    CacheLine* new_line = target_set.replace( address_parts.tag_, temp_buffer.data() );

    data = *reinterpret_cast<uint32_t*>( &new_line->data_[ address_parts.offset_ ] );
  }

  std::printf( "Cache reading result:\nAddress: 0x%x\nData: 0x%x\n\n", address, data );
  return data;
}

template <class ReplacementPolicy>
void SetAssociativeCache<ReplacementPolicy>::write( uint32_t address, uint32_t data ) {
  ParsedAddress address_parts{ address };

  CacheSet<ReplacementPolicy>& target_set = sets_.at( address_parts.index_ );
  CacheLine* target_line = target_set.find( address_parts.tag_ );

  if ( target_line ) {  // cache hit
    *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) ) = data;
  }

  main_mem_->write( address, sizeof( uint32_t ), reinterpret_cast<uint8_t*>( &data ) );
}

#endif
