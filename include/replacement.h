#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <cstdint>

class ReplacementAlgorithm {
  public:
    void SetWays( uint8_t num_of_ways );
    uint8_t select_victim();

  private:
    uint8_t num_of_ways_;
};

#endif  // REPLACEMENT_H
