#pragma once
#include <cstdint>
class concurrent_stack_Allocator
{
};
class POOL_allocator
{
};
class arena_allocator
{
};
class STACK_Allocator
{
    // make per thread specific
    U32   Core_id;
    U8  thread_id;

   public:
    STACK_Allocator();
    ~STACK_Allocator();

     U8 * Allocate( U32  size);

    void Deallocate( U8 * ptr);
};
