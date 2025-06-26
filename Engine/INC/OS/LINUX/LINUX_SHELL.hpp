#pragma once

#include <Define.hpp>
#if ODS_LINUX
  #include <poll.h>
  #include <spawn.h>
  #include <sys/poll.h>
  #include <sys/wait.h>
  #include <unistd.h>

  #include <Types.hpp>
  #define LINUX_SHELL_VARIABLE LINUX_SHELL_VARIABLES SHELL_Linux;

  #define LINUX_SHELL_PROCESS \
    void update_Eventpoll();  \
    void check_error();       \
    void prepare_read();      \
    void prepare_write();     \
    bool write_avail();       \
    bool read_avail();        \
    bool error_avail();

struct LINUX_SHELL_VARIABLES {
  // we will need one file descriptors
  // one process process id varible

  pid_t processID;
  I32 READ;
  I32 WRITE;  // stdin for bash / other shell
  I32 ERROR;
  // Eventpoll indexing
  //  READ=0
  //  WRITE=1
  //  ERROR=2
  pollfd Eventpoll[ 3 ];
};

#else
  #define LINUX_SHELL_VARIABLE

  #define LINUX_SHELL_PROCESS

#endif
