#include "../include/cache.h"
#include <array>
#include <cstring>

CacheSet::CacheSet() {
  replacement_.SetWays(SET_ASSOCIATIVE_CACHE_N_WAYS);
}

CacheLine* CacheSet::find(uint32_t tag) {
  for (uint8_t way{ 0 }; way<SET_ASSOCIATIVE_CACHE_N_WAYS; way++)
    if (set_.at(way).valid_ &&  set_.at(way).tag_ == tag) return &set_.at(way);
  return nullptr;
}

CacheLine* CacheSet::replace(uint32_t tag, uint8_t* new_data) {
  uint8_t victim_index = replacement_.select_victim();
 
  set_.at(victim_index).valid_ = true;
  set_.at(victim_index).tag_ = tag;

  std::memcpy(set_.at(victim_index).data_.data(), new_data, CACHE_LINE_SIZE);

  return nullptr;
}

void Cache::initialize(MainMemory* memory) {
  main_mem_ = memory;
}

uint32_t Cache::read(uint32_t address) {
  AddressParts address_parts{ address };
  
  CacheSet target_set = sets_.at(address_parts.index_);
  CacheLine* target_line = target_set.find(address_parts.tag_);
  
  if (target_line) { // cache hit
    return *reinterpret_cast<uint32_t*>(target_line->data_[address_parts.offset_]);
  } else { // cache miss
    uint32_t target_line_start = address & ~(CACHE_LINE_SIZE - 1);
    std::array<uint8_t, CACHE_LINE_SIZE> temp_buffer;
    main_mem_->read(target_line_start, CACHE_LINE_SIZE, temp_buffer.data());
    CacheLine* new_line = target_set.replace(address_parts.tag_, temp_buffer.data());
    return *reinterpret_cast<uint32_t*>(&new_line->data_[address_parts.offset_]);
  }
  
  return 0;
}

void Cache::write(uint32_t address, uint32_t data) {
  AddressParts address_parts{ address };
  
  CacheSet target_set = sets_.at(address_parts.index_);
  CacheLine* target_line = target_set.find(address_parts.tag_);

  if (target_line) { // cache hit 
    *reinterpret_cast<uint32_t*>(&target_line->data_.at(address_parts.offset_)) = data; 
  }

  main_mem_->write(address, sizeof(uint32_t), reinterpret_cast<uint8_t*>(&data));
}

