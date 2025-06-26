#pragma once
#include <DATA_STRUCTURE/DATA_STRUCTURE.hpp>
#include <OS/COMMON/CPU_ID.hpp>
#include <iostream>
// NOTE: cometed most  of what needed

namespace OS {

  class Cpu_Info_Collector {
    char _VendorId[ 13 ];  // Processor vendor ID string
    char _ProcessorBrand[ 49 ]{ "unknown" };
    enum cpu_vendor { AMD, INTEL } vendor;
    U32 Max_eax;

    inline bool check_bit( U64 Input, U64 bit ) const
    {
      U64 Bit = 1;
      return ( Input & ( Bit << bit ) );
    }
    void collect_vendor();
    void collect_ProcessorBrand();
    void collect_Feature();

   public:
    struct {
      bool SSE3                = 0;
      bool PCLMULQDQ           = 0;
      bool DTES64              = 0;
      bool MONITOR             = 0;
      bool CPL                 = 0;
      bool VMX                 = 0;
      bool SMX                 = 0;
      bool EST                 = 0;
      bool TM2                 = 0;
      bool SSSE3               = 0;
      bool CNXT_ID             = 0;
      bool SDBG                = 0;
      bool FMA                 = 0;
      bool CMPXCHG16B          = 0;
      bool xTPR_Update_Control = 0;
      bool PDCM                = 0;
      bool PCID                = 0;
      bool DCA                 = 0;
      bool SSE4_1              = 0;
      bool SSE4_2              = 0;
      bool x2APIC              = 0;
      bool MOVBE               = 0;
      bool POPCNT              = 0;
      bool TSC_Deadline        = 0;
      bool AES                 = 0;
      bool XSAVE               = 0;
      bool OSXSAVE             = 0;
      bool AVX                 = 0;
      bool F16C                = 0;
      bool RDRAND3             = 0;
      bool FPU                 = 0;
      bool VME                 = 0;
      bool DE                  = 0;
      bool PSE                 = 0;
      bool TSC                 = 0;
      bool MSR                 = 0;
      bool PAE                 = 0;
      bool MCE                 = 0;
      bool CMPXCHG8B           = 0;
      bool APIC                = 0;
      bool SEP                 = 0;
      bool MTRR                = 0;
      bool PGE                 = 0;
      bool MCA                 = 0;
      bool CMOV                = 0;
      bool PAT                 = 0;
      bool PSE_36              = 0;
      bool PSN                 = 0;
      bool CLFSH               = 0;
      bool DS                  = 0;
      bool ACPI                = 0;
      bool MMX                 = 0;
      bool FXSR                = 0;
      bool SSE                 = 0;
      bool SSE2                = 0;
      bool SS                  = 0;
      bool HTT                 = 0;
      bool TM                  = 0;
      bool PBE                 = 0;
    } feature;
    Cpu_Info_Collector();
    ~Cpu_Info_Collector() { std::cout << "ended cpu info collection\n"; }
  };
}  // namespace OS
