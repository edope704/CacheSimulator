#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <cstdint>
#include <cstdlib>
#include <type_traits>

#include "common.hpp"

class ReplacementAlgorithm {
  public:
    virtual uint8_t select_victim() = 0;
};

class RandomReplacement : public ReplacementAlgorithm {
  public:
    void SetWays( uint8_t num_of_ways ) { num_of_ways_ = num_of_ways; }

    uint8_t select_victim() {
      if ( num_of_ways_ == 0 ) return 0;
      return std::rand() % num_of_ways_;
    }

  private:
    uint8_t num_of_ways_ = CACHE_SIZE / CACHE_LINE_SIZE;
};

#endif  // REPLACEMENT_H
