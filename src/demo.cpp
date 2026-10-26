#include <iostream>
#include <string>

#include "cache.hpp"
#include "main_mem.hpp"
#include "memory_processor.hpp"
#include "replacement.hpp"

void print_separator( const std::string& title ) {
  std::cout << "============================================================\n";
  std::cout << "  " << title << "\n";
  std::cout << "============================================================\n";
}

void test_set_associative_cache() {
  print_separator( "1. Testing Set-Associative Cache (4-way, Random Replacement)" );

  MemoryProcessor<SetAssociativeCache<RandomReplacement>> memory;

  std::cout << ">>> Step 1: Initial Read at address 0x100 (Expect: Cache Miss)\n";
  memory.read( 0x100 );

  std::cout << ">>> Step 2: Write 0xDEADBEEF to address 0x100\n";
  memory.write( 0x100, 0xDEADBEEF );

  std::cout << ">>> Step 3: Subsequent Read at address 0x100 (Expect: Cache Hit)\n";
  memory.read( 0x100 );
}

void test_fully_associative_cache() {
  print_separator( "2. Testing Fully-Associative Cache (Random Replacement)" );

  MemoryProcessor<FullyAssociativeCache<RandomReplacement>> memory;

  std::cout << ">>> Step 1: Initial Read at address 0x200 (Expect: Cache Miss)\n";
  memory.read( 0x200 );

  std::cout << ">>> Step 2: Write 0xCAFEBABE to address 0x200\n";
  memory.write( 0x200, 0xCAFEBABE );

  std::cout << ">>> Step 3: Subsequent Read at address 0x200 (Expect: Cache Hit)\n";
  memory.read( 0x200 );
}

void test_directly_mapped_cache() {
  print_separator( "3. Testing Directly-Mapped Cache" );

  MemoryProcessor<DirectlyMappedCache> memory;

  std::cout << ">>> Step 1: Initial Read at address 0x300 (Expect: Cache Miss)\n";
  memory.read( 0x300 );

  std::cout << ">>> Step 2: Write 0x12345678 to address 0x300\n";
  memory.write( 0x300, 0x12345678 );

  std::cout << ">>> Step 3: Subsequent Read at address 0x300 (Expect: Cache Hit)\n";
  memory.read( 0x300 );
}

void test_lru_replacement() {
  print_separator( "4. Testing Set-Associative Cache with Least Recently Used (LRU)" );

  MemoryProcessor<SetAssociativeCache<LeastRecentlyUsed>> memory;

  std::cout << ">>> Step 1: Fill all 4 ways of Set 0 (Addresses: 0x0000, 0x1000, 0x2000, 0x3000)\n";
  memory.read( 0x0000 );
  memory.read( 0x1000 );
  memory.read( 0x2000 );
  memory.read( 0x3000 );

  std::cout << ">>> Step 2: Access 0x0000 again (Cache Hit -> Marks 0x0000 as MRU, 0x1000 becomes LRU)\n";
  memory.read( 0x0000 );

  std::cout << ">>> Step 3: Access 5th line 0x4000 mapping to Set 0 (Cache Miss -> Evicts LRU: 0x1000)\n";
  memory.read( 0x4000 );

  std::cout << ">>> Step 4: Read 0x0000 again (Expect: Cache Hit, since it was MRU and preserved)\n";
  memory.read( 0x0000 );

  std::cout << ">>> Step 5: Read 0x1000 again (Expect: Cache Miss, since it was LRU and evicted)\n";
  memory.read( 0x1000 );
}

int main() {
  test_set_associative_cache();
  test_fully_associative_cache();
  test_directly_mapped_cache();
  test_lru_replacement();

  return 0;
}
