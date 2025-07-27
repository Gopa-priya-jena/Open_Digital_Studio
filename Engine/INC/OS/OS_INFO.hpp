#pragma once
#include <Types.hpp>
#include <array>
namespace OS::INFO {

  constexpr U16 cpu_Max_ = 2048;
  struct OS_mem_info {
    U64 physical_memory;
    U64 free_physical_memory;
    U64 Swap;
    U64 Swap_busy;
    U64 Swap_free;
    U64 page_size;
    U64 MAX_Huge_page_size;
    U64 Huge_page_size;
    U64 HugePages_Total;
    U64 HugePages_Free;
    U64 DirectMap4k;
    U64 DirectMap2M;
    U64 DirectMap1G;
    bool overcommit;
  };

  struct OS_cpu_info {

    char VendorId[ 13 ];  // Processor vendor ID string
    char _ProcessorBrand[ 49 ]{ "unknown" };
    U32 cores;
    U64 L1i_mem, L1d_mem, L2_mem, L3_mem;
    U64 L1i_mem_total, L1d_mem_total, L2_mem_total, L3_mem_total;
    U32 thread_per_core;
    U32 L1d_ways, L1d_LineSize, L1d_setSize;
    U32 L1i_ways, L1i_LineSize, L1i_setSize;
    U32 L2_ways, L2_LineSize, L2_setSize;
    U32 L3_ways, L3_LineSize, L3_setSize;
    U16 Physical_Address_Bits, Virtual_Address_Bits;
    // std::array< U8, cpu_Max_ / 2 > Topology;
    F64 cpu_Max_MHz, cpu_Mix_MHz;
    U32 Numa;
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

};  // namespace OS::INFO
