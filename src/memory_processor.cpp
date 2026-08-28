#include "../include/memory_processor.h"

MemoryProcessor::MemoryProcessor() {
  cache_.initialize(&main_memory_);
}

uint32_t MemoryProcessor::read(uint32_t address) {
  return cache_.read(address);
}

void MemoryProcessor::write(uint32_t address, uint32_t data) {
  cache_.write(address, data);
}

void MemoryProcessor::print_main_memory() {
  main_memory_.print_memory();
}

