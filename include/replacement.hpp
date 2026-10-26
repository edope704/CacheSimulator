#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

#include "common.hpp"

class ReplacementAlgorithm {
  public:
    virtual ~ReplacementAlgorithm() = default;
    virtual void SetWays( uint16_t num_of_ways ) = 0;
    virtual void record_access( uint16_t index ) = 0;
    virtual uint16_t select_victim() = 0;
};

class RandomReplacement : public ReplacementAlgorithm {
  public:
    void SetWays( uint16_t num_of_ways ) override { num_of_ways_ = num_of_ways; }
    void record_access( uint16_t index ) override { (void)index; }

    uint16_t select_victim() override {
      if ( num_of_ways_ == 0 ) return 0;
      return std::rand() % num_of_ways_;
    }

  private:
    uint16_t num_of_ways_ = N_CACHE_LINES;
};

class LeastRecentlyUsed : public ReplacementAlgorithm {
  public:
    LeastRecentlyUsed() { SetWays( N_CACHE_LINES ); }

    void SetWays( uint16_t num_of_ways ) override {
      num_of_ways_ = num_of_ways;
      lru_list_.clear();
      node_map_.resize( num_of_ways );

      for ( uint16_t i = 0; i < num_of_ways; ++i ) {
        lru_list_.push_back( i );
        node_map_[ i ] = std::prev( lru_list_.end() );
      }
    }

    void record_access( uint16_t index ) override {
      if ( index < num_of_ways_ ) {
        lru_list_.splice( lru_list_.begin(), lru_list_, node_map_[ index ] );
      }
    }

    uint16_t select_victim() override {
      if ( lru_list_.empty() ) return 0;
      return lru_list_.back();
    }

  private:
    uint16_t num_of_ways_ = 0;
    std::list<uint16_t> lru_list_;
    std::vector<std::list<uint16_t>::iterator> node_map_;
};

#endif  // REPLACEMENT_H
