#include <Define.hpp>
#include <immintrin.h>
#if ( GCC || CLANG )
#  include <cpuid.h>
#elif ( MSVC )
#  include <ammintrin.h>
#  include <immintrin.h>
#  include <intrin.h>
#endif
