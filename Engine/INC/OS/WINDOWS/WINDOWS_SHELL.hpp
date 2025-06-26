#pragma once
#include <Define.hpp>
#if ODS_WINDOWS
  #include <windows.h>

  #include <Types.hpp>
  #define WINDOWS_SHELL_VARIABLE WINDOWS_SHELL_VARIABLES SHELL_windows;

  #define WINDOWS_SHELL_PROCESS \
    void prepare_read();        \
    bool read_avail();          \
    bool error_avail();         \
    void populate_error();

struct WINDOWS_SHELL_VARIABLES {

  HANDLE READ;
  HANDLE WRITE;
  HANDLE Error;
  PROCESS_INFORMATION ProcInfo;
};

#else
  #define WINDOWS_SHELL_VARIABLE
  #define WINDOWS_SHELL_PROCESS
#endif
