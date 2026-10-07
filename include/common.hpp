#ifndef COMMON_HPP
#define COMMON_HPP

constexpr uint32_t MAIN_MEM_SIZE = 4 * 1024 * 1024;

constexpr uint8_t MEMORY_ADDRESS_SIZE = 32;
constexpr uint8_t CACHE_LINE_SIZE = 64;
constexpr uint16_t CACHE_SIZE = 32 * 1024;  // 32kb

constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_SETS = 64;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_WAYS = 4;

constexpr uint8_t SET_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_INDEX_SIZE = 6;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_OFFEST_SIZE = 6;  // 64 byte cache line

constexpr uint8_t DIRECTLY_MAPPED_CACHE_TAG_SIZE = 14;
constexpr uint8_t DIRECTLY_MAPPED_CACHE_INDEX_SIZE = 12;  // 4096 lines
constexpr uint8_t DIRECTLY_MAPPED_CACHE_OFFEST_SIZE = 6;  // 64 byte cache line

constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_OFFEST_SIZE = 12;  // 4096 byte

#endif  // COMMON_HPP
