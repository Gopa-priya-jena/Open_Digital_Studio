
#include <Types.hpp>

template < class... Args >
consteval U64 calc_offset_fixed( Args &...arg )
{
  U64 result = 0;
  ( ( result += ( sizeof( arg ) ) ), ... );
  return result;
}
void *alloc( std::size_t sz ) { return malloc( sz ); }
void dealloc( void *ptr, U64 sz ) {}
