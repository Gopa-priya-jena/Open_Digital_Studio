#pragma once
#include <ALLOCATOR/mem_internals.hpp>
#include <OS/LINUX/LINUX_MEMORY.hpp>
#include <OS/WINDOWS/WINDOWS_MEMORY.hpp>
#include <Types.hpp>

// the main goal of this header is to provide
// os level  imlementation of various memory
// requirment  ,this is not intened  to be  included anywhwere else
// please  take carefull action here
//

//
/// INFO: keep these functional and simple
//
namespace OS {
#define ALLOCA( X ) LINUX_ALLOCA( X ) WINDOWS_ALLOCA( X )

  bool Alloc_Physically_contiguious_mem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_contiguious_mem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_Shared_contiguious_mem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_contiguious_swapmem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_Shared_contiguious_swapmem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_noncontiguious_mem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_Shared_noncontiguious_mem( memory_orders orders, memory_allocations &allocation );
  bool Alloc_noncontiguious_swapmem( memory_orders orders, memory_allocations &allocation );

  void Dealloc_Physically_contiguious_mem( memory_allocations &allocation );
  void Dealloc_contiguious_mem( memory_allocations &allocation );
  void Dealloc_Shared_contiguious_mem( memory_allocations &allocation );
  void Dealloc_contiguious_swapmem( memory_allocations &allocation );
  void Dealloc_Shared_contiguious_swapmem( memory_allocations &allocation );
  void Dealloc_noncontiguious_mem( memory_allocations &allocation );
  void Dealloc_Shared_noncontiguious_mem( memory_allocations &allocation );
  void Dealloc_noncontiguious_swapmem( memory_allocations &allocation );

}  // namespace OS
