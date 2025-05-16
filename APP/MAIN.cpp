
#include <DATA_STRUCTURE/DATA_STRUCTURE.hpp>
#include <DEBUG.hpp>
#include <Global.hpp>
#include <OS/OS.hpp>
#include <iostream>

#include "OS/OS_INFO.hpp"

void hellow() {
  FUNCTION_PROFILE;

  std::cout << "hellow" << std::endl;
}
int main() {
  hellow();

  OS::TERMINAL::OS_console_out("HELLOW \n", RED, BLACK, Bold);
  // OS::OS_Init();
  OS::INFO::GET_all_HW_info();
  using namespace DS;

  // hellow();

  END_LOG
  return 0;
}
