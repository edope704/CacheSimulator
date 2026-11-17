# Cache Simulator

A C++20 memory hierarchy simulator modeling cache architectures, replacement policies, and write-through persistence to a simulated physical memory block.

## Architectures & Features

- **Cache Types**:
  - **Direct-Mapped Cache (`DirectlyMappedCache`)**: Maps each address to a single predetermined cache line by index.
  - **Set-Associative Cache (`SetAssociativeCache`)**: Configurable N-way set-associative organization (default: 4 ways, 64 sets).
  - **Fully-Associative Cache (`FullyAssociativeCache`)**: Single set spanning all cache lines (default: 512 lines).
- **Replacement Policies**:
  - **Least Recently Used (`LeastRecentlyUsed`)**: State-tracked eviction using an `std::list::splice`-backed doubly linked list for O(1) recency updates.
  - **Random (`RandomReplacement`)**: Uniform pseudo-random line selection.
- **Metrics & Logging**:
  - Optional compile-time hit-rate tracking (`-Dhitrate_on_`) providing total, read, and write hit-rate statistics.
  - Optional compile-time verbose logging (`-Dlog_on_`) for step-by-step cache inspection during reads and writes.

## Default Specifications

Configurations are defined in [`include/common.hpp`](include/common.hpp):

| Parameter | Value |
| :--- | :--- |
| Main Memory Size | 4 MB (byte-addressable, 32-bit addresses) |
| Cache Capacity | 32 KB (512 lines) |
| Cache Line Size | 64 bytes (6-bit block offset) |
| Set-Associative Layout | 64 sets x 4 ways (20-bit tag, 6-bit index, 6-bit offset) |
| Direct-Mapped Layout | 512 lines x 1 way (17-bit tag, 9-bit index, 6-bit offset) |
| Fully-Associative Layout | 1 set x 512 ways (26-bit tag, 0-bit index, 6-bit offset) |
| Write Policy | Write-through to main memory |

## Project Structure

```text
include/
├── common.hpp                 # System constants and bit configurations
├── main_mem.hpp               # Simulated 4 MB physical memory
├── replacement.hpp            # Replacement algorithms (LRU, Random)
├── cache_base.hpp             # Common CacheLine, AddressParts, and Cache base class
├── directly_mapped_cache.hpp  # Direct-mapped cache implementation
├── fully_associative_cache.hpp# Fully-associative cache implementation
├── set_associative_cache.hpp  # Set-associative cache implementation
├── cache.hpp                  # Umbrella header
└── memory_processor.hpp       # Memory controller wrapper
src/
└── demo.cpp                   # Demonstration and verification driver
```

## Build and Run

### Requirements
- CMake 3.14+
- A C++20 compliant compiler (GCC 10+, Clang 12+)

### Standard Build

```bash
cmake -B build -S .
cmake --build build
./build/demo
```

### Build with Options

Flags can be combined via `-DCMAKE_CXX_FLAGS`:

```bash
# Enable verbose logging (step-by-step cache inspection)
cmake -B build -S . -DCMAKE_CXX_FLAGS="-Dlog_on_"
cmake --build build
./build/demo

# Enable hit-rate tracking and verbose logging together
cmake -B build -S . -DCMAKE_CXX_FLAGS="-Dhitrate_on_ -Dlog_on_"
cmake --build build
./build/demo
```

## Example Usage

```cpp
#include "cache.hpp"
#include "memory_processor.hpp"
#include "replacement.hpp"

int main() {
    // Set-associative cache with LRU replacement
    MemoryProcessor<SetAssociativeCache<LeastRecentlyUsed>> sa_cache;
    sa_cache.write(0x1000, 0xDEADBEEF);
    uint32_t value = sa_cache.read(0x1000);

    // Direct-mapped cache
    MemoryProcessor<DirectlyMappedCache> dm_cache;
    dm_cache.write(0x2000, 0x12345678);

    return 0;
}
```
