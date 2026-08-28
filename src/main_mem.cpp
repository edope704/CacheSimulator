#include "../include/main_mem.h"
#include <array>
#include <cstring>
#include <memory>
#include <iostream>

MainMemory::MainMemory() {
  memory_ = std::make_unique<std::array<uint8_t, MAIN_MEM_SIZE>>();
  memory_.get()->fill(0x00);
}

MainMemory::~MainMemory() { }

void MainMemory::read(uint32_t start_address, uint32_t size, uint8_t* destination) {
  std::memcpy(destination, &memory_.get()->at(start_address), size);
}

void MainMemory::write(uint32_t start_address, uint32_t size, uint8_t* source) {
  std::memcpy(&memory_.get()->at(start_address), source, size);
}

void MainMemory::print_memory() {
  for (uint8_t byte : *memory_.get()) std::cout << byte;
}

