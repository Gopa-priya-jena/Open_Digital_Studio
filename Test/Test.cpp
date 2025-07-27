#include <gtest/gtest.h>

#include <DEBUG.hpp>
#include <OS/OS_SHELL.hpp>
int main( int argc, char **argv )
{
  testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
  // LOG_NORMAL( "started" );
  // OS::SHELL::OS_SHELL shell( OS::SHELL::SHELL_Type::BLOCKING );
  // std::string i;
  // // i = "cat /proc/cpuinfo\n";
  // //
  // // // shell.exec_command( i );
  // // shell.shell( i );
  // // std::string res{};
  // // do {
  // //   res += shell.OUTPUT_view;
  // //
  // // } while ( shell.shell_read_again() );
  // //
  // // shell.exec_command( "TERM=dumb vim -u NONE -n test.txt\n" );
  // // shell.exec_command( "i" );  // Insert mode
  // // shell.exec_command( res );  // Write the text
  // // shell.exec_command( "\u001b" );  // Exit insert mode (ESC key)
  // // shell.exec_command( ":wq" );  // Write command to save and quit
  // // shell.exec_command( "\n" );
  // while ( i != "exit\n" ) {
  //   shell.shell( i );
  //   auto result = shell.OUTPUT_view;
  //   LOG_CRITICAL( result );
  //   std::getline( std::cin, i );
  //   i = i + "\n";
  //
  //   LOG_DEBUG( "command ", i )
  // }
}
