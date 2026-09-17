#include "../include/memory_processor.h"
#include "iostream"

MemoryProcessor::MemoryProcessor() {
  cache_.initialize(&main_memory_);
}

uint32_t MemoryProcessor::read(uint32_t address) {
  std::printf( "Reading from main memory\nAddress: 0x%x\n\n", address );
  return cache_.read(address);
}

void MemoryProcessor::write(uint32_t address, uint32_t data) {
  std::printf( "Writing to main memory \nAddress: 0x%x\nData: 0x%x\n\n", address, data );
  cache_.write(address, data);
}

void MemoryProcessor::print_main_memory() {
  main_memory_.print_memory();
}

