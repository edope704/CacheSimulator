# Cache Simulator

A C++ based cache and main memory simulator designed to demonstrate fundamental memory hierarchy concepts. The project simulates cache behavior including reading and writing mechanisms, cache hits and misses, and data persistence between cache and main memory.

## Overview

The simulator models a set-associative cache system backed by a simulated 4MB main memory block. It tracks memory addresses and data, simulating memory retrieval operations and providing hexadecimal memory dumps for inspection.

## Building and Running

Ensure you have CMake (minimum version 3.14) and a modern C++ compiler (C++17 supported) installed.

```bash
# Configure the project
cmake -B build -S .

# Build the executable
cmake --build build

# Run the simulator
./build/cache_simulator
```
