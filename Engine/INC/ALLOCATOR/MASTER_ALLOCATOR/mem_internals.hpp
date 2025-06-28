#pragma once
#include <Types.hpp>

//////////////////////////////////////
// these are some necessary data structure
///
struct memory_orders {
  U64 Quantity;
  U16 Order_count;
};

struct memory_allocations {
  U64 Quantity;
  U32 success;
  U32 Failure;
  void* pointer_list[];
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
