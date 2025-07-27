#pragma once
#include <ALLOCATOR/allocator_internal/pool_stack.hpp>
class pool_stack_list {
  pool_stack *head;
  pool_stack *tail;
  pool_stack *present;

  void update_tail();

 public:
  pool_stack_list();
  pool_stack_list( pool_stack_list & ) = delete;
  ~pool_stack_list();

  pool_stack *get_present();

  pool_stack_list &operator++();
  pool_stack_list &operator++( int );

  pool_stack_list &operator--();
  pool_stack_list &operator--( int );
};
