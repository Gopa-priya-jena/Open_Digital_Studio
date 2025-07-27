#pragma once
#include <Types.hpp>
#include <thread>
//////////////////////////////////////
// these are some necessary data structure

enum TLB_size {
  page_1GB,
  page_2mb,
  page_sys,

};

struct memory_orders {
  U64 Quantity;
  U16 Order_count;
  TLB_size page;
};

struct memory_allocations {
  U64 Quantity;
  U16 Success;
  U16 Failure;
  void* pointer_list[];
};

struct Pointer_List {
  U64 SIZE;
  U16 Count;
  void* Pointers[];
};

struct Allocation_table {
  Allocation_table* next = nullptr;
  Pointer_List Alloaction_list;
};
struct Thread_allocation {

  std::thread::id Thread_id{ 0 };
  Allocation_table* Alloc_table = nullptr;
};

struct Thread_allocation_Tree {
  U32 size;
  U32 Table_count;
  Thread_allocation_Tree* next;
  Thread_allocation thread_allocation_table[];
};

// override

// // C
// void*  malloc(size_t size);
// void*  calloc(size_t size, size_t n);
// void*  realloc(void* p, size_t newsize);
// void   free(void* p);
//
// void*  aligned_alloc(size_t alignment, size_t size);
// char*  strdup(const char* s);
// char*  strndup(const char* s, size_t n);
// char*  realpath(const char* fname, char* resolved_name);
//
//
// // C++
// void   operator delete(void* p);
// void   operator delete[](void* p);
//
// void*  operator new(std::size_t n) noexcept(false);
// void*  operator new[](std::size_t n) noexcept(false);
// void*  operator new( std::size_t n, std::align_val_t align) noexcept(false);
// void*  operator new[]( std::size_t n, std::align_val_t align) noexcept(false);
//
// void*  operator new  ( std::size_t count, const std::nothrow_t& tag);
// void*  operator new[]( std::size_t count, const std::nothrow_t& tag);
// void*  operator new  ( std::size_t count, std::align_val_t al, const std::nothrow_t&);
// void*  operator new[]( std::size_t count, std::align_val_t al, const std::nothrow_t&);
//
// // Posix
// int    posix_memalign(void** p, size_t alignment, size_t size);
//
// // Linux
// void*  memalign(size_t alignment, size_t size);
// void*  valloc(size_t size);
// void*  pvalloc(size_t size);
// size_t malloc_usable_size(void *p);
// void*  reallocf(void* p, size_t newsize);
//
// // macOS
// void   vfree(void* p);
// size_t malloc_size(const void* p);
// size_t malloc_good_size(size_t size);
//
// // BSD
// void*  reallocarray( void* p, size_t count, size_t size );
// void*  reallocf(void* p, size_t newsize);
// void   cfree(void* p);
//
// // NetBSD
// int    reallocarr(void* p, size_t count, size_t size);
//
// // Windows
// void*  _expand(void* p, size_t newsize);
// size_t _msize(void* p);
//
// void*  _malloc_dbg(size_t size, int block_type, const char* fname, int line);
// void*  _realloc_dbg(void* p, size_t newsize, int block_type, const char* fname, int line);
// void*  _calloc_dbg(size_t count, size_t size, int block_type, const char* fname, int line);
// void*  _expand_dbg(void* p, size_t size, int block_type, const char* fname, int line);
// size_t _msize_dbg(void* p, int block_type);
// void   _free_dbg(void* p, int block_type);
