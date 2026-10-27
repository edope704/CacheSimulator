#ifndef COMMON_HPP
#define COMMON_HPP

constexpr uint32_t MAIN_MEM_SIZE = 4 * 1024 * 1024;

constexpr uint8_t MEMORY_ADDRESS_SIZE = 32;
constexpr uint8_t CACHE_LINE_SIZE = 64;
constexpr uint16_t CACHE_SIZE = 32 * 1024;                            // 32KB
constexpr uint16_t N_CACHE_LINES = ( CACHE_SIZE / CACHE_LINE_SIZE );  // 512

constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_SETS = 64;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_N_WAYS = 4;

constexpr uint8_t SET_ASSOCIATIVE_CACHE_TAG_SIZE = 20;
constexpr uint8_t SET_ASSOCIATIVE_CACHE_INDEX_SIZE = 6;   // 64 sets to index
constexpr uint8_t SET_ASSOCIATIVE_CACHE_OFFEST_SIZE = 6;  // 64 bytes per cache line

constexpr uint8_t DIRECTLY_MAPPED_CACHE_TAG_SIZE = 17;
constexpr uint8_t DIRECTLY_MAPPED_CACHE_INDEX_SIZE = 9;   // 512 lines to index
constexpr uint8_t DIRECTLY_MAPPED_CACHE_OFFEST_SIZE = 6;  // 64 byte per cache line

constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_TAG_SIZE = 26;    // Can check up to 2^26 lines
constexpr uint8_t FULLY_ASSOCIATIVE_CACHE_OFFEST_SIZE = 6;  // 64 bytes per cache linme

#endif  // COMMON_HPP
