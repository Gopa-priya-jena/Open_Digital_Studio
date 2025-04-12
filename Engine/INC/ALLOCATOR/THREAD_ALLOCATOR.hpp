#pragma once
#include <cstdint>
#include <thread>

struct Function_alloc
{
    void*         Base_pointer;
    std::uint16_t Size;
    std::uint16_t Request;
};

class THREAD_ALLOCATOR
{
    void*         Base_pointer;
    void*         END_pointer;
    void*         stack_head_pointer;
    void*         heap_top_pointer;
    uintptr_t     cache;
    std::uint64_t Size;

    uint64_t* free_list;

   public:
    THREAD_ALLOCATOR();

    ~THREAD_ALLOCATOR();

    void* STACK_ALLOCATE();

    void STACK_DEALLOCATE(void* free_ptr);

    void* HEAP_ALLOCATE();

    void* HEAP_DEALLOCATE(void* free_ptr);

    Function_alloc FUNCTION_ALLOCATE();
    Function_alloc FUNCTION_ALLOC_UPDATE(Function_alloc& falloc);

    void FUNCTION_DEALLOCATE(Function_alloc falloc);
};
