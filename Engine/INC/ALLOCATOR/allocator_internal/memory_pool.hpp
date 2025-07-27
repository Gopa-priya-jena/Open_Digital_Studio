
#pragma once
#include <ALLOCATOR/allocator_internal/common.hpp>
constexpr U32 uniform_pool_type    = 1;
constexpr U32 nonuniform_pool_type = 2;

struct pointer {
  U64 Size;
  void *ptr;
  pointer();
  pointer( U64 size, void *pointer );
  pointer( pointer &pointer_ele );
  pointer &operator=( pointer &&ptr );
};
struct uniform_pool {
  U32 Type;
  U32 pointer_count;
  U64 pointer_size;
  void *pointer_list[];
  uniform_pool()                 = delete;
  uniform_pool( uniform_pool & ) = delete;
  uniform_pool( U32 element_count, U64 ptr_size );
  class upool_itter {
    U32 pos;
    U32 max;
    uniform_pool *pool_ptr;

   public:
    bool end;
    upool_itter( uniform_pool *pool );

    upool_itter( upool_itter &pool );
    void *&get_data();
    upool_itter &operator++();

    upool_itter &operator++( int );
  };

  upool_itter Get_ittr();
  bool modify_push( void *ptr, U64 Size );
  void *next();
  U64 pool_size();
};
inline U64 calc_sizeof_uniform_pool( U32 ele_count )  //  claulate the expeted size
{

  return sizeof( uniform_pool ) + sizeof( void * ) * ele_count;
}

struct non_uniform_pool {
  U32 Type;
  U32 pointer_count;
  pointer pointer_list[];
  non_uniform_pool()                     = delete;
  non_uniform_pool( non_uniform_pool & ) = delete;
  non_uniform_pool( U32 element_count );
  class nupool_itter {
    U32 pos;
    U32 max;
    non_uniform_pool *pool_ptr;

   public:
    bool end;
    nupool_itter( non_uniform_pool *pool );

    nupool_itter( nupool_itter &pool );
    pointer &get_data();
    nupool_itter &operator++();

    nupool_itter &operator++( int );
  };

  nupool_itter Get_ittr();
  bool modify_push( void *ptr, U64 Size );
  void *next();
  U64 pool_size();
};

inline U64 calc_sizeof_non_uniform_pool( U32 ele_count )  //  claulate the expeted size
{
  return sizeof( non_uniform_pool ) + sizeof( pointer ) * ele_count;
}

inline U32 check_type( void *ptr ) { return *reinterpret_cast< U32 * >( ptr ); }
