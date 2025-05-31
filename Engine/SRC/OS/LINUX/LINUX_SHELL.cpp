
#include <Define.hpp>

#if ODS_LINUX
#  include <DEBUG.hpp>
#  include <Global.hpp>
#  include <OS/LINUX/LINUX_SHELL.hpp>
#  include <OS/OS_SHELL.hpp>
#  include <cassert>
#  include <cerrno>
#  include <chrono>
#  include <csignal>
#  include <cstddef>
#  include <cstdint>
#  include <cstdlib>
#  include <fcntl.h>
#  include <stdexcept>
#  include <string_view>
#  include <sys/poll.h>
#  include <sys/wait.h>
#  include <thread>
#  include <unistd.h>
namespace OS::SHELL
{

  void Terminal( const std::string_view command, char &output, U32 output_size )
  {
    FUNCTION_PROFILE;

    assert( output_size >= 3 );
    std::unique_ptr<FILE, decltype( &pclose )> pipe(
        popen( command.data(), "r" ), pclose );
    if ( !pipe )
    {
      throw std::runtime_error( "popen() failed!" );
    }

    while ( fgets( &output, ( I32 )output_size, pipe.get() ) != nullptr )
      ;
  }

  OS_SHELL::OS_SHELL( SHELL_Type type )
  {
    FUNCTION_PROFILE;

    if ( pipe( SHELL_Linux.WRITE ) == -1 || pipe( SHELL_Linux.READ ) == -1 ||
         pipe( SHELL_Linux.ERROR ) == -1 )

    {
      LOG( "PIPES creation  failed" );
      throw std::runtime_error( "Failed to create pipes" );
    }
    else
    {
      const char *path = "bash";
      const char *argV[] = { "bash", "--norc", "--noediting", "--noprofile",
                             "-C",   "-s",     nullptr };
      posix_spawn_file_actions_t file_actions;
      posix_spawn_file_actions_init( &file_actions );

      posix_spawn_file_actions_adddup2( &file_actions, SHELL_Linux.WRITE[ 0 ],
                                        STDIN_FILENO );
      posix_spawn_file_actions_addclose( &file_actions,
                                         SHELL_Linux.WRITE[ 1 ] );

      posix_spawn_file_actions_adddup2( &file_actions, SHELL_Linux.READ[ 1 ],
                                        STDOUT_FILENO );
      posix_spawn_file_actions_addclose( &file_actions, SHELL_Linux.READ[ 0 ] );

      posix_spawn_file_actions_adddup2( &file_actions, SHELL_Linux.ERROR[ 1 ],
                                        STDERR_FILENO );
      posix_spawn_file_actions_addclose( &file_actions,
                                         SHELL_Linux.ERROR[ 0 ] );

      I32 result = fcntl( SHELL_Linux.WRITE[ 0 ], F_SETPIPE_SZ, 1024 );
      if ( result == -1 )
        ALERT( "fcntl F_SETPIPE_SZ WRITE" );

      result =
          fcntl( SHELL_Linux.READ[ 0 ], F_SETPIPE_SZ, OUTPUT_BUFFER_SIZE * 10 );
      if ( result == -1 )
        ALERT( "fcntl F_SETPIPE_SZ READ" );
      result = fcntl( SHELL_Linux.ERROR[ 0 ], F_SETPIPE_SZ, 1024 );
      if ( result == -1 )
        ALERT( "fcntl F_SETPIPE_SZ error" );
      posix_spawnattr_t attributes;
      result = posix_spawnattr_init( &attributes );
      if ( result != 0 )
      {
        throw std::runtime_error( "Failed to creation of attributes" );
        // Handle the error - maybe the system couldn't give you the box.
      }

      sigset_t default_signals;
      sigemptyset( &default_signals );
      sigaddset( &default_signals, SIGCHLD );

      result = posix_spawnattr_setsigdefault( &attributes, &default_signals );
      if ( result != 0 )
      {
        throw std::runtime_error( "posix_spawnattr_setsigdefault failed" );
      }
      pid_t pgroup_id =
          getpgrp(); // Let's say we want it in the same group as the parent
      result = posix_spawnattr_setpgroup( &attributes, pgroup_id );
      if ( result != 0 )
      {
        std::runtime_error( "posix_spawnattr_setpgroup failed" );
      }

      if ( posix_spawnp( &SHELL_Linux.processID, path, &file_actions,
                         &attributes,
                         static_cast<char **>( const_cast<char **>( argV ) ),
                         environ ) == 0 )
      {
        LOG( "SHELLL ACTIVE" )
        result = posix_spawnattr_destroy( &attributes );
        if ( result != 0 )
        {
          perror( "posix_spawnattr_destroy failed" );
        }
        close( SHELL_Linux.WRITE[ 0 ] );
        close( SHELL_Linux.READ[ 1 ] );
        close( SHELL_Linux.ERROR[ 1 ] );

        // prepare th poll data structure

        SHELL_Linux.Eventpoll[ 0 ].fd = SHELL_Linux.READ[ 0 ];
        SHELL_Linux.Eventpoll[ 1 ].fd = SHELL_Linux.WRITE[ 1 ];
        SHELL_Linux.Eventpoll[ 2 ].fd = SHELL_Linux.ERROR[ 0 ];

        // events setting of Read

        SHELL_Linux.Eventpoll[ 0 ].events = POLLIN;
        SHELL_Linux.Eventpoll[ 0 ].revents = 0;

        // events setting of Write
        SHELL_Linux.Eventpoll[ 1 ].events = POLLOUT;
        SHELL_Linux.Eventpoll[ 1 ].revents = 0;

        // events setting of ERROR
        SHELL_Linux.Eventpoll[ 2 ].events = POLLIN;
        SHELL_Linux.Eventpoll[ 2 ].revents = 0;

        if ( type == SHELL_Type::ASYNC )
        {
          Thread_init();
        }
      }

      posix_spawn_file_actions_destroy( &file_actions );
    }
  }
  void OS_SHELL::check_error()
  {
    FUNCTION_PROFILE;

    char    buffer[ 1024 ];
    ssize_t bytesRead;
    OS_console_out( "ERROR in shell: ", RED, BLACK, Bold );

    while ( ( bytesRead = read( SHELL_Linux.ERROR[ 0 ], buffer,
                                sizeof( buffer ) - 1 ) ) > 0 )
    {
      buffer[ bytesRead ] = '\0';
      OS::TERMINAL::OS_console_out( buffer, RED, BLACK, Bold );
    }
  }
  void OS_SHELL::update_Eventpoll()
  {
    // FUNCTION_PROFILE;
  start_again:
    auto result = poll( SHELL_Linux.Eventpoll, 3, 0 );
    if ( result == -1 || errno == EAGAIN )
      goto start_again;
  }

  void OS_SHELL::Thread_init()
  {
    FUNCTION_PROFILE;

    // std::thread read(output());
  }
  OS_SHELL::~OS_SHELL()

  {
    FUNCTION_PROFILE;

    Send_command( "exit\n" );
    I32 status = 0;
    waitpid( SHELL_Linux.processID, &status, 0 );
    close( SHELL_Linux.WRITE[ 1 ] );
    close( SHELL_Linux.READ[ 0 ] );
    close( SHELL_Linux.ERROR[ 0 ] );
    // Thread_close();
    //
  }
  bool OS_SHELL::Get_output()
  {
    FUNCTION_PROFILE;

    while ( !Read_ready() )
      std::this_thread::sleep_for( std::chrono::nanoseconds( 10 ) );

    {
      output_size = read( SHELL_Linux.READ[ 0 ], &Output_Buffer[ 0 ],
                          OUTPUT_BUFFER_SIZE - 1 );
      Output_Buffer[ output_size ] = '\0';
      OUTPUT = std::string_view( Output_Buffer, output_size );
    }
    LOG( OUTPUT )
    return ( Read_ready() );
  }
  bool OS_SHELL::Send_command( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command );

    while ( !Write_ready() )
      std::this_thread::sleep_for( std::chrono::nanoseconds( 10 ) );

    if ( command.back() == '\n' )
    {
      auto res =
          write( SHELL_Linux.WRITE[ 1 ], command.data(), command.size() );

      return res == command.size();
    }
    else
    {
      auto last_pos = command.copy( command_buffer, 1023 );
      command_buffer[ last_pos ] = '\0';
      auto res = write( SHELL_Linux.WRITE[ 1 ], command_buffer, last_pos + 1 );
      return res == last_pos + 1;
    }
  }

  bool OS_SHELL::shell( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command )

    FUNCTION_PROFILE_ARG( command )

    if ( Send_command( command ) )
    {
      return Get_output();
    }
    else
    {
      ERROR = Write_error;
      return 0;
    }
  }

  std::string OS_SHELL::shell_read_complete()
  {
    FUNCTION_PROFILE;

    std::string result;

    if ( Get_output() )
    {
      result += OUTPUT.substr( 0, output_size );
    }
    return result;
  }

  void OS_SHELL::flush_output()
  {

    FUNCTION_PROFILE;
    if ( Read_ready() )
    {
      LOG( "ok got pollin " )
      char    flush_buff[ 1024 ];
      ssize_t val = 1;
      while ( val && Read_ready() )
      {

        // while (!Read_ready()) {
        //   LOG("sleeping in ", __FUNCTION__, "VAL ", val)
        //   std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        // }
        val = read( SHELL_Linux.READ[ 0 ], flush_buff, 1023 );
        flush_buff[ 1023 ] = '\0';

        LOG( flush_buff )
        LOG( "\n val \t", val )
      }
    }
  }
  std::string_view OS_SHELL::exec( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command )

    if ( Read_ready() )
    {
      flush_output();
    }

    if ( shell( command ) )
    {
      ALERT( ERROR );
      flush_output();
    }
    return OUTPUT;
  }
  bool OS_SHELL::Read_ready()
  {

    update_Eventpoll();
    return ( SHELL_Linux.Eventpoll[ 0 ].revents & POLLIN );
  }
  bool OS_SHELL::Write_ready()
  {

    update_Eventpoll();
    return ( SHELL_Linux.Eventpoll[ 1 ].revents & POLLOUT );
  }

} // namespace OS::SHELL
#endif
