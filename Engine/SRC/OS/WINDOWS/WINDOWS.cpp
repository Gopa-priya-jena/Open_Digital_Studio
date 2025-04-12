#if ODS_WINDOWS

#include <OS/OS.hpp>
namespace OS {

void *alloc(size_t DataType, size_t number) {
  return _alloca(DataType * number);
}

} // namespace OS
#endif
