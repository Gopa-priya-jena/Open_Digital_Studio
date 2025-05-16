#pragma once
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace OS::INFO {

constexpr std::uint16_t cpu_Max_ = 2048;
struct OS_mem_info {
  std::uint64_t physical_memory;
  std::uint64_t free_physical_memory;
  std::uint64_t Swap;
  std::uint64_t Swap_busy;
  std::uint64_t Swap_free;
  std::uint64_t page_size;
  std::uint64_t Huge_page_size;
  std::uint64_t HugePages_Total;
  std::uint64_t HugePages_Free;
  std::uint64_t DirectMap4k;
  std::uint64_t DirectMap2M;
  std::uint64_t DirectMap1G;
  bool overcommit;
};

struct OS_cpu_info {
  std::uint32_t cores;
  std::uint64_t L1i_mem, L1d_mem, L2_mem, L3_mem;
  std::uint64_t L1i_mem_total, L1d_mem_total, L2_mem_total, L3_mem_total;
  std::uint8_t thread_per_core;
  std::uint8_t L1d_assoc, L1d_LineSize, L1d_setSize;
  std::uint8_t L1i_assoc, L1i_LineSize, L1i_setSize;
  std::uint8_t L2_assoc, L2_LineSize, L2_setSize;
  std::uint8_t L3_assoc, L3_LineSize, L3_setSize;
  std::uint16_t Physical_Address_Bits, Virtual_Address_Bits;
  std::uint64_t TLB_count;
  std::uint64_t TLB_pagesize;
  std::array<std::uint8_t, cpu_Max_ / 2> Topology;
  long double cpu_Max_MHz, cpu_Mix_MHz;
};

/*
will implemented if necesary  only
currently no need
struct OS_operating_system_info
{
};

struct Numa_info
{
    // TODO:: need more research on how it will be done
    // i need advice on how open mp and others have implemented
    //
    //
};

struct gpu

*/
struct OS_complete_info {
  OS_mem_info mem_info;
  OS_cpu_info cpu_info;
};

OS_complete_info GET_all_HW_info();

}; // namespace OS::INFO
