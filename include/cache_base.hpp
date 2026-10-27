#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <array>
#include <cstdint>

#include "common.hpp"
#include "main_mem.hpp"

template <uint8_t TagSize, uint8_t IndexSize, uint8_t OffsetSize>
struct AddressParts {
    AddressParts( uint32_t address ) {
      offset_ = address & ( ( 1U << OffsetSize ) - 1 );
      index_ = ( address >> OffsetSize ) & ( ( 1U << IndexSize ) - 1 );
      tag_ = ( address >> ( OffsetSize + IndexSize ) );
    }

    uint32_t tag_;
    uint16_t index_;
    uint8_t offset_;
};

struct CacheLine {
    uint32_t tag_;
    std::array<uint8_t, CACHE_LINE_SIZE> data_;
    bool valid_;
};

class Cache {
  public:
    virtual ~Cache() = default;

    void initialize( MainMemory* memory ) { main_mem_ = memory; }
    virtual uint32_t read( uint32_t address ) = 0;
    virtual void write( uint32_t address, uint32_t data ) = 0;

  protected:
    MainMemory* main_mem_;
};

#endif  // CACHE_BASE_HPP

