#ifndef SET_ASSOCIATIVE_CACHE_HPP
#define SET_ASSOCIATIVE_CACHE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "cache_base.hpp"
#include "common.hpp"
#include "replacement.hpp"

template <class ReplacementPolicy>
class SetAssociativeCache : public Cache {
  public:
    uint32_t read( uint32_t address ) override;
    void write( uint32_t address, uint32_t data ) override;

  private:
    using ParsedAddress =
        AddressParts<SET_ASSOCIATIVE_CACHE_TAG_SIZE, SET_ASSOCIATIVE_CACHE_INDEX_SIZE,
                     SET_ASSOCIATIVE_CACHE_OFFEST_SIZE>;

    class CacheSet {
      public:
        CacheSet();
        CacheLine* find( uint32_t tag );
        CacheLine* replace( uint32_t tag, uint8_t* new_data );

      private:
        std::array<CacheLine, SET_ASSOCIATIVE_CACHE_N_WAYS> set_;
        ReplacementPolicy replacement_;
    };

    std::array<CacheSet, SET_ASSOCIATIVE_CACHE_N_SETS> sets_;
};

// ----------------------------------------------------------------------------
// Template Implementations
// ----------------------------------------------------------------------------

template <class ReplacementPolicy>
SetAssociativeCache<ReplacementPolicy>::CacheSet::CacheSet() {
  if constexpr ( requires { replacement_.SetWays( SET_ASSOCIATIVE_CACHE_N_SETS ); } ) {
    replacement_.SetWays( SET_ASSOCIATIVE_CACHE_N_WAYS );
  }
}

template <class ReplacementPolicy>
CacheLine* SetAssociativeCache<ReplacementPolicy>::CacheSet::find( uint32_t tag ) {
  for ( uint8_t way{ 0 }; way < SET_ASSOCIATIVE_CACHE_N_WAYS; way++ ) {
    if ( set_.at( way ).valid_ && set_.at( way ).tag_ == tag ) {
      if constexpr ( requires { replacement_.record_access( way ); } ) {
        replacement_.record_access( way );
      }
      return &set_.at( way );
    }
  }
  return nullptr;
}

template <class ReplacementPolicy>
CacheLine* SetAssociativeCache<ReplacementPolicy>::CacheSet::replace( uint32_t tag,
                                                                      uint8_t* new_data ) {
  uint16_t victim_index = 0;
  bool found_empty = false;
  for ( uint8_t way{ 0 }; way < SET_ASSOCIATIVE_CACHE_N_WAYS; way++ ) {
    if ( !set_.at( way ).valid_ ) {
      victim_index = way;
      found_empty = true;
      break;
    }
  }
  if ( !found_empty ) {
    victim_index = replacement_.select_victim();
  }

  set_.at( victim_index ).valid_ = true;
  set_.at( victim_index ).tag_ = tag;

  std::memcpy( set_.at( victim_index ).data_.data(), new_data, CACHE_LINE_SIZE );

  if constexpr ( requires { replacement_.record_access( victim_index ); } ) {
    replacement_.record_access( victim_index );
  }

  return &set_.at( victim_index );
}

template <class ReplacementPolicy>
uint32_t SetAssociativeCache<ReplacementPolicy>::read( uint32_t address ) {
  ParsedAddress address_parts{ address };

  CacheSet& target_set = sets_.at( address_parts.index_ );
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

  CacheSet& target_set = sets_.at( address_parts.index_ );
  CacheLine* target_line = target_set.find( address_parts.tag_ );

  if ( target_line ) {  // cache hit
    *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) ) = data;
  }

  main_mem_->write( address, sizeof( uint32_t ), reinterpret_cast<uint8_t*>( &data ) );
}

#endif  // SET_ASSOCIATIVE_CACHE_HPP

