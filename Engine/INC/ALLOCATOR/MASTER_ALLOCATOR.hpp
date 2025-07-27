#include <Define.hpp>
#if ODS_LINUX
  #include <ALLOCATOR/mem_internals.hpp>
  #include <Types.hpp>
// usage  to keep collection of  pointer of same size

namespace ALLOCATOR {

  // thread allocation  memory

  class MASTER_ALLOCATOR {
    Allocation_table* Base_table;
    Thread_allocation_Tree* Alloc_Tree;
    U64 Page_size;
    U64 MAX_Huge_page_size;

    void Base_init( U64 pre_threads );
    void Base_grow( U64 pre_threads );
    void Base_destroy( U64 pre_threads );

   public:
    MASTER_ALLOCATOR();
    ~MASTER_ALLOCATOR();
  };
}  // namespace ALLOCATOR
#endif
