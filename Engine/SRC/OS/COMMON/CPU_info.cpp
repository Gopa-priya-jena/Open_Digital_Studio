#include <OS/COMMON/CPU_ID.hpp>
#include <OS/COMMON/CPU_info.hpp>

namespace OS {

  // some common commnads
  enum Feature_check {
    /* clang-format off */
    // this infor mation was collected from architecture-instruction-set-extensions-programming-reference-dec-24 
    // ECX REGISTER INFO
    // Bit # 	Mnemonic 	Description
    // 0 	SSE3 	Intel®  Streaming SIMD Extensions 3 (Intel®  SSE3). A value of 1 indicates the processor supports this technology.
    // 1 	PCLMULQDQ 	A value of 1 indicates the processor supports the PCLMULQDQ instruction.
    // 2 	DTES64 	64-bit DS Area. A value of 1 indicates the processor supports DS area using 64-bit layout.
    // 3 	MONITOR 	MONITOR/MWAIT. A value of 1 indicates the processor supports this feature.
    // 4 	DS-CPL 	CPL Qualified Debug Store. A value of 1 indicates the processor supports the extensions to the Debug Store feature to allow for branch message storage qualified by CPL.
    // 5 	VMX 	Virtual Machine Extensions. A value of 1 indicates that the processor supports this technology.
    // 6 	SMX 	Safer Mode Extensions. A value of 1 indicates that the processor supports this technology. See Chapter 7, “Safer Mode Extensions Reference.”
    // 7 	EST 	Enhanced Intel SpeedStep® Technology. A value of 1 indicates that the processor supports this technology.
    // 8 	TM2 	Thermal Monitor 2. A value of 1 indicates whether the processor supports this technology.
    // 9 	SSSE3 	A value of 1 indicates the presence of the Supplemental Streaming SIMD Extensions 3 (SSSE3). A value of 0 indicates the instruction extensions are not present in the processor.
    // 10 	CNXT-ID 	L1 Context ID. A value of 1 indicates the L1 data cache mode can be set to either adaptive mode or shared mode. A value of 0 indicates this feature is not supported. See definition of the IA32_MISC_ENABLE MSR Bit 24 (L1 Data Cache Context Mode) for details.
    // 11 	SDBG 	A value of 1 indicates the processor supports IA32_DEBUG_INTERFACE MSR for silicon debug.
    // 12 	FMA 	A value of 1 indicates the processor supports FMA extensions using YMM state.
    // 13 	CMPXCHG16B 	CMPXCHG16B Available. A value of 1 indicates that the feature is available.
    // 14 	xTPR Update Control 	xTPR Update Control. A value of 1 indicates that the processor supports changing IA32_MISC_ENABLES[bit 23].
    // 15 	PDCM 	Perfmon and Debug Capability. A value of 1 indicates the processor supports the performance and debug feature indication MSR IA32_PERF_CAPABILITIES.
    // 16 	Reserved 	Reserved.
    // 17 	PCID 	Process-context identifiers. A value of 1 indicates that the processor supports PCIDs and that software may set CR4.PCIDE to 1.
    // 18 	DCA 	A value of 1 indicates the processor supports the ability to prefetch data from a memory mapped device.
    // 19 	SSE4.1 	A value of 1 indicates that the processor supports SSE4.1.
    // 20 	SSE4.2 	A value of 1 indicates that the processor supports SSE4.2.
    // 21 	x2APIC 	A value of 1 indicates that the processor supports x2APIC feature.
    // 22 	MOVBE 	A value of 1 indicates that the processor supports MOVBE instruction.
    // 23 	POPCNT 	A value of 1 indicates that the processor supports the POPCNT instruction.
    // 24 	TSC-Deadline 	A value of 1 indicates that the processor’s local APIC timer supports one-shot operation using a TSC deadline value.
    // 25 	AES 	A value of 1 indicates that the processor supports the AESNI instruction extensions.
    // 26 	XSAVE 	A value of 1 indicates that the processor supports the XSAVE/XRSTOR processor extended states feature,
    // the XSETBV / XGETBV         instructions,
    // and XCR0.27 OSXSAVE A value of 1 indicates that the OS has set
    //             CR4.OSXSAVE[ bit 18 ] to enable XSETBV /
    //         XGETBV instructions to access XCR0 and to support processor extended
    //             state             management using XSAVE /
    //         XRSTOR.28 AVX A value of 1 indicates that processor supports AVX
    //             instructions operating on 256 -
    //     bit YMM                        state,
    // and three - operand encoding       of 256 - bit and
    //     128 -
    //         bit SIMD instructions.29 F16C A value of 1 indicates that processor
    //                                               supports 16 -
    //         bit                                   floating -
    //         point conversion instructions.30 RDRAND A value of 1 indicates that
    //             processor supports RDRAND instruction.31 Not Used
    //                                       Always return 0.
    //
    //         EDX REGISTER INFO
    //
    //             Bit #Mnemonic Description 0 FPU Floating -
    //         point Unit                          On -
    //         Chip.The processor contains an x87 FPU.1 VME Virtual 8086 Mode
    //             Enhancements.Virtual 8086 mode enhancements, including CR4.VME for controlling the feature, CR4.PVI for protected mode virtual interrupts,
    // software interrupt indirection,
    // expansion of the TSS with the software indirection bitmap, and EFLAGS.VIF and EFLAGS.VIP flags.
    // 2 		DE 	Debugging Extensions. Support for I/O breakpoints, including CR4.DE for controlling the feature,
    // and optional trapping of accesses
    //         to DR4 and DR5.Bit #Mnemonic Description 3 PSE Page Size
    //             Extension.Large pages of size 4 MByte are supported, including CR4.PSE for controlling the feature,
    // the defined dirty bit in          PDE( Page Directory Entries ),
    // optional reserved bit trapping in CR3,
    // PDEs,
    // and PTEs.4 TSC Time Stamp Counter.The RDTSC instruction is supported, including CR4.TSD for controlling privilege.
    // 5 MSR 	Model Specific Registers RDMSR and WRMSR Instructions. The RDMSR and WRMSR instructions are supported. Some of the MSRs are implementation dependent.
    // 6 PAE 	Physical Address Extension. Physical addresses greater than 32 bits are supported: extended page table entry formats,
    // an extra level in the page translation tables is defined,
    // 2 - MByte pages are supported instead            of 4 Mbyte pages if PAE bit
    //         is 1. The actual number of address bits beyond 32 is not defined, and is implementation specific.
    // 7  MCE 	Machine Check Exception. Exception 18 is defined for Machine Checks, including CR4.MCE for controlling the feature. This feature does not define the model-specific implementations of machine-check error logging,
    // reporting,
    // and processor shutdowns.Machine Check exception handlers may have to depend
    //         on processor version to do model specific processing of the
    //             exception, or test for the presence of the Machine Check feature.
    // 8 CX8 	CMPXCHG8B Instruction. The compare-and-exchange 8 bytes (64 bits) instruction is supported (implicitly locked and atomic).
    // 9 APIC 	APIC On-Chip. The processor contains an Advanced Programmable Interrupt Controller (APIC),
    // responding
    //     to memory mapped commands in the physical address range FFFE0000H to
    //     FFFE0FFFH( by default - some processors permit the APIC to be
    //                                 relocated ) .10 Reserved Reserved
    //     .11 SEP SYSENTER and SYSEXIT Instructions
    //         .The SYSENTER and SYSEXIT and associated MSRs are supported
    //     .12 MTRR Memory Type Range Registers.MTRRs are supported.The MTRRcap MSR
    //     contains feature bits that describe what memory types are supported,
    // how many variable MTRRs are supported,
    // and whether fixed MTRRs are supported.13 PGE Page Global Bit.The global bit
    //         is supported in                                  paging -
    //     structure entries that map a                         page,
    // indicating TLB entries that are common to
    //     different processes and need not be flushed.The CR4
    //         .PGE bit controls this feature.14 MCA Machine Check
    //             Architecture.The Machine Check Architecture, which provides a compatible mechanism for error reporting in P6 family,
    // Pentium 4,
    // Intel Xeon processors,
    // and future processors,
    // is         supported
    //     .The MCG_CAP MSR contains feature bits describing how many banks of
    //         error reporting MSRs are supported.15 CMOV Conditional Move
    //                 Instructions.The conditional move instruction CMOV is
    //                 supported.In              addition,
    // if x87 FPU is present as indicated by the CPUID.FPU feature bit,
    // then the FCOMI and FCMOV instructions are                   supported 16 PAT
    //     Page Attribute Table.Page Attribute Table is supported.This
    //         feature augments the Memory Type Range
    //         Registers( MTRRs ), allowing an operating system to specify attributes of memory accessed through a linear address on a 4KB granularity.
    // 17 PSE-36 	36-Bit Page Size Extension. 4-MByte pages addressing physical memory beyond 4 GBytes are supported with 32-bit paging. This feature indicates that upper bits of the physical address of a 4-MByte page are encoded in bits 20:13 of the page-directory entry. Such physical addresses are limited by MAXPHYADDR and may be up to 40 bits in size.
    // 18 PSN 	Processor Serial Number. The processor supports the 96-bit processor identification number feature and the feature is enabled.
    // 19 CLFSH 	CLFLUSH Instruction. CLFLUSH Instruction is supported.
    // 20 Reserved 	Reserved.
    // Bit	# 	Mnemonic 	Description
    // 21 		DS 	Debug Store. The processor supports the ability to write debug information into a memory resident buffer. This feature is used by the branch trace store (BTS) and precise event-based sampling (PEBS) facilities (see Chapter 25, “Introduction to Virtual-Machine Extensions,” in the Intel ® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3C).
    // 22 		ACPI 	Thermal Monitor and Software Controlled Clock Facilities. The processor implements internal MSRs that allow processor temperature to be monitored and processor performance to be modulated in predefined duty cycles under software control.
    // 23 		MMX 	Intel MMX Technology. The processor supports the Intel MMX technology.
    // 24 		FXSR 	FXSAVE and FXRSTOR Instructions. The FXSAVE and FXRSTOR instructions are supported for fast save and restore of the floating-point context. Presence of this bit also indicates that CR4.OSFXSR is available for an operating system to indicate that it supports the FXSAVE and FXRSTOR instructions.
    // 25 		SSE 	SSE. The processor supports the SSE extensions.
    // 26 		SSE2 	SSE2. The processor supports the SSE2 extensions.
    // 27 		SS 	Self Snoop. The processor supports the management of conflicting memory types by performing a snoop of its own cache structure for transactions issued to the bus.
    // 28 		HTT 	Max APIC IDs reserved field is Valid. A value of 0 for HTT indicates there is only a single logical processor in the package and software should assume only a single APIC ID is reserved. A value of 1 for HTT indicates the value in CPUID.1.EBX[23:16] (the Maximum number of addressable IDs for logical processors in this package) is valid for the package.
    // 29 		TM 	Thermal Monitor. The processor implements the thermal monitor automatic thermal control circuitry (TCC).
    // 30 		Reserved 	Reserved.
    // 31 		PBE 	Pending Break Enable. The processor supports the use of the FERR#/PBE# pin when the processor is in the stop-clock state (STPCLK# is asserted) to signal the processor that an interrupt is pending and that the processor should return to normal operation to handle the interrupt. Bit 10 (PBE enable) in the IA32_MISC_ENABLE MSR enables this capability.
    //
    // // ECX
    // /*
    //  + -----+--------------+--------------------------------------*----+
    //  | Bit |      Flag      | Description                              |
    //  +-----+--------------+------------------------------------------+
    //  | 31  |     (n/a)    | Not used                                 |
    //  | 30  |    RDRAND    | RDRAND instruction supported             |
    //  | 29  |     F16C     | F16C (half-precision) conversions        |
    //  | 28  |      AVX     | Advanced Vector Extensions supported     |
    //  | 27  |   OSXSAVE    | OS has enabled XSAVE                     |
    //  | 26  |     XSAVE    | XSAVE/XRSTOR/XSETBV/XGETBV instructions  |
    //  | 25  |      AES     | AES instruction set supported            |
    //  | 24  | TSC-Deadline | TSC-Deadline timer support               |
    //  | 23  |    POPCNT    | POPCNT instruction supported             |
    //  | 22  |     MOVBE    | MOVBE instruction supported              |
    //  | 21  |    x2APIC    | x2APIC support                           |
    //  | 20  |    SSE4.2    | SSE4.2 extensions supported              |
    //  | 19  |    SSE4.1    | SSE4.1 extensions supported              |
    //  | 18  |      DCA     | Direct Cache Access                      |
    //  | 17  |     PCID     | Process-context Identifiers              |
    //  | 16  |     PDCM     | Perf/Debug Capability MSR                |
    //  | 15  |   (Reserved) | Reserved                                 |
    //  | 14  |  xTPR Update | xTPR Update Control                      |
    //  | 13  |  CMPXCHG16B  | CMPXCHG16B instruction                   |
    //  | 12  |      FMA     | Fused Multiply-Add extensions            |
    //  | 11  |     (n/a)    | Not used                                 |
    //  | 10  |    CNXT-ID   | L1 Context ID                            |
    //  | 9   |     SDBG     | Silicon Debug                            |
    //  | 8   |     TM2      | Thermal Monitor 2                        |
    //  | 7   |      EST     | Enhanced Intel SpeedStep Technology      |
    //  | 6   |      SMX     | Safer Mode Extensions                    |
    //  | 5   |      VMX     | Virtual Machine Extensions               |
    //  | 4   |     DS-CPL   | CPL Qualified Debug Store                |
    //  | 3   |    MONITOR   | MONITOR/MWAIT instructions               |
    //  | 2   |     DTES64   | 64-bit Debug Store Area                  |
    //  | 1   |  PCLMULQDQ   | Carry-less Multiplication instruction    |
    //  | 0   |     SSE3     | SSE3 extensions supported                |
    //  +-----+--------------+------------------------------------------+
    //  clang-format on 
    SSE3 = 0,
    PCLMULQDQ = 1,
    DTES64 = 2,
    MONITOR = 3,
    CPL = 4,
    VMX = 5,
    SMX = 6,
    EST = 7,
    TM2 = 8,
    SSSE3 = 9,
    CNXT_ID = 10,
    SDBG = 11,
    FMA = 12,
    CMPXCHG16B = 13,
    xTPR_Update_Control = 14,
    PDCM = 15,
    Reserved = 16,
    PCID = 17,
    DCA = 18,
    SSE4_1 = 19,
    SSE4_2 = 20,
    x2APIC = 21,
    MOVBE = 22,
    POPCNT = 23,
    TSC_Deadline = 24,
    AES = 25,
    XSAVE = 26,
    OSXSAVE = 27,
    AVX = 28,
    F16C = 29,
    RDRAND3 = 30,
    // EDX

    FPU = 0,
    VME = 1,
    DE = 2,
    PSE = 3,
    TSC = 4,
    MSR = 5,
    PAE = 6,
    MCE = 7,
    CMPXCHG8B = 8,
    APIC = 9,
    _Reserved = 10,
    SEP = 11,
    MTRR = 12,
    PGE = 13,
    MCA = 14,
    CMOV = 15,
    PAT = 16,
    PSE_36 = 17,
    PSN = 18,
    CLFSH = 19,
    __Reserved = 20,
    DS = 21,
    ACPI = 22,
    MMX = 23,
    FXSR = 24,
    SSE = 25,
    SSE2 = 26,
    SS = 27,
    HTT = 28,
    TM = 29,
    ___Reserved = 30,
    PBE = 31,
  };

constexpr struct {
  U32 VENDOR             = 0X0;
  U32 MAX_EXTENDED_CPUID = 0X80000000;
  U32 BRAND_STRING2      = 0x80000002;
  U32 BRAND_STRING3      = 0x80000003;
  U32 BRAND_STRING4      = 0x80000004;
  U32 FEATURE_IDENTIFIER = 0x00000001;
  // U32 

} command;
Cpu_Info_Collector::Cpu_Info_Collector()
{
  collect_vendor();
  collect_ProcessorBrand();
  collect_Feature();
}
void Cpu_Info_Collector::collect_vendor()
{
  Cpuid_Regs reg;
  I_cpuid( 0, 0, reg );
  Max_eax         = reg.EAX;
  uint32_t *p     = ( uint32_t * )_VendorId;
  p[ 0 ]          = reg.EBX;
  p[ 1 ]          = reg.EDX;
  p[ 2 ]          = reg.ECX;
  _VendorId[ 12 ] = '\0';

  std::string_view sv = _VendorId;

  if ( sv.find( "AuthenticAMD" ) != std::string_view::npos )
    vendor = AMD;
  else
    vendor = INTEL;

  std::cout << _VendorId << "\t EAX" << reg.EAX << "VENDOR \t" << vendor << std::endl;
}
void Cpu_Info_Collector::collect_ProcessorBrand()
{
  Cpuid_Regs r1;

  I_cpuid( command.MAX_EXTENDED_CPUID, 0, r1 );
  if ( r1.EAX < command.BRAND_STRING4 ) return;
  Cpuid_Regs r2, r3, r4;
  uint32_t *p = ( uint32_t * )_ProcessorBrand;
  I_cpuid( command.BRAND_STRING2, 0, r2 );
  I_cpuid( command.BRAND_STRING3, 0, r3 );
  I_cpuid( command.BRAND_STRING4, 0, r4 );

  p[ 0 ]  = r2.EAX;
  p[ 1 ]  = r2.EBX;
  p[ 2 ]  = r2.ECX;
  p[ 3 ]  = r2.EDX;
  p[ 4 ]  = r3.EAX;
  p[ 5 ]  = r3.EBX;
  p[ 6 ]  = r3.ECX;
  p[ 7 ]  = r3.EDX;
  p[ 8 ]  = r4.EAX;
  p[ 9 ]  = r4.EBX;
  p[ 10 ] = r4.ECX;
  p[ 11 ] = r4.EDX;

  _ProcessorBrand[ 48 ] = '\0';
  std::cout << _ProcessorBrand << std::endl;
}
void Cpu_Info_Collector::collect_Feature()
{
  Cpuid_Regs reg;
  I_cpuid( command.FEATURE_IDENTIFIER, 0, reg );

  // ok certains checks are for only Intel and not amd
  // ECX feaures check
  if ( check_bit( reg.ECX, SSE3 ) ) feature.SSE3 = true;

  if ( check_bit( reg.ECX, PCLMULQDQ ) ) feature.PCLMULQDQ = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, DTES64 ) ) feature.DTES64 = true;

  if ( check_bit( reg.ECX, MONITOR ) ) feature.MONITOR = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, CPL ) ) feature.CPL = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, VMX ) ) feature.VMX = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, SMX ) ) feature.SMX = true;

  if ( check_bit( reg.ECX, SSSE3 ) ) feature.SSSE3 = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, CNXT_ID ) ) feature.CNXT_ID = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, SDBG ) ) feature.SDBG = true;

  if ( check_bit( reg.ECX, FMA ) ) feature.FMA = true;

  if ( check_bit( reg.ECX, CMPXCHG16B ) ) feature.CMPXCHG16B = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, xTPR_Update_Control ) ) feature.xTPR_Update_Control = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, PDCM ) ) feature.PDCM = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, PCID ) ) feature.PCID = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, DCA ) ) feature.DCA = true;

  if ( check_bit( reg.ECX, SSE4_1 ) ) feature.SSE4_1 = true;

  if ( check_bit( reg.ECX, SSE4_2 ) ) feature.SSE4_2 = true;

  if ( check_bit( reg.ECX, x2APIC ) ) feature.x2APIC = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, MOVBE ) ) feature.MOVBE = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, POPCNT ) ) feature.POPCNT = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.ECX, TSC_Deadline ) ) feature.TSC_Deadline = true;

  if ( check_bit( reg.ECX, AES ) ) feature.AES = true;

  if ( check_bit( reg.ECX, XSAVE ) ) feature.XSAVE = true;

  if ( check_bit( reg.ECX, OSXSAVE ) ) feature.OSXSAVE = true;

  if ( check_bit( reg.ECX, AVX ) ) feature.AVX = true;

  if ( check_bit( reg.ECX, F16C ) ) feature.F16C = true;

  if ( check_bit( reg.ECX, RDRAND3 ) ) feature.RDRAND3 = true;

  if ( check_bit( reg.EDX, FPU ) ) feature.FPU = true;
  if ( check_bit( reg.EDX, VME ) ) feature.VME = true;

  if ( check_bit( reg.EDX, DE ) ) feature.DE = true;

  if ( check_bit( reg.EDX, PSE ) ) feature.PSE = true;

  if ( check_bit( reg.EDX, TSC ) ) feature.TSC = true;

  if ( check_bit( reg.EDX, MSR ) ) feature.MSR = true;

  if ( check_bit( reg.EDX, PAE ) ) feature.PAE = true;

  if ( check_bit( reg.EDX, MCE ) ) feature.MCE = true;

  if ( check_bit( reg.EDX, CMPXCHG8B ) ) feature.CMPXCHG8B = true;

  if ( check_bit( reg.EDX, APIC ) ) feature.APIC = true;

  if ( check_bit( reg.EDX, SEP ) ) feature.SEP = true;

  if ( check_bit( reg.EDX, MTRR ) ) feature.MTRR = true;

  if ( check_bit( reg.EDX, PGE ) ) feature.PGE = true;

  if ( check_bit( reg.EDX, MCA ) ) feature.MCA = true;

  if ( check_bit( reg.EDX, CMOV ) ) feature.CMOV = true;

  if ( check_bit( reg.EDX, PAT ) ) feature.PAT = true;

  if ( check_bit( reg.EDX, PSE_36 ) ) feature.PSE_36 = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, PSN ) ) feature.PSN = true;

  if ( check_bit( reg.EDX, CLFSH ) ) feature.CLFSH = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, DS ) ) feature.DS = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, ACPI ) ) feature.ACPI = true;

  if ( check_bit( reg.EDX, MMX ) ) feature.MMX = true;

  if ( check_bit( reg.EDX, FXSR ) ) feature.FXSR = true;

  if ( check_bit( reg.EDX, SSE ) ) feature.SSE = true;

  if ( check_bit( reg.EDX, SSE2 ) ) feature.SSE2 = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, SS ) ) feature.SS = true;

  if ( check_bit( reg.EDX, HTT ) ) feature.HTT = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, TM ) ) feature.TM = true;

  if ( vendor == INTEL )
    if ( check_bit( reg.EDX, PBE ) ) feature.PBE = true;
}
}
