#include <gtest/gtest.h>

TEST( ShellTest, InterfaceTest )
{

  EXPECT_STRNE( "hello", "world" );
  // Expect equality.
  EXPECT_EQ( 7 * 6, 42 );
}
