
#include <Define.hpp>

#if ODS_LINUX
  #include <sys/mman.h>

  #include <DEBUG.hpp>
  #include <OS/LINUX/LINUX_MEMORY.hpp>
  #include <OS/OS_MEMORY.hpp>
  #include <Types.hpp>
  #include <cerrno>
  #include <cstring>

namespace OS {
  bool Alloc_Physically_contiguious_mem( memory_orders orders, memory_allocations &allocation )
  {

    if ( 0 ) {

      // check for keernel active
    }
    else {
      // Alloc_contiguious_mem( orders, allocation );
    }
  }

  bool Alloc_contiguious_mem( memory_orders orders, memory_allocations &allocation )
  {

    auto prot  = PROT_READ | PROT_WRITE;
    auto flags = MAP_HUGETLB | MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE;
    switch ( orders.page ) {
      case TLB_size::page_1GB: {
        flags |= MAP_HUGE_1GB;
        break;
      }
      case TLB_size::page_2mb: {
        flags |= MAP_HUGE_2MB;

        break;
      }
    }
    void *mem;
    auto fail_recovery = [ & ]() -> bool {
      if ( mem != MAP_FAILED ) return true;

      auto count = 5;
      do {
        auto err = strerror( errno );
        LOG_ERROR( "meomory allocation failed ", err )
        mem = mmap( nullptr, orders.Quantity, prot, flags, -1, 0 );
      } while ( ( mem == MAP_FAILED ) && ( count != 0 ) );

      return ( mem != MAP_FAILED );
    };
    allocation.Quantity = orders.Quantity;
    allocation.Success  = 0;
    allocation.Failure  = 0;

    for ( U16 i = 1; i <= orders.Order_count; i++ ) {
      // try  allocate
      mem = mmap( nullptr, orders.Quantity, prot, flags, -1, 0 );
      // if fails try to fix
      if ( fail_recovery() ) {
        allocation.pointer_list[ allocation.Success ] = mem;
        allocation.Success++;
      }
      else {

        allocation.Failure++;
      }
    }

    // *mmap( size_t length; void addr[ length ], size_t length, prot, int flags, int fd, off_t offset );
  }

  void Dealloc_Physically_contiguious_mem( memory_allocations &allocation )
  {
    for ( U16 i = 0; i < allocation.Success; i++ )
      munmap( allocation.pointer_list[ i ], allocation.Quantity );
  }

  bool Alloc_Shared_contiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  bool Alloc_contiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
  bool Alloc_Shared_contiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
  bool Alloc_noncontiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  bool Alloc_Shared_noncontiguious_mem( memory_orders orders, memory_allocations &allocation ) {}
  bool Alloc_noncontiguious_swapmem( memory_orders orders, memory_allocations &allocation ) {}
}  // namespace OS
#endif
