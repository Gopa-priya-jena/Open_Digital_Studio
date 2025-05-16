
#if 0
#pragma once
#include "THREAD_ALLOCATOR.hpp"

class func_allocate {
  Function_alloc allocs;

public:
  func_allocate();
  ~func_allocate();

  void *f_malloc();
  void *f_salloc();

  void f_free(void *data);
};
#endif
