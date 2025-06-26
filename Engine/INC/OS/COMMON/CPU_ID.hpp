
#pragma once
#include <Compiler_Intrinsics.hpp>
#include <Types.hpp>

struct Cpuid_Regs {
  U32 EAX;
  U32 EBX;
  U32 ECX;
  U32 EDX;
};
inline U32 I_cpuid( U32 EAX, U32 ECX, Cpuid_Regs &out )
{
#if ODS_WINDOWS
  I32 cpuid_info[ 4 ];
  cpuid_info[ 0 ] = cpuid_info[ 1 ] = cpuid_info[ 2 ] = cpuid_info[ 3 ] = 0;
  __cpuidex( cpuid_info, EAX, ECX );
#endif
#if ODS_LINUX
  U32 cpuid_info[ 4 ];
  cpuid_info[ 0 ] = cpuid_info[ 1 ] = cpuid_info[ 2 ] = cpuid_info[ 3 ] = 0;
  __get_cpuid_count( EAX, ECX, &cpuid_info[ 0 ], &cpuid_info[ 1 ], &cpuid_info[ 2 ], &cpuid_info[ 3 ] );
#endif

  out.EAX = cpuid_info[ 0 ];
  out.EBX = cpuid_info[ 1 ];
  out.ECX = cpuid_info[ 2 ];
  out.EDX = cpuid_info[ 3 ];

  return ( cpuid_info[ 0 ] | cpuid_info[ 1 ] | cpuid_info[ 2 ] | cpuid_info[ 3 ] );
}
