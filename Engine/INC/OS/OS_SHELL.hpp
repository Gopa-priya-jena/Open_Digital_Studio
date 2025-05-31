#pragma once
#include <Define.hpp>

#include <OS/LINUX/LINUX_SHELL.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <thread>
#define PLATFORM_SHELL_VARIABLE LINUX_SHELL_VARIABLE
#define PLATFORM_SHELL_PROCESS  LINUX_SHELL_PROCESS

// TODO:: ASYNC IS NOT PRIORITY BUT WILL BE IMPLEMENTED IN FUTURE

namespace OS
{
  namespace SHELL
  {

    enum SHELL_Type
    {
      ASYNC,
      BLOCKING
    };

    // is unsafe and slow and may be unable to do d its job for  long task
    void               Terminal( const std::string_view command, char &output,
                                 U32 output_size );
    constexpr uint32_t OUTPUT_BUFFER_SIZE = 1024 * 10;

    // use this faster and better for completing task
    // TODO:: conplete asyn part

    class OS_SHELL
    {
      char Output_Buffer[ OUTPUT_BUFFER_SIZE ];
      char command_buffer[ 1024 ];
      char error_buffer[ 1024 ];

      std::size_t output_size{ 0 };

      // std::thread READ, WRITE, ERROR;
      PLATFORM_SHELL_VARIABLE

      void             Thread_init();
      void             Thread_close();
      bool             Get_output();
      bool             Send_command( std::string_view command );
      void             flush_output();
      std::string_view Write_error = "Write failed", read_error = "READ_failed";

    public:
      std::string_view ERROR;
      std::string_view OUTPUT;
      OS_SHELL() = delete;
      OS_SHELL( OS_SHELL & ) = delete;

      OS_SHELL( SHELL_Type type );
      ~OS_SHELL();

      void             exec_command( std::string_view );
      bool             shell( std::string_view command );
      std::string_view exec( std::string_view command );
      bool             shell_read_again();
      std::string      shell_read_complete();
      PLATFORM_SHELL_PROCESS
    };

  } // namespace SHELL

} // namespace OS
