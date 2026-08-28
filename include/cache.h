#ifndef CACHE_H
#define CACHE_H

#include "main_mem.h"
#include "replacement.h"
#include <array>

constexpr uint8_t MEMORY_ADDRESS_SIZE = 32;
constexpr uint8_t CACHE_LINE_SIZE = 64;
constexpr uint16_t CACHE_SIZE = 32 * 1024;      // 32kb
                                                
constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_SETS = 64;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_WAYS = 4;

constexpr uint8_t SET_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_INDEX_SIZE = 6;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_OFFEST_SIZE = 6; // 64 byte cache line
                                                         //
constexpr uint8_t DIRECTLY_MAPPED_CACHE_TAG_SIZE = 14;
constexpr uint8_t DIRECTLY_MAPPED_CACHE_INDEX_SIZE = 12; // 4096 lines 
constexpr uint8_t DIRECTLY_MAPPED_CACHE_OFFEST_SIZE = 6; // 64 byte cache line

constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_TAG_SIZE = 20; 
constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_OFFEST_SIZE = 12; // 4096 byte 

struct AddressParts {
  AddressParts(uint32_t address) {
    offset_ = address & ((1U << SET_ASSOCIATIVE_CACHE_OFFEST_SIZE) - 1);
    index_ = (address >> SET_ASSOCIATIVE_CACHE_OFFEST_SIZE) & ((1U << SET_ASSOCIATIVE_CACHE_INDEX_SIZE) - 1);
    tag_ = (address >> (MEMORY_ADDRESS_SIZE - SET_ASSOCIATIVE_CACHE_TAG_SIZE));
  }

  uint32_t tag_;
  uint8_t index_;
  uint8_t offset_;
};

struct CacheLine {
  uint32_t tag_;
  std::array<uint8_t, CACHE_LINE_SIZE> data_;
  bool valid_;
};

class CacheSet {
  public:
    CacheSet();
    CacheLine* find(uint32_t tag);
    CacheLine* replace(uint32_t tag, uint8_t* new_data);

  private:
    std::array<CacheLine, SET_ASSOCIATIVE_CACHE_N_WAYS> set_;
    ReplacementAlgorithm replacement_;
};

class Cache {
  public:
    void initialize(MainMemory* memory);
    uint32_t read(uint32_t address);
    void write(uint32_t address, uint32_t data);

  private:
    std::array<CacheSet, SET_ASSOCIATIVE_CACHE_N_SETS> sets_;
    MainMemory* main_mem_;
};

#endif
