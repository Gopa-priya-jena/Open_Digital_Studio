
#include <Types.hpp> 

// use this to activate the osmemoru module
#define OS_MEMORY_UNIT
#ifdef OS_MEMORY_UNIT

#  pragma once

// the main goal of this header is to provide
// os level  imlementation of various memory
// requirment  ,this is not intened  to be  included any whwere else
// please  take carefull action here
//
//
//
// #include <>

#  include <cstdint>
struct memory_orders
{
  U64 size;
  U16 numbers;
};

// it an not be functional
// and sorry but nennds to be ojectoriented to be safe and  better

#  define PLATFORM_MEMORY_VARIABLE LINUX_MEMORY_VARIABLE
#  define PLATFORM_MEMORY_PROCESS  LINUX_MEMORY_PROCESS

void Get_Physically_contiguious_mem( memory_orders orders, void *pointers );
void Get_contiguious_mem( memory_orders orders, void *pointers );
void Get_Shared_contiguious_mem( memory_orders orders, void *pointers );
void Get_contiguious_swapmem( memory_orders orders, void *pointers );

void Get_Shared_contiguious_swapmem( memory_orders orders, void *pointers );
void Get_noncontiguious_mem( memory_orders orders, U64 min_size,
                             void *pointers );
void Get_Shared_noncontiguious_mem( memory_orders orders, void *pointers );
void Get_noncontiguious_swapmem( memory_orders orders, void *pointers );

#endif
