#include "cache.h"
#include "main_mem.h"
#include "memory_processor.h"

int main() {
  MemoryProcessor memory;
  uint32_t demo_address{ 0x20 };

  memory.read( 0x28 );

  uint8_t data{ 0x1 };
  for ( int i{ 0 }; i < 16; i++ ) {
    memory.write( demo_address++, data++ );
  }

  memory.read( 0X28 );
  memory.read( 0X28 );

  // print the first 128 bytes of memory
  memory.print_main_memory();

  return 0;
}