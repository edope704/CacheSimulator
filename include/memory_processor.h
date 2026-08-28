#ifndef MEMORY_PROCESSOR_H
#define MEMORY_PROCESSOR_H

#include "main_mem.h"
#include "cache.h"

class MemoryProcessor {
  public:
    MemoryProcessor();
    uint32_t read(uint32_t address);
    void write(uint32_t address, uint32_t data);
    void print_main_memory();

  private:
    MainMemory main_memory_;
    Cache cache_;
};


#endif // MEMORY_PROCESSOR_H

