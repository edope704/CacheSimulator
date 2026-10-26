#ifndef DIRECTLY_MAPPED_CACHE_HPP
#define DIRECTLY_MAPPED_CACHE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "cache_base.hpp"
#include "common.hpp"

class DirectlyMappedCache : public Cache {
  public:
    uint32_t read( uint32_t address ) override;
    void write( uint32_t address, uint32_t data ) override;

  private:
    using ParsedAddress =
        AddressParts<DIRECTLY_MAPPED_CACHE_TAG_SIZE, DIRECTLY_MAPPED_CACHE_INDEX_SIZE,
                     DIRECTLY_MAPPED_CACHE_OFFEST_SIZE>;

    CacheLine* find( uint32_t tag, uint16_t index );
    CacheLine* replace( uint32_t tag, uint16_t index, uint8_t* new_data );

    std::array<CacheLine, N_CACHE_LINES> cache_;
};

inline CacheLine* DirectlyMappedCache::find( uint32_t tag, uint16_t index ) {
  if ( cache_.at( index ).valid_ && cache_.at( index ).tag_ == tag )
    return &cache_.at( index );
  return nullptr;
}

inline CacheLine* DirectlyMappedCache::replace( uint32_t tag, uint16_t index, uint8_t* new_data ) {
  cache_.at( index ).valid_ = true;
  cache_.at( index ).tag_ = tag;

  std::memcpy( cache_.at( index ).data_.data(), new_data, CACHE_LINE_SIZE );

  return &cache_.at( index );
}

inline uint32_t DirectlyMappedCache::read( uint32_t address ) {
  ParsedAddress address_parts{ address };

  CacheLine* target_line = find( address_parts.tag_, address_parts.index_ );
  uint32_t data;

  if ( target_line ) {  // Cache hit
    std::printf( "Cache hit. Retrieving data from Cache...\n" );
    data = *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) );
  } else {  // Cache miss
    std::printf( "Cache miss, retrieving data from main memory...\n" );
    uint32_t target_line_start = address & ~( CACHE_LINE_SIZE - 1 );
    std::array<uint8_t, CACHE_LINE_SIZE> temp_buffer;
    main_mem_->read( target_line_start, CACHE_LINE_SIZE, temp_buffer.data() );
    CacheLine* new_line = replace( address_parts.tag_, address_parts.index_, temp_buffer.data() );
    data = *reinterpret_cast<uint32_t*>( &new_line->data_.at( address_parts.offset_ ) );
  }

  std::printf( "Cache reading result:\nAddress: 0x%x\nData: 0x%x\n\n", address, data );
  return data;
}

inline void DirectlyMappedCache::write( uint32_t address, uint32_t data ) {
  ParsedAddress address_parts{ address };

  CacheLine* target_line = find( address_parts.tag_, address_parts.index_ );

  if ( target_line ) {  // Cache hit
    *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) ) = data;
  }

  main_mem_->write( address, sizeof( uint32_t ), reinterpret_cast<uint8_t*>( &data ) );
}

#endif  // DIRECTLY_MAPPED_CACHE_HPP

