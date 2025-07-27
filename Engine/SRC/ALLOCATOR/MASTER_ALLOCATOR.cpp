#include <Define.hpp>

#include "ALLOCATOR/mem_internals.hpp"
#include "OS/OS_MEMORY.hpp"
#if ODS_LINUX
  #include <ALGORITHMS/ALGORITHMS.hpp>
  #include <ALLOCATOR/MASTER_ALLOCATOR.hpp>
  #include <DATA_STRUCTURE/DATA_STRUCTURE.hpp>
  #include <OS/OS_INFO.hpp>

namespace ALLOCATOR {

  U64 calc_thread_cahe( OS::INFO::OS_complete_info& info )
  {
    U64 page_size = 0;
    if ( info.mem_info.MAX_Huge_page_size ) {
      page_size = info.mem_info.MAX_Huge_page_size;
    }

    if ( page_size == 0 ) {

      page_size = info.mem_info.page_size;
    }

    if ( page_size < ( 500 << 20 ) ) {
    }
    else {
    }
  }
  void MASTER_ALLOCATOR::Base_init( U64 pre_threads )
  {
    memory_orders orders;
    orders.Order_count = 1;
    orders.Quantity    = pre_threads * Page_size;

    // OS::Alloc_Physically_contiguious_mem( orders, &allocation )
  }
  void MASTER_ALLOCATOR::Base_grow( U64 pre_threads ) {}
  void MASTER_ALLOCATOR::Base_destroy( U64 pre_threads ) {}

  MASTER_ALLOCATOR::MASTER_ALLOCATOR()
  {
    // collect info about cpu  and strorage
    auto complete_info = OS::INFO::GET_all_HW_info();
    auto& cpu          = complete_info.cpu_info;
    auto& mem          = complete_info.mem_info;
    U64 Temp           = cpu.cores * cpu.thread_per_core;
    Temp               = Temp << 4;

    Page_size          = mem.page_size;
    MAX_Huge_page_size = mem.Huge_page_size;
    Base_init( Temp );
  }
}  // namespace ALLOCATOR

#endif
