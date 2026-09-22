cmake_minimum_required(VERSION 3.14)
project(cache_simulator)

set(CMAKE_CXX_STANDARD 17)
SET( CMAKE_EXPORT_COMPILE_COMMANDS )

add_executable( demo
  src/demo.cpp
  src/cache.cpp 
  src/main_mem.cpp 
  src/replacement.cpp
)

target_include_directories( demo
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)
