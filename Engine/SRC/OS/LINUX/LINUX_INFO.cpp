#include <Define.hpp>
#include <cstring>
#include <string>
#include <string_view>

#include "ALGORITHMS/strings.hpp"
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

      shell.shell( "cat /proc/meminfo \n" );

      Token_iterator TK( shell.OUTPUT_view, ":\n" );

      mem_info.physical_memory      = Str_to_mem_size( TK.next_token( "MemTotal" ) );
      mem_info.free_physical_memory = Str_to_mem_size( TK.next_token( "MemFree" ) );
      mem_info.Swap                 = Str_to_mem_size( TK.next_token( "SwapTotal" ) );
      mem_info.Swap_free            = Str_to_mem_size( TK.next_token( "SwapFree" ) );
      mem_info.Huge_page_size       = Str_to_mem_size( TK.next_token( "Hugepagesize" ) );
      mem_info.HugePages_Total      = Str_to_mem_size( TK.next_token( "HugePages_Total" ) );
      mem_info.HugePages_Free       = Str_to_mem_size( TK.next_token( "HugePages_Free" ) );
      mem_info.DirectMap4k          = Str_to_mem_size( TK.next_token( "DirectMap4k" ) );
      mem_info.DirectMap2M          = Str_to_mem_size( TK.next_token( "DirectMap2M" ) );
      mem_info.DirectMap1G          = Str_to_mem_size( TK.next_token( "DirectMap1G" ) );
      mem_info.page_size            = sysconf( _SC_PAGESIZE );
      mem_info.Swap_busy            = mem_info.Swap - mem_info.Swap_free;
      shell.exec( "cat /proc/sys/vm/overcommit_memory \n" );

      if ( shell.OUTPUT_view[ 0 ] == '0' || shell.OUTPUT_view[ 0 ] == '1' )
        mem_info.overcommit = true;
      else
        mem_info.overcommit = false;

      shell.exec( "ls /sys/kernel/mm/hugepages \n" );
      TK = Token_iterator( shell.OUTPUT_view, "-\n" );

      U64 page_size = Str_to_mem_size( TK.next_token( "hugepages" ) );
      while ( !TK.end() ) {
        TK.next_token();
        U64 temp = Str_to_mem_size( TK.next_token() );

        if ( temp > page_size ) page_size = temp;
      }

      mem_info.MAX_Huge_page_size = page_size;
    }
    void collect_cpu_info( OS::SHELL::OS_SHELL &shell, OS_cpu_info &info )
    {
      FUNCTION_PROFILE;

      using namespace ALGO::STRING;

      shell.shell( "lscpu\n" );
      Token_iterator it( shell.OUTPUT_view, " \n" );
      auto vendor = it.next_token( "Vendor ID:" );

      info.Physical_Address_Bits = Str_to_uint( it.next_token_p( "Address sizes:" ) );
      info.Virtual_Address_Bits  = Str_to_uint( it.next_token( "physical," ) );

      vendor.copy( info.VendorId, vendor.size() );

      info.thread_per_core = Str_to_uint( it.next_token_p( "Thread(s) per core:" ) );
      info.cores           = Str_to_uint( it.next_token_p( "Core(s) per socket:" ) );

      info.cpu_Max_MHz = Str_to_F32( it.next_token_p( "max MHz:" ) );
      info.cpu_Mix_MHz = Str_to_F32( it.next_token_p( "min MHz:" ) );

      info.Numa = Str_to_uint( it.next_token_p( "std::string_view view" ) );
      shell.shell( "lscpu -C=NAME,ONE-SIZE,ALL-SIZE,WAYS,SETS,COHERENCY-SIZE\n" );

      it                 = Token_iterator( shell.OUTPUT_view, " \n" );
      info.L1d_mem       = Str_to_mem_size( it.next_token( "L1d" ) );
      info.L1d_mem_total = Str_to_mem_size( it.next_token() );
      info.L1d_ways      = Str_to_uint( it.next_token() );
      info.L1d_setSize   = Str_to_uint( it.next_token() );
      info.L1d_LineSize  = Str_to_uint( it.next_token() );
      // l1i
      info.L1i_mem       = Str_to_mem_size( it.next_token( "L1i" ) );
      info.L1i_mem_total = Str_to_mem_size( it.next_token() );
      info.L1i_ways      = Str_to_uint( it.next_token() );
      info.L1i_setSize   = Str_to_uint( it.next_token() );
      info.L1i_LineSize  = Str_to_uint( it.next_token() );

      // l2
      info.L2_mem       = Str_to_mem_size( it.next_token( "L2" ) );
      info.L2_mem_total = Str_to_mem_size( it.next_token() );
      info.L2_ways      = Str_to_uint( it.next_token() );
      info.L2_setSize   = Str_to_uint( it.next_token() );
      info.L2_LineSize  = Str_to_uint( it.next_token() );
      // l3
      info.L3_mem       = Str_to_mem_size( it.next_token( "L3" ) );
      info.L3_mem_total = Str_to_mem_size( it.next_token() );
      info.L3_ways      = Str_to_uint( it.next_token() );
      info.L3_setSize   = Str_to_uint( it.next_token() );
      info.L3_LineSize  = Str_to_uint( it.next_token() );
    }
    // add this to test remove from here its not usefull only for debugging
    void print_info( OS_complete_info info )
    {
      FUNCTION_PROFILE;

      std::stringstream ss;

      // cpuinfo
      ss << " cores \t" << info.cpu_info.cores << std::endl;
      ss << " thread_per_core\t" << ( U64 )info.cpu_info.thread_per_core << std::endl;
      ss << " L1i_mem\t" << info.cpu_info.L1i_mem << std::endl;
      ss << " L1d_mem\t" << info.cpu_info.L1d_mem << std::endl;
      ss << " L2_mem\t" << info.cpu_info.L2_mem << std::endl;
      ss << " L3_mem\t" << info.cpu_info.L3_mem << std::endl;
      ss << " L1i_mem total\t" << ( U64 )info.cpu_info.L1i_mem_total << std::endl;
      ss << " L1d_mem total \t" << ( U64 )info.cpu_info.L1d_mem_total << std::endl;
      ss << " L2_mem total \t" << ( U64 )info.cpu_info.L2_mem_total << std::endl;
      ss << " L3_mem total \t" << ( U64 )info.cpu_info.L3_mem_total << std::endl;

      ss << " L1d_assoc, L1d_LineSize, L1d_setSize\t" << ( U64 )info.cpu_info.L1d_ways << "\t" << ( U64 )info.cpu_info.L1d_LineSize << "\t" << ( U64 )info.cpu_info.L1d_setSize << std::endl;
      ss << " L1i_assoc, L1i_LineSize, L1i_setSize\t" << ( U64 )info.cpu_info.L1i_ways << "\t" << ( U64 )info.cpu_info.L1i_LineSize << "\t" << ( U64 )info.cpu_info.L1i_setSize << std::endl;
      ss << " L2_assoc, L2_LineSize, L2_setSize\t" << ( U64 )info.cpu_info.L2_ways << "\t" << ( U64 )info.cpu_info.L2_LineSize << "\t" << ( U64 )info.cpu_info.L2_setSize << std::endl;
      ss << " L3_assoc, L3_LineSize, L3_setSize\t" << ( U64 )info.cpu_info.L3_ways << "\t" << ( U64 )info.cpu_info.L3_LineSize << "\t" << ( U64 )info.cpu_info.L3_setSize << std::endl;
      ss << " Physical_Address_Bits\t" << ( U64 )info.cpu_info.Physical_Address_Bits << std::endl;
      ss << " Virtual_Address_Bits\t" << ( U64 )info.cpu_info.Virtual_Address_Bits << std::endl;
      ss << " cpu_MHz\t " << ( U64 )info.cpu_info.cpu_Max_MHz << std::endl;
      // memoryin

      ss << "physical_memoy\t" << info.mem_info.physical_memory << std::endl;
      ss << "free_physical_memoy\t" << info.mem_info.free_physical_memory << std::endl;
      ss << "Swap\t" << info.mem_info.Swap << std::endl;
      ss << "Swap_buy\t" << info.mem_info.Swap_busy << std::endl;
      ss << "Swap_free\t" << info.mem_info.Swap_free << std::endl;
      ss << "page_sie\t" << info.mem_info.page_size << std::endl;
      ss << "Huge_page_sie\t" << info.mem_info.Huge_page_size << std::endl;
      ss << "HugePages_Total\t" << info.mem_info.HugePages_Total << std::endl;
      ss << "MAX HugePages \t" << info.mem_info.MAX_Huge_page_size << std::endl;
      ss << "HugePages_Free\t" << info.mem_info.HugePages_Free << std::endl;
      ss << "DirectMap1k\t" << info.mem_info.DirectMap4k << std::endl;
      ss << "DirectMap2M\t" << info.mem_info.DirectMap2M << std::endl;
      ss << "DirectMap1G\t" << info.mem_info.DirectMap1G << std::endl;
      ss << "overcommt\t" << info.mem_info.overcommit << std::endl;

      LOG_CRITICAL( ss.str() );
    }

    OS_complete_info GET_all_HW_info()
    {
      FUNCTION_PROFILE;

      OS_complete_info info;
      SHELL::OS_SHELL shell( SHELL::SHELL_Type::BLOCKING );
      collect_cpu_info( shell, info.cpu_info );
      collect_memory_resources( shell, info.mem_info );
      // print_info( info );
      return info;
    }
    // void cpu_info( cpu_info &info ) {}
  }  // namespace INFO

}  // namespace OS

#endif
