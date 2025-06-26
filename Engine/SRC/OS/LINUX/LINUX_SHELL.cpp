
#include <Define.hpp>
#include <Types.hpp>
#include <chrono>

#if ODS_LINUX
  #include <fcntl.h>
  #include <sys/poll.h>
  #include <sys/wait.h>
  #include <unistd.h>

  #include <DEBUG.hpp>
  #include <Global.hpp>
  #include <OS/LINUX/LINUX_SHELL.hpp>
  #include <OS/OS_SHELL.hpp>
  #include <cassert>
  #include <cerrno>
  #include <cstddef>
  #include <cstdlib>
  #include <cstring>
  #include <stdexcept>
  #include <string_view>
  #include <thread>
namespace OS::SHELL {

  void Terminal( const std::string_view command, char &output, U32 output_size )
  {
    // FUNCTION_PROFILE;

    assert( output_size >= 3 );
    std::unique_ptr< FILE, decltype( &pclose ) > pipe( popen( command.data(), "r" ), pclose );
    if ( !pipe ) {
      throw std::runtime_error( "popen() failed!" );
    }

    while ( fgets( &output, ( I32 )output_size, pipe.get() ) != nullptr )
      ;
  }

  OS_SHELL::OS_SHELL( SHELL_Type type )
  {
    // FUNCTION_PROFILE;
    I32 READ_temp[ 2 ];
    I32 WRITE_temp[ 2 ];  // stdin for bash / other shell
    I32 ERROR_temp[ 2 ];
    if ( pipe( WRITE_temp ) == -1 || pipe( READ_temp ) == -1 || pipe( ERROR_temp ) == -1 )

    {
      LOG( "PIPES creation  failed" );
      throw std::runtime_error( "Failed to create pipes" );
    }
    else {
      const char *path = "bash";
      // const char *argV[] = { "/usr/bin/bash", NULL };
      const char *argV[] = { "bash",
                             "--norc",  // Don't read ~/.bashrc
                             "--noprofile",  // Don't read profile files
                             // "-i",  // Force interactive mode
                             // "--posix",  // posix
                             "--noediting", NULL };

      posix_spawn_file_actions_t file_actions;
      posix_spawn_file_actions_init( &file_actions );

      posix_spawn_file_actions_adddup2( &file_actions, WRITE_temp[ 0 ], STDIN_FILENO );
      posix_spawn_file_actions_addclose( &file_actions, WRITE_temp[ 1 ] );

      posix_spawn_file_actions_adddup2( &file_actions, READ_temp[ 1 ], STDOUT_FILENO );
      posix_spawn_file_actions_addclose( &file_actions, READ_temp[ 0 ] );

      posix_spawn_file_actions_adddup2( &file_actions, ERROR_temp[ 1 ], STDERR_FILENO );
      posix_spawn_file_actions_addclose( &file_actions, ERROR_temp[ 0 ] );

      // buffer managment
      I32 result = fcntl( WRITE_temp[ 1 ], F_SETPIPE_SZ, INPUT_BUFFER_SIZE );
      if ( result == -1 ) ALERT( "fcntl F_SETPIPE_SZ WRITE" );
      result = fcntl( READ_temp[ 0 ], F_SETPIPE_SZ, OUTPUT_BUFFER_SIZE );
      if ( result == -1 ) ALERT( "fcntl F_SETPIPE_SZ READ" );
      result = fcntl( ERROR_temp[ 0 ], F_SETPIPE_SZ, ERROR_BUFFER_SIZE );
      if ( result == -1 ) ALERT( "fcntl F_SETPIPE_SZ error" );

      auto nonblocking = [ & ]( I32 fd ) {
        auto result = fcntl( fd, F_SETFL, fcntl( fd, F_GETFL ) | O_NONBLOCK );

        if ( result == -1 ) {
          ALERT( "fcntl F_SETPIPE_SZ error" );
          LOG_ERROR( "failed nonblocking io" );
        }
      };
      nonblocking( WRITE_temp[ 1 ] );
      nonblocking( READ_temp[ 0 ] );
      nonblocking( ERROR_temp[ 0 ] );

      posix_spawnattr_t attributes;
      result = posix_spawnattr_init( &attributes );
      if ( result != 0 ) {
        throw std::runtime_error( "Failed to creation of attributes" );
        // Handle the error - maybe the system couldn't give you the box.
      }
      result = posix_spawnattr_setflags( &attributes, POSIX_SPAWN_SETSIGDEF );
      if ( result != 0 ) {
        std::cerr << "Failed to set spawn flags: " << std::endl;
      }
      sigset_t default_signals;
      sigemptyset( &default_signals );
      sigaddset( &default_signals, SIGCHLD );

      result = posix_spawnattr_setsigdefault( &attributes, &default_signals );
      if ( result != 0 ) {
        LOG_CRITICAL( "posix_spawnattr_setsigdefault failed" )
        throw std::runtime_error( "posix_spawnattr_setsigdefault failed" );
      }
      pid_t pgroup_id = getpgrp();  // Let's say we want it in the same group as the parent
      result          = posix_spawnattr_setpgroup( &attributes, pgroup_id );
      if ( result != 0 ) {
        LOG_CRITICAL( "posix_spawnattr_setpgroup failed" )
        std::runtime_error( "posix_spawnattr_setpgroup failed" );
      }

      if ( posix_spawnp( &SHELL_Linux.processID, path, &file_actions, &attributes, static_cast< char ** >( const_cast< char ** >( argV ) ), environ ) == 0 ) {
        LOG( "SHELLL ACTIVE" )
        // clossing file
        close( WRITE_temp[ 0 ] );
        close( READ_temp[ 1 ] );
        close( ERROR_temp[ 1 ] );

        SHELL_Linux.READ  = READ_temp[ 0 ];
        SHELL_Linux.WRITE = WRITE_temp[ 1 ];
        SHELL_Linux.ERROR = ERROR_temp[ 0 ];

        // signal( SIGPIPE, SIG_IGN );
        result = posix_spawnattr_destroy( &attributes );
        if ( result != 0 ) {
          perror( "posix_spawnattr_destroy failed" );
        }
        // int wstatus;
        // if ( waitpid( SHELL_Linux.processID, &wstatus, 0 ) == -1 ) {
        //   std::cerr << "waitpid failed: " << std::endl;
        //   // prepare th poll data structure
        // }
        SHELL_Linux.Eventpoll[ 0 ].fd = SHELL_Linux.READ;
        SHELL_Linux.Eventpoll[ 1 ].fd = SHELL_Linux.WRITE;
        SHELL_Linux.Eventpoll[ 2 ].fd = SHELL_Linux.ERROR;

        // events setting of Read

        SHELL_Linux.Eventpoll[ 0 ].events  = POLLIN;
        SHELL_Linux.Eventpoll[ 0 ].revents = 0;

        // events setting of Write
        SHELL_Linux.Eventpoll[ 1 ].events  = POLLOUT;
        SHELL_Linux.Eventpoll[ 1 ].revents = 0;

        // events setting of ERROR
        SHELL_Linux.Eventpoll[ 2 ].events  = POLLIN;
        SHELL_Linux.Eventpoll[ 2 ].revents = 0;

        if ( type == SHELL_Type::ASYNC ) {
          Thread_init();
        }
      }

      posix_spawn_file_actions_destroy( &file_actions );
    }
  }

  void OS_SHELL::check_error()
  {
    FUNCTION_PROFILE;

    char buffer[ 1024 ];
    ssize_t bytesRead;
    OS_console_out( "\nERROR in shell: ", RED, BLACK, Bold );

    while ( ( bytesRead = read( SHELL_Linux.ERROR, buffer, sizeof( buffer ) - 1 ) ) > 0 ) {
      buffer[ bytesRead ] = '\0';
      OS::TERMINAL::OS_console_out( buffer, RED, BLACK, Bold );
    }
  }
  void OS_SHELL::update_Eventpoll()
  {
    // //FUNCTION_PROFILE;
  start_again:
    auto result = poll( SHELL_Linux.Eventpoll, 3, 100 );
    if ( result == -1 && errno == EAGAIN ) goto start_again;

    if ( SHELL_Linux.Eventpoll[ 2 ].revents & POLLIN ) {
      check_error();
      ;
    }
  }

  // void OS_SHELL::Thread_init()
  // {
  //   // FUNCTION_PROFILE;
  //
  //   // std::thread read(output());
  // }
  OS_SHELL::~OS_SHELL()

  {
    // FUNCTION_PROFILE;

    Input( "exit\n" );
    I32 status = 0;
    waitpid( SHELL_Linux.processID, &status, 0 );
    close( SHELL_Linux.WRITE );
    close( SHELL_Linux.READ );
    close( SHELL_Linux.ERROR );
    // Thread_close();
    //
  }
  bool OS_SHELL::Get_output()
  {
    FUNCTION_PROFILE;
    if ( !read_avail() ) {
      prepare_read();
    }

    auto count = 0;
    do {
      if ( output_size == -1 ) {
        // auto err = strerror( errno );
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
        // LOG_ERROR( output_size, err )
      }
      if ( count > 10 ) break;
      output_size = read( SHELL_Linux.READ, &Output_Buffer[ 0 ], OUTPUT_BUFFER_SIZE - 1 );
      count++;
    } while ( output_size == -1 );

    Output_Buffer[ output_size ] = '\0';
    OUTPUT_view                  = std::string_view( Output_Buffer, output_size );

    return read_avail();
  }
  bool OS_SHELL::Input( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command );

    if ( !write_avail() ) {
      prepare_write();
    }
    U64 write_pos = 0;
    I32 res       = 0;
    U64 ip_size   = command.size();
    do {

      res = write( SHELL_Linux.WRITE, command.data() + write_pos, ip_size );

      if ( res == -1 ) {
        auto err = strerror( errno );
        LOG_ERROR( output_size, err )
        res = 0;
      }
      write_pos += res;
      ip_size -= res;
    } while ( ( write_pos != command.size() ) );

    if ( res != -1 ) {
      LOG_DEBUG( "write was sucessfull ", command );
      if ( res == command.size() ) {
        LOG_DEBUG( "sucessfull written  ", res );
        return true;
      }
      return false;
    }
    else {
      auto err = strerror( errno );
      LOG_CRITICAL( "WRITE FAILED", err );

      return false;
    }
  }

  bool OS_SHELL::shell( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command )

    if ( Input( command ) ) {
      return Get_output();
    }
    else {
      ERROR_view = Write_error;
      return 0;
    }
  }

  std::string OS_SHELL::shell_read_complete()
  {
    FUNCTION_PROFILE;

    std::string result;

    while ( Get_output() ) {
      result += OUTPUT_view;
    }
    return result;
  }

  void OS_SHELL::flush_output()
  {

    FUNCTION_PROFILE;
    if ( read_avail() ) {
      LOG( "ok got pollin " )
      char flush_buff[ 1024 ];
      ssize_t val = 1;
      while ( val && read_avail() ) {

        val                = read( SHELL_Linux.READ, flush_buff, 1023 );
        flush_buff[ 1023 ] = '\0';

        // LOG( flush_buff )
        // LOG( "\n val \t", val )
      }
    }
  }

  void OS_SHELL::exec_command( std::string_view cmd )
  {

    FUNCTION_PROFILE_ARG( cmd )

    if ( read_avail() ) {
      flush_output();
    }
    Input( cmd );
  }
  bool OS_SHELL::shell_read_again()
  {
    Get_output();
    prepare_read();
    return ( read_avail() && write_avail() );
  }
  std::string_view OS_SHELL::exec( std::string_view command )
  {
    FUNCTION_PROFILE_ARG( command )

    if ( read_avail() ) {
      flush_output();
    }

    if ( shell( command ) ) {
      ALERT( ERROR_view );
      flush_output();
    }
    return OUTPUT_view;
  }
  void OS_SHELL::prepare_read()
  {
    auto count = 0;
    FUNCTION_PROFILE;
    U16 time = 1;
    do {
      std::this_thread::sleep_for( std::chrono::nanoseconds( time ) );
      count++;
      time = 1 << count;
    } while ( !read_avail() && ( count < 16 ) );
  }
  void OS_SHELL::prepare_write()
  {

    FUNCTION_PROFILE;
    auto count = 0;
    do {
      std::this_thread::sleep_for( std::chrono::nanoseconds( 100 ) );

      count++;
    } while ( !write_avail() && ( count < 100 ) );
  }

  bool OS_SHELL::read_avail()
  {
    FUNCTION_PROFILE
    update_Eventpoll();
    return ( SHELL_Linux.Eventpoll[ 0 ].revents & POLLIN );
  }
  bool OS_SHELL::write_avail()
  {
    // //FUNCTION_PROFILE
    update_Eventpoll();
    return ( SHELL_Linux.Eventpoll[ 1 ].revents & POLLOUT );
  }
  bool OS_SHELL::error_avail()
  {
    // //FUNCTION_PROFILE
    update_Eventpoll();
    return ( SHELL_Linux.Eventpoll[ 2 ].revents & POLLIN );
  }

}  // namespace OS::SHELL
#endif
