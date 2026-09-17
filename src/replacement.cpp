#include "replacement.h"
#include <cstdlib>

void ReplacementAlgorithm::SetWays(uint8_t num_of_ways) {
    num_of_ways_ = num_of_ways;
}

uint8_t ReplacementAlgorithm::select_victim() {
    // Simple random replacement policy for now
    if (num_of_ways_ == 0) return 0;
    return rand() % num_of_ways_;
}

