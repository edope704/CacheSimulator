/*
 * main_mem.h
 *
 * Simulate main memory
 */

#ifndef MAIN_MEM_H
#define MAIN_MEM_H

#include <stdint.h>

#include <array>
#include <memory>

constexpr uint32_t MAIN_MEM_SIZE = 4 * 1024 * 1024;

class MainMemory {
  public:
    MainMemory();
    ~MainMemory();

    // reads from memory at address start_address and writes to destination
    void read( uint32_t start_address, uint32_t size, uint8_t* destination );
    // reads from source and writes to memory at address start_address
    void write( uint32_t start_address, uint32_t size, uint8_t* source );
    // print_memory prints the first 256 bytes of memory
    void print_memory();

  private:
    std::unique_ptr< std::array< uint8_t, MAIN_MEM_SIZE > > memory_;
};

#endif
