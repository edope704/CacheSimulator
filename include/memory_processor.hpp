#ifndef MEMORY_PROCESSOR_H
#define MEMORY_PROCESSOR_H

#include <iostream>

#include "cache.hpp"
#include "main_mem.hpp"

template <class CacheType>
class MemoryProcessor {
  public:
    MemoryProcessor() { cache_.initialize( &main_memory_ ); }

    uint32_t read( uint32_t address ) {
#ifdef log_on_
      std::printf( "Reading from main memory\nAddress: 0x%x\n\n", address );
#endif
      return cache_.read( address );
    }

    void write( uint32_t address, uint32_t data ) {
#ifdef log_on_
      std::printf( "Writing to main memory \nAddress: 0x%x\nData: 0x%x\n\n", address, data );
#endif
      cache_.write( address, data );
    }

    void print_main_memory() { main_memory_.print_memory(); }

#ifdef hitrate_on_
    float get_total_hit_rate() { return cache_.get_total_hit_rate(); }
    float get_read_hitrate() { return cache_.get_read_hitrate(); }
    float get_write_hitrate() { return cache_.get_write_hitrate(); }
#endif

  private:
    MainMemory main_memory_;
    CacheType cache_;
};

#endif  // MEMORY_PROCESSOR_H
