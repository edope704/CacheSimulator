/*
 * main_mem.h
 *
 * Simulate main memory
 */

#ifndef MAIN_MEM_H
#define MAIN_MEM_H

#include <array>
#include <cstdint>
#include <iomanip>
#include <memory>

#include "common.hpp"

class MainMemory {
  public:
    MainMemory() {
      memory_ = std::make_unique<std::array<uint8_t, MAIN_MEM_SIZE>>();
      memory_.get()->fill( 0x00 );
    }
    ~MainMemory();

    // reads from memory at address start_address and writes to destination
    void read( uint32_t start_address, uint32_t size, uint8_t* destination ) {
      std::memcpy( destination, &memory_.get()->at( start_address ), size );
    }

    // reads from source and writes to memory at address start_address
    void write( uint32_t start_address, uint32_t size, uint8_t* source ) {
      std::memcpy( &memory_.get()->at( start_address ), source, size );
    }

    // prints the first 256 bytes of memory
    void print_memory() {
      const uint32_t bytes_per_line = 16;

      for ( uint32_t i = 0; i < 128; ++i ) {
        if ( i % bytes_per_line == 0 ) {
          if ( i > 0 ) std::cout << "\n";
          std::cout << "0x" << std::setw( 8 ) << std::setfill( '0' ) << std::hex << i << ": ";
        }
        std::cout << std::setw( 2 ) << std::setfill( '0' ) << std::hex
                  << static_cast<int>( ( *memory_ )[ i ] ) << " ";
      }

      std::cout << "\n" << std::dec << std::setfill( ' ' );  // Reset formatting
    }

  private:
    std::unique_ptr<std::array<uint8_t, MAIN_MEM_SIZE>> memory_;
};

#endif
