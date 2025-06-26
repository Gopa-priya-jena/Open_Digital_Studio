#include <Define.hpp>
#include <cstring>
#include <string>
#include <string_view>

#include "DEBUG.hpp"
#if ODS_LINUX
  #include <ALGORITHMS/ALGORITHMS.hpp>
  #include <OS/LINUX/LINUX_INFO.hpp>
  #include <OS/OS_INFO.hpp>
  #include <OS/OS_SHELL.hpp>
namespace OS {

  namespace INFO

  {

    void collect_memory_resources( OS::SHELL::OS_SHELL &shell, OS_mem_info &mem_info )
    {
      FUNCTION_PROFILE;
      LOG( "starting memory collection" )
      using namespace ALGO::STRING;

      std::string_view command_result = shell.exec( "cat /proc/meminfo \n" );

      mem_info.physical_memory      = ( String_To_Bytes( ( get_line_with( "MemTotal:", command_result ).substr( std::strlen( "MemTotal:" ) ) ) ) );
      mem_info.free_physical_memory = String_To_Bytes( ( get_line_with( "MemFree: ", command_result ).substr( std::strlen( "MemFree: " ) ) ) );

      mem_info.page_size      = sysconf( _SC_PAGESIZE );
      mem_info.Swap           = String_To_Bytes( ( get_line_with( "SwapTotal:", command_result ).substr( std::strlen( "SwapTotal:" ) ) ) );
      mem_info.Swap_free      = String_To_Bytes( ( get_line_with( "SwapFree:", command_result ).substr( std::strlen( "SwapFree" ) ) ) );
      mem_info.Swap_busy      = mem_info.Swap - mem_info.Swap_free;
      mem_info.Huge_page_size = String_To_Bytes( ( get_line_with( "Hugepagesize", command_result ).substr( std::strlen( "Hugepagesize" ) ) ) );

      mem_info.HugePages_Total = atoi( get_line_with( "HugePages_Total:", command_result ).substr( std::strlen( "HugePages_Total:" ) ).data() );

      mem_info.HugePages_Free = atoi( ( get_line_with( "HugePages_Free:", command_result ) ).substr( std::strlen( "HugePages_Free:" ) ).data() );

      mem_info.DirectMap4k = String_To_Bytes( ( get_line_with( "DirectMap4k:", command_result ).substr( 0, strlen( "DirectMap4k:" ) ) ) );
      mem_info.DirectMap2M = String_To_Bytes( ( get_line_with( "DirectMap2M:", command_result ).substr( 0, strlen( "DirectMap2M:" ) ) ) );
      mem_info.DirectMap1G = String_To_Bytes( ( get_line_with( "DirectMap1G:", command_result ).substr( 0, strlen( "DirectMap1G:" ) ) ) );

      command_result = shell.exec( "cat /proc/sys/vm/overcommit_memory \n" );

      if ( command_result[ 0 ] == '0' || command_result[ 0 ] == '1' )
        mem_info.overcommit = true;
      else
        mem_info.overcommit = false;
    }
    void collect_cpu_info( OS::SHELL::OS_SHELL &shell, OS_cpu_info &cpu_info )
    {
      FUNCTION_PROFILE;

      using namespace ALGO::STRING;

      std::string_view command_result;

      command_result = shell.exec( "cat /proc/cpuinfo  \n" );

      command_result = shell.exec( "cat /proc/cpuinfo |grep 'cpu cores' \n" );
      cpu_info.cores = getlast_number( get_line_with( "cpu cores	", command_result ), ':' );

      command_result           = shell.exec( "cat /proc/cpuinfo |grep 'siblings' \n" );
      cpu_info.thread_per_core = getlast_number( get_line_with( "siblings	", command_result ), ':' ) / cpu_info.cores;

      command_result       = shell.exec( "cat /proc/cpuinfo |grep 'cpu MHz' \n" );
      cpu_info.cpu_Max_MHz = getlast_numberf( get_line_with( "cpu MHz		", command_result ), ':' );

      command_result     = shell.exec( "cat /proc/cpuinfo |grep 'TLB size' \n" );
      cpu_info.TLB_count = getlast_number( get_line_with( "TLB size", command_result ), ':' );

      command_result                 = shell.exec( "cat /proc/cpuinfo |grep 'address sizes' \n" );
      cpu_info.Physical_Address_Bits = getlast_number( get_line_with( "address sizes	", command_result ), ':' );
      cpu_info.Virtual_Address_Bits  = getlast_number( get_line_with( "address sizes	", command_result ), ',' );
      LOG( "entering one Size" )
      command_result   = shell.exec( "lscpu -C=NAME,ONE-SIZE \n" );
      cpu_info.L1d_mem = String_To_Bytes( get_line_with( "L1d", command_result ).substr( std::strlen( "L1d" ) ) );
      cpu_info.L1i_mem = String_To_Bytes( get_line_with( "L1i", command_result ).substr( std::strlen( "L1i" ) ) );
      cpu_info.L2_mem  = String_To_Bytes( get_line_with( "L2", command_result ).substr( std::strlen( "L2" ) ) );
      cpu_info.L3_mem  = String_To_Bytes( get_line_with( "L3", command_result ).substr( std::strlen( "L3" ) ) );

      LOG( "entering all Size" )
      command_result         = shell.exec( "lscpu -C=NAME,ALL-SIZE \n" );
      cpu_info.L1d_mem_total = String_To_Bytes( get_line_with( "L1d", command_result ).substr( std::strlen( "L1d" ) ) );
      cpu_info.L1i_mem_total = String_To_Bytes( get_line_with( "L1i", command_result ).substr( std::strlen( "L1i" ) ) );
      cpu_info.L2_mem_total  = String_To_Bytes( get_line_with( "L2", command_result ).substr( std::strlen( "L2" ) ) );
      cpu_info.L3_mem_total  = String_To_Bytes( get_line_with( "L3", command_result ).substr( std::strlen( "L3" ) ) );

      command_result = shell.exec( "lscpu -C=NAME,WAYS \n" );

      cpu_info.L1i_assoc = getlast_number( get_line_with( "L1d", command_result ).substr( std::strlen( "L1d" ) ) );
      cpu_info.L1d_assoc = getlast_number( get_line_with( "L1i", command_result ).substr( std::strlen( "L1i" ) ) );
      cpu_info.L2_assoc  = getlast_number( get_line_with( "L2", command_result ).substr( std::strlen( "L2" ) ) );
      cpu_info.L3_assoc  = getlast_number( get_line_with( "L3", command_result ).substr( std::strlen( "L3" ) ) );

      command_result = shell.exec( "lscpu -C=NAME,COHERENCY-SIZE\n" );

      cpu_info.L1i_LineSize = getlast_number( get_line_with( "L1d", command_result ).substr( std::strlen( "L1d" ) ) );
      cpu_info.L1d_LineSize = getlast_number( get_line_with( "L1i", command_result ).substr( std::strlen( "L1i" ) ) );
      cpu_info.L2_LineSize  = getlast_number( get_line_with( "L2", command_result ).substr( std::strlen( "L2" ) ) );
      cpu_info.L3_LineSize  = getlast_number( get_line_with( "L3", command_result ).substr( std::strlen( "L3" ) ) );

      command_result = shell.exec( "lscpu -C=NAME,SETS\n" );

      cpu_info.L1i_setSize = getlast_number( get_line_with( "L1d", command_result ).substr( std::strlen( "L1d" ) ) );
      cpu_info.L1d_setSize = getlast_number( get_line_with( "L1i", command_result ).substr( std::strlen( "L1i" ) ) );
      cpu_info.L2_setSize  = getlast_number( get_line_with( "L2", command_result ).substr( std::strlen( "L2" ) ) );
      cpu_info.L3_setSize  = getlast_number( get_line_with( "L3", command_result ).substr( std::strlen( "L3" ) ) );
    }

    void print_info( OS_complete_info info )
    {
      FUNCTION_PROFILE;

      std::stringstream ss;

      // cpuinfo
      ss << " cores \t" << info.cpu_info.cores << std::endl;
      ss << " thread_per_core\t" << info.cpu_info.thread_per_core << std::endl;
      ss << " L1i_mem\t" << info.cpu_info.L1i_mem << std::endl;
      ss << " L1d_mem\t" << info.cpu_info.L1d_mem << std::endl;
      ss << " L2_mem\t" << info.cpu_info.L2_mem << std::endl;
      ss << " L3_mem\t" << info.cpu_info.L3_mem << std::endl;
      ss << " L1i_mem total\t" << info.cpu_info.L1i_mem_total << std::endl;
      ss << " L1d_mem total \t" << info.cpu_info.L1d_mem_total << std::endl;
      ss << " L2_mem total \t" << info.cpu_info.L2_mem_total << std::endl;
      ss << " L3_mem total \t" << info.cpu_info.L3_mem_total << std::endl;

      ss << " L1d_assoc, L1d_LineSize, L1d_setSize\t" << info.cpu_info.L1d_assoc << "\t" << info.cpu_info.L1d_LineSize << "\t" << info.cpu_info.L1d_setSize << std::endl;
      ss << " L1i_assoc, L1i_LineSize, L1i_setSize\t" << info.cpu_info.L1i_assoc << "\t" << info.cpu_info.L1i_LineSize << "\t" << info.cpu_info.L1i_setSize << std::endl;
      ss << " L2_assoc, L2_LineSize, L2_setSize\t" << info.cpu_info.L2_assoc << "\t" << info.cpu_info.L2_LineSize << "\t" << info.cpu_info.L2_setSize << std::endl;
      ss << " L3_assoc, L3_LineSize, L3_setSize\t" << info.cpu_info.L3_assoc << "\t" << info.cpu_info.L3_LineSize << "\t" << info.cpu_info.L3_setSize << std::endl;
      ss << " Physical_Address_Bits\t" << info.cpu_info.Physical_Address_Bits << std::endl;
      ss << " Virtual_Address_Bits\t" << info.cpu_info.Virtual_Address_Bits << std::endl;
      ss << " TLB_size\t" << info.cpu_info.TLB_count << std::endl;
      ss << " cpu_MHz\t " << info.cpu_info.cpu_Max_MHz << std::endl;
      // memoryin

      ss << "physical_memoy\t" << info.mem_info.physical_memory << std::endl;
      ss << "free_physical_memoy\t" << info.mem_info.free_physical_memory << std::endl;
      ss << "Swap\t" << info.mem_info.Swap << std::endl;
      ss << "Swap_buy\t" << info.mem_info.Swap_busy << std::endl;
      ss << "Swap_free\t" << info.mem_info.Swap_free << std::endl;
      ss << "page_sie\t" << info.mem_info.page_size << std::endl;
      ss << "Huge_page_sie\t" << info.mem_info.Huge_page_size << std::endl;
      ss << "HugePages_Total\t" << info.mem_info.HugePages_Total << std::endl;
      ss << "HugePages_Free\t" << info.mem_info.HugePages_Free << std::endl;
      ss << "DirectMap1k\t" << info.mem_info.DirectMap4k << std::endl;
      ss << "DirectMap2M\t" << info.mem_info.DirectMap2M << std::endl;
      ss << "DirectMap1G\t" << info.mem_info.DirectMap1G << std::endl;
      ss << "overcommt\t" << info.mem_info.overcommit << std::endl;

      LOG( ss.str() );
    }

    OS_complete_info GET_all_HW_info()
    {
      FUNCTION_PROFILE;

      OS_complete_info info;
      SHELL::OS_SHELL shell( SHELL::SHELL_Type::BLOCKING );
      collect_cpu_info( shell, info.cpu_info );
      collect_memory_resources( shell, info.mem_info );
      print_info( info );
      return info;
    }
    // void cpu_info( cpu_info &info ) {}
  }  // namespace INFO

}  // namespace OS

#endif
