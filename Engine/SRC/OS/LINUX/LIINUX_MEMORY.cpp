
#include <Define.hpp>
#if ODS_LINUX

  #include <OS/LINUX/LINUX_MEMORY.hpp>
  #include <OS/OS_MEMORY.hpp>
namespace OS {
  void Get_Physically_contiguious_mem( memory_orders orders, memory_allocations &allocation )
  {

    if ( 0 ) {

      // check for keernel active
    }
    else {
      Get_contiguious_mem( orders, allocation );
    }
  }
  void Get_contiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_Shared_contiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_contiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_Shared_contiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_noncontiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_Shared_noncontiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  void Get_noncontiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
}  // namespace OS
#endif
