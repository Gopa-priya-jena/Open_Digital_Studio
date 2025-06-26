#include <Define.hpp>

#if ODS_WINDOWS
// #include <consoleapi.h>
// #include <fileapi.h>
// #include <handleapi.h>
// #include <minwinbase.h>
// #include <processthreadsapi.h>

  #include <DEBUG.hpp>
  #include <OS/OS_SHELL.hpp>
  #include <OS/WINDOWS/WINDOWS_SHELL.hpp>
  #include <chrono>
  #include <string_view>
  #include <thread>
namespace OS {

  namespace SHELL {

    OS_SHELL::OS_SHELL( OS::SHELL::SHELL_Type type )
    {
      FUNCTION_PROFILE
      // get temporray handels

      HANDLE rd_temp = NULL;
      HANDLE wr_temp = NULL;
      HANDLE er_temp = NULL;

      PROCESS_INFORMATION piProcInfo;  // Stores info about the created child process
      STARTUPINFOW siStartInfo;  // Specifies startup properties for the new process
      SECURITY_ATTRIBUTES saAttr;  // Security attributes for pipe handles

      saAttr.nLength              = sizeof( SECURITY_ATTRIBUTES );
      saAttr.bInheritHandle       = TRUE;
      saAttr.lpSecurityDescriptor = NULL;

      auto pipe_creation = [ & ]( HANDLE& RD_handle, HANDLE& WR_handle, U32 Size, SECURITY_ATTRIBUTES& saAttr, U8 num ) -> bool {
        if ( !CreatePipe( &RD_handle, &WR_handle, &saAttr, 0 ) ) {
          LOG_ERROR( "failed pipe creation" )
          return true;
        }
        LOG_NORMAL( "PIPE CREATION SUCCESFULL" )
        HANDLE object = NULL;
        if ( num == 1 ) {
          object = RD_handle;
        }
        else if ( num == 2 ) {
          object = WR_handle;
        }

        if ( !SetHandleInformation( object, HANDLE_FLAG_INHERIT, 0 ) ) {
          LOG_ERROR( "HandleInformation creation Error" );
          return true;
        }
        return false;
      };

      if ( pipe_creation( wr_temp, SHELL_windows.WRITE, OUTPUT_BUFFER_SIZE, saAttr, 2 ) ) {

        LOG_ERROR( "Write handle vcreation falied" )
      }

      if ( pipe_creation( SHELL_windows.READ, rd_temp, INPUT_BUFFER_SIZE, saAttr, 1 ) ) {

        LOG_ERROR( "read handle vcreation falied" )
      }
      if ( pipe_creation( SHELL_windows.Error, SHELL_windows.WRITE, ERROR_BUFFER_SIZE, saAttr, 1 ) ) {

        LOG_ERROR( "error handle vcreation falied" )
      }

      ZeroMemory( &piProcInfo, sizeof( PROCESS_INFORMATION ) );
      ZeroMemory( &siStartInfo, sizeof( STARTUPINFO ) );
      siStartInfo.cb         = sizeof( STARTUPINFO );
      siStartInfo.hStdInput  = wr_temp;
      siStartInfo.hStdOutput = rd_temp;
      siStartInfo.hStdError  = er_temp;
      siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

      wchar_t commandLine[] = L"powershell -NoProfile ";

      if ( !CreateProcessW( NULL, commandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo ) ) {

        CloseHandle( SHELL_windows.READ );
        CloseHandle( SHELL_windows.WRITE );
        CloseHandle( SHELL_windows.Error );
      }
      CloseHandle( rd_temp );
      CloseHandle( wr_temp );
      CloseHandle( er_temp );
    }

    OS_SHELL::~OS_SHELL()
    {

      FUNCTION_PROFILE
      CloseHandle( SHELL_windows.READ );
      CloseHandle( SHELL_windows.WRITE );
      CloseHandle( SHELL_windows.Error );
    }
    bool OS_SHELL::read_avail()
    {
      // FUNCTION_PROFILE

      U64 bytesRead = 0;
      PeekNamedPipe( SHELL_windows.READ, NULL, 0, NULL, reinterpret_cast< LPDWORD >( &bytesRead ), NULL );
      if ( bytesRead > 0 ) {
        return true;
      }
      else
        return { false };
    }
    void OS_SHELL::prepare_read()
    {

      FUNCTION_PROFILE
      while ( !read_avail() ) {
        std::this_thread ::sleep_for( std::chrono::milliseconds( 10 ) );
      }
    }
    bool OS_SHELL::Get_output()
    {
      FUNCTION_PROFILE

      if ( !read_avail() ) prepare_read();
      ReadFile( SHELL_windows.READ, Output_Buffer, OUTPUT_BUFFER_SIZE - 1, reinterpret_cast< LPDWORD >( &output_size ), NULL );
      LOG_NORMAL( "output size", output_size )

      Output_Buffer[ output_size ] = '\0';
      OUTPUT_view                  = std::string_view( Output_Buffer, output_size );

      return read_avail();
    }
    bool OS_SHELL::Input( std::string_view command )
    {

      FUNCTION_PROFILE_ARG( command )
      DWORD written  = 0;
      DWORD cmd_size = static_cast< DWORD >( command.size() );
      WriteFile( SHELL_windows.WRITE, command.data(), static_cast< DWORD >( command.size() ), &written, NULL );
      if ( written != cmd_size ) {
        LOG_ERROR( "write failed" )
        return 0;
      }
      return 1;
    }
    void OS_SHELL::flush_output()
    {

      FUNCTION_PROFILE
      char flush[ 1000 ];
      while ( read_avail() ) {

        ReadFile( SHELL_windows.READ, flush, 1000 - 1, NULL, NULL );
        flush[ 999 ] = '\0';
        LOG_NORMAL( flush )
      }
    }

    void OS_SHELL::exec_command( std::string_view cmd )
    {

      FUNCTION_PROFILE_ARG( cmd )
      if ( read_avail() ) flush_output();

      Input( cmd );
      Get_output();
    }

    std::string_view OS_SHELL::exec( std::string_view command )
    {

      FUNCTION_PROFILE_ARG( command )
      if ( read_avail() ) flush_output();
      Input( command );
      Get_output();

      flush_output();
      return OUTPUT_view;
    }
    std::string OS_SHELL::shell_read_complete()
    {
      std::string result;
      while ( Get_output() ) {
        result += OUTPUT_view;
      }
      return result;
    }

    bool OS_SHELL::shell_read_again()
    {
      Get_output();
      prepare_read();
      return read_avail();
    }

    bool OS_SHELL::shell( std::string_view command )
    {
      FUNCTION_PROFILE_ARG( command )
      flush_output();
      if ( Input( command ) ) {
        return Get_output();
      }
      else {
        ERROR_view = Write_error;
        return 0;
      }
    }
    void OS_SHELL::populate_error()
    {
      I64 size = 0;
      ReadFile( SHELL_windows.Error, error_buffer, ERROR_BUFFER_SIZE, reinterpret_cast< DWORD* >( &size ), NULL );
      error_buffer[ size ] = '\0';
      ERROR_view           = std::string_view( error_buffer, size );
    }

  }  // namespace SHELL

}  // namespace OS

#endif
