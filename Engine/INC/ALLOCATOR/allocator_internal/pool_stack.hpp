#pragma once

#include <ALLOCATOR/allocator_internal/memory_pool.hpp>

class pool_stack {
  pool_stack *next;
  pool_stack *prev;
  U8 *stack_ptr;
  U64 Size;
  U64 element_count;
  void *get_data();
  U64 available_space();
  void create_new( U64 hint = 0 );

 public:
  pool_stack()               = delete;
  pool_stack( pool_stack & ) = delete;
  pool_stack( pool_stack *prev, U64 Size );
  void push( void *ptr, U64 Size );
  uniform_pool &create_uniform_pool_entry( U32 uniform_element_count, U64 ptr_size );
  non_uniform_pool &create_non_uniform_pool_entry( U32 non_uniform_element_count );
  class pool_stack_ittr {
    pool_stack *stack;
    void *end_ptr;
    U64 ele;

    void *Data;

   public:
    bool end;
    pool_stack_ittr() = delete;
    pool_stack_ittr( pool_stack *st );
    pool_stack_ittr( pool_stack_ittr &ittr );
    pool_stack_ittr &operator++();
    pool_stack_ittr &operator++( int );
    void *get_last();
    void *Get_data() { return Data; }
  };
  pool_stack_ittr Get_ittr();
  friend class pool_stack_list;
};
