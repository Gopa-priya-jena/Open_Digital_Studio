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
    std::uint32_t Core_id;
    std::uint8_t  thread_id;

   public:
    STACK_Allocator();
    ~STACK_Allocator();

    std::uint8_t* Allocate(std::uint32_t size);

    void Deallocate(std::uint8_t* ptr);
};
