#ifndef MEMORY_PROCESSOR_H
#define MEMORY_PROCESSOR_H

#include <iostream>

#include "cache.h"
#include "main_mem.h"

template <class CacheType>
class MemoryProcessor {
  public:
    MemoryProcessor() { cache_.initialize( &main_memory_ ); }

    uint32_t read( uint32_t address ) {
      std::printf( "Reading from main memory\nAddress: 0x%x\n\n", address );
      return cache_.read( address );
    }

    void write( uint32_t address, uint32_t data ) {
      std::printf( "Writing to main memory \nAddress: 0x%x\nData: 0x%x\n\n", address, data );
      cache_.write( address, data );
    }

    void print_main_memory() { main_memory_.print_memory(); }

  private:
    MainMemory main_memory_;
    CacheType cache_;
};

#endif  // MEMORY_PROCESSOR_H
