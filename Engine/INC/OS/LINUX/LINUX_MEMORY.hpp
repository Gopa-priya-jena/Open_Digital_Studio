#include <Define.hpp>
#if ODS_LINUX

  #include <features.h>
  #include <linux/prctl.h>

  #define LINUX_ALLOCA( X ) alloca( X );
#else

  #define LINUX_ALLOCA( X )

#endif
