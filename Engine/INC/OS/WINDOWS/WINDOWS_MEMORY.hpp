#pragma once
#include <Define.hpp>
#if ODS_WINDOWS
  #include <malloc.h>
  #define WINDOWS_ALLOCA( X ) _alloca()
#else
  #define WINDOWS_ALLOCA( X )
#endif
