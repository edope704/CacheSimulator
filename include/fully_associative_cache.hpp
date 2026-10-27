#ifndef FULLY_ASSOCIATIVE_CACHE_HPP
#define FULLY_ASSOCIATIVE_CACHE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "cache_base.hpp"
#include "common.hpp"
#include "replacement.hpp"

template <class ReplacementPolicy>
class FullyAssociativeCache : public Cache {
  public:
    FullyAssociativeCache();
    uint32_t read( uint32_t address ) override;
    void write( uint32_t address, uint32_t data ) override;

  private:
    using ParsedAddress =
        AddressParts<FULLY_ASSOCIATIVE_CACHE_TAG_SIZE, 0, FULLY_ASSOCIATIVE_CACHE_OFFEST_SIZE>;

    CacheLine* find( uint32_t tag );
    CacheLine* replace( uint32_t tag, uint8_t* new_data );

    ReplacementPolicy replacement_;
    std::array<CacheLine, N_CACHE_LINES> cache_;
};

// ----------------------------------------------------------------------------
// Template Implementations
// ----------------------------------------------------------------------------

template <class ReplacementPolicy>
FullyAssociativeCache<ReplacementPolicy>::FullyAssociativeCache() {
  if constexpr ( requires { replacement_.SetWays( N_CACHE_LINES ); } ) {
    replacement_.SetWays( N_CACHE_LINES );
  }
}

template <class ReplacementPolicy>
CacheLine* FullyAssociativeCache<ReplacementPolicy>::find( uint32_t tag ) {
  for ( size_t i = 0; i < cache_.size(); ++i ) {
    if ( cache_[ i ].valid_ && cache_[ i ].tag_ == tag ) {
      if constexpr ( requires { replacement_.record_access( i ); } ) {
        replacement_.record_access( static_cast<uint16_t>( i ) );
      }
      return &cache_[ i ];
    }
  }
  return nullptr;
}

template <class ReplacementPolicy>
CacheLine* FullyAssociativeCache<ReplacementPolicy>::replace( uint32_t tag, uint8_t* new_data ) {
  uint16_t victim_index = 0;
  bool found_empty = false;
  for ( size_t i = 0; i < cache_.size(); ++i ) {
    if ( !cache_[ i ].valid_ ) {
      victim_index = static_cast<uint16_t>( i );
      found_empty = true;
      break;
    }
  }
  if ( !found_empty ) {
    victim_index = replacement_.select_victim();
  }

  cache_.at( victim_index ).valid_ = true;
  cache_.at( victim_index ).tag_ = tag;

  std::memcpy( cache_.at( victim_index ).data_.data(), new_data, CACHE_LINE_SIZE );

  if constexpr ( requires { replacement_.record_access( victim_index ); } ) {
    replacement_.record_access( victim_index );
  }

  return &cache_.at( victim_index );
}

template <class ReplacementPolicy>
uint32_t FullyAssociativeCache<ReplacementPolicy>::read( uint32_t address ) {
  ParsedAddress address_parts{ address };

  CacheLine* target_line = find( address_parts.tag_ );
  uint32_t data;

  if ( target_line ) {  // Cache hit
    std::printf( "Cache hit, retrieving data from cache...\n" );
    data = *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) );
  } else {  // Cache miss
    std::printf( "Cache miss. Retrieving data from main memory...\n" );
    uint32_t target_line_start = address & ~( CACHE_LINE_SIZE - 1 );
    std::array<uint8_t, CACHE_LINE_SIZE> temp_buffer;
    main_mem_->read( target_line_start, CACHE_LINE_SIZE, temp_buffer.data() );
    CacheLine* new_line = replace( address_parts.tag_, temp_buffer.data() );
    data = *reinterpret_cast<uint32_t*>( &new_line->data_.at( address_parts.offset_ ) );
  }

  std::printf( "Cache reading result:\nAddress: 0x%x\nData: 0x%x\n\n", address, data );
  return data;
}

template <class ReplacementPolicy>
void FullyAssociativeCache<ReplacementPolicy>::write( uint32_t address, uint32_t data ) {
  ParsedAddress address_parts{ address };

  CacheLine* target_line = find( address_parts.tag_ );

  if ( target_line ) {  // cache hit
    *reinterpret_cast<uint32_t*>( &target_line->data_.at( address_parts.offset_ ) ) = data;
  }

  main_mem_->write( address, sizeof( uint32_t ), reinterpret_cast<uint8_t*>( &data ) );
}

#endif  // FULLY_ASSOCIATIVE_CACHE_HPP

