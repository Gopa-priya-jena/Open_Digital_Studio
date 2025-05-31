#if 0

#include <ALLOCATOR/MASTER_ALLOCATOR/MASTER_ALLOCATOR.hpp>
#include <DATA_STRUCTURE/DATA_STRUCTURE.hpp>
#include <cstdint>

#include "DEBUG.hpp"
#include "OS/OS_INFO.hpp"

MASTER_ALLOCATOR::MASTER_ALLOCATOR()
{
    // collect info about cpu  and strorage
    auto          complete_info = OS::INFO::OS_complete_info();
    U64   per_thread_cache;
    // check l3 chae size for per  core



    per_thread_cache =
        complete_info.cpu_info.L3_mem_total / complete_info.cpu_info.cores;

    LOG(per_thread_cache);
    per_thread_cache *= 20;
    U16 total_allocations =
        complete_info.cpu_info.cores * complete_info.cpu_info.thread_per_core;
    // shared memory ( for threads on thwe ame core) 1 ~ thread_per_core_mb

    // arena = 30
    // micro size ~50mb *10
    // small size ~100mb*5
    // medium size ~300mb*3   // in studio
    // Large size ~ 6000mb*2  // in db  only
    // uber Size ~ 800mb*2    // in db only

    // buffer POOL =100
    // 1GB *2 of 2K

    // general allocator = 10
    // ~2GB of 64k
    //
}
#endif
