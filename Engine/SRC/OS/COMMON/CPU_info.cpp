#include <OS/COMMON/CPU_info.hpp>

// some common commnads
constexpr struct {
  U32 VENDOR = 0X0;
  U32 MAX_EXTENDED_CPUID = 0X80000000;
  U32 BRAND_STRING2 = 0x80000002;
  U32 BRAND_STRING3 = 0x80000003;
  U32 BRAND_STRING4 = 0x80000004;
  U32 FEATURE_IDENTIFIER = 0x00000001;

} command;
Cpu_Info_Collector::Cpu_Info_Collector() {
  collect_vendor();
  collect_ProcessorBrand();
  collect_Feature();
}
void Cpu_Info_Collector::collect_vendor() {
  Cpuid_Regs reg;
  I_cpuid(0, 0, reg);
  Max_eax = reg.EAX;
  uint32_t *p = (uint32_t *)_VendorId;
  p[0] = reg.EBX;
  p[1] = reg.EDX;
  p[2] = reg.ECX;
  _VendorId[12] = '\0';

  std::string_view sv = _VendorId;

  if (sv.find("AuthenticAMD") != std::string_view::npos)
    vendor = AMD;
  else
    vendor = INTEL;

  std::cout << _VendorId << "\t EAX" << reg.EAX << "VENDOR \t" << vendor
            << std::endl;
}
void Cpu_Info_Collector::collect_ProcessorBrand() {
  Cpuid_Regs r1;

  I_cpuid(command.MAX_EXTENDED_CPUID, 0, r1);
  if (r1.EAX < command.BRAND_STRING4)
    return;
  Cpuid_Regs r2, r3, r4;
  uint32_t *p = (uint32_t *)_ProcessorBrand;
  I_cpuid(command.BRAND_STRING2, 0, r2);
  I_cpuid(command.BRAND_STRING3, 0, r3);
  I_cpuid(command.BRAND_STRING4, 0, r4);

  p[0] = r2.EAX;
  p[1] = r2.EBX;
  p[2] = r2.ECX;
  p[3] = r2.EDX;
  p[4] = r3.EAX;
  p[5] = r3.EBX;
  p[6] = r3.ECX;
  p[7] = r3.EDX;
  p[8] = r4.EAX;
  p[9] = r4.EBX;
  p[10] = r4.ECX;
  p[11] = r4.EDX;

  _ProcessorBrand[48] = '\0';
  std::cout << _ProcessorBrand << std::endl;
}
void Cpu_Info_Collector::collect_Feature() {
  Cpuid_Regs reg;
  I_cpuid(command.FEATURE_IDENTIFIER, 0, reg);

  // ok certains checks are for only Intel and not amd
  // ECX feaures check
  if (check_bit(reg.ECX, SSE3))
    feature.SSE3 = true;

  if (check_bit(reg.ECX, PCLMULQDQ))
    feature.PCLMULQDQ = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, DTES64))
      feature.DTES64 = true;

  if (check_bit(reg.ECX, MONITOR))
    feature.MONITOR = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, CPL))
      feature.CPL = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, VMX))
      feature.VMX = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, SMX))
      feature.SMX = true;

  if (check_bit(reg.ECX, SSSE3))
    feature.SSSE3 = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, CNXT_ID))
      feature.CNXT_ID = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, SDBG))
      feature.SDBG = true;

  if (check_bit(reg.ECX, FMA))
    feature.FMA = true;

  if (check_bit(reg.ECX, CMPXCHG16B))
    feature.CMPXCHG16B = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, xTPR_Update_Control))
      feature.xTPR_Update_Control = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, PDCM))
      feature.PDCM = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, PCID))
      feature.PCID = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, DCA))
      feature.DCA = true;

  if (check_bit(reg.ECX, SSE4_1))
    feature.SSE4_1 = true;

  if (check_bit(reg.ECX, SSE4_2))
    feature.SSE4_2 = true;

  if (check_bit(reg.ECX, x2APIC))
    feature.x2APIC = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, MOVBE))
      feature.MOVBE = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, POPCNT))
      feature.POPCNT = true;

  if (vendor == INTEL)
    if (check_bit(reg.ECX, TSC_Deadline))
      feature.TSC_Deadline = true;

  if (check_bit(reg.ECX, AES))
    feature.AES = true;

  if (check_bit(reg.ECX, XSAVE))
    feature.XSAVE = true;

  if (check_bit(reg.ECX, OSXSAVE))
    feature.OSXSAVE = true;

  if (check_bit(reg.ECX, AVX))
    feature.AVX = true;

  if (check_bit(reg.ECX, F16C))
    feature.F16C = true;

  if (check_bit(reg.ECX, RDRAND3))
    feature.RDRAND3 = true;

  if (check_bit(reg.EDX, FPU))
    feature.FPU = true;
  if (check_bit(reg.EDX, VME))
    feature.VME = true;

  if (check_bit(reg.EDX, DE))
    feature.DE = true;

  if (check_bit(reg.EDX, PSE))
    feature.PSE = true;

  if (check_bit(reg.EDX, TSC))
    feature.TSC = true;

  if (check_bit(reg.EDX, MSR))
    feature.MSR = true;

  if (check_bit(reg.EDX, PAE))
    feature.PAE = true;

  if (check_bit(reg.EDX, MCE))
    feature.MCE = true;

  if (check_bit(reg.EDX, CMPXCHG8B))
    feature.CMPXCHG8B = true;

  if (check_bit(reg.EDX, APIC))
    feature.APIC = true;

  if (check_bit(reg.EDX, SEP))
    feature.SEP = true;

  if (check_bit(reg.EDX, MTRR))
    feature.MTRR = true;

  if (check_bit(reg.EDX, PGE))
    feature.PGE = true;

  if (check_bit(reg.EDX, MCA))
    feature.MCA = true;

  if (check_bit(reg.EDX, CMOV))
    feature.CMOV = true;

  if (check_bit(reg.EDX, PAT))
    feature.PAT = true;

  if (check_bit(reg.EDX, PSE_36))
    feature.PSE_36 = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, PSN))
      feature.PSN = true;

  if (check_bit(reg.EDX, CLFSH))
    feature.CLFSH = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, DS))
      feature.DS = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, ACPI))
      feature.ACPI = true;

  if (check_bit(reg.EDX, MMX))
    feature.MMX = true;

  if (check_bit(reg.EDX, FXSR))
    feature.FXSR = true;

  if (check_bit(reg.EDX, SSE))
    feature.SSE = true;

  if (check_bit(reg.EDX, SSE2))
    feature.SSE2 = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, SS))
      feature.SS = true;

  if (check_bit(reg.EDX, HTT))
    feature.HTT = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, TM))
      feature.TM = true;

  if (vendor == INTEL)
    if (check_bit(reg.EDX, PBE))
      feature.PBE = true;
}
