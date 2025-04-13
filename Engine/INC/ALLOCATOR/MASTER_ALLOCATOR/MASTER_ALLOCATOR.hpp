#include <atomic>
#include <bitset>
#include <cstdint>

// usage  to keep collection of  pointer of same size

struct Pointer_List {
  std::uint64_t SIZE;
  void *Pointers[];
};
// thread allocation  memory

struct Thread_Cache_memory {
  std::uint64_t Allocated, free;
  // alocation list i think a cpu an have at most 512 cores and 2 threads per
  // core, i will use 4 threads per core
  std::atomic<std::bitset<2048>> Allocation_list;
  // pointer list
  Pointer_List pointer_List;
};

struct Arena_Memory_List {
  Pointer_List pointer_List;
};

class MASTER_ALLOCATOR {
  Thread_Cache_memory *Thread_Cache;

  std::uint64_t calc_member_size();
  void initializse_member_info();
  void Thread_Cache_allocate();

  void Arena_allocate();
  void Buffer_pool_allocate();
  void general_allocator();

public:
  MASTER_ALLOCATOR();
  ~MASTER_ALLOCATOR();
};
