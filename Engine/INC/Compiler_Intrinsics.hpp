#include <immintrin.h>

#include <Define.hpp>

#if ( GCC || CLANG )
  #include <cpuid.h>
#elif ( MSVC )
  #include <ammintrin.h>
  #include <immintrin.h>
  #include <intrin.h>
#endif

#if ( GCC )
  #define F_INLINE __attribute__( ( always_inline ) )
#elif ( CLANG )
  #define F_INLINE __attribute__( ( always_inline ) )
#elif ( MSVC )
  #define F_INLINE __forceinline
#endif
