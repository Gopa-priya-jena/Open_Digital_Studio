
#include <ALLOCATOR/allocator_internal/memory_pool.hpp>
#include <cstdint>

pointer::pointer() : Size( 0 ), ptr( nullptr ) {}
pointer::pointer( U64 size, void *pointer ) : Size( size ), ptr( static_cast< U8 * >( pointer ) ) {}

pointer::pointer( pointer &pointer_ele )
{
  Size = pointer_ele.Size;
  ptr  = pointer_ele.ptr;
}
pointer &pointer::operator=( pointer &&ptr_ele )
{
  Size = ptr_ele.Size;
  ptr  = ptr_ele.ptr;
  return *this;
}
// uniform pool struct definations
uniform_pool::uniform_pool( U32 element_count, U64 ptr_size )
{
  Type          = uniform_pool_type;
  pointer_count = element_count;
  pointer_size  = ptr_size;
}

bool uniform_pool::modify_push( void *ptr, U64 Size )
{

  if ( Size == pointer_size ) {
    pointer_list[ pointer_count ] = ptr;
    pointer_count++;

    return true;
  }
  else {
    return false;
  }
}

void *uniform_pool::next() { return ( void * )( ( uintptr_t )( Type ) + calc_offset_fixed( Type, pointer_count, pointer_size ) + ( sizeof( void * ) * pointer_count ) ); }

U64 uniform_pool::pool_size() { return calc_offset_fixed( Type, pointer_count, pointer_size ) + ( sizeof( void * ) * pointer_count ); }

uniform_pool::upool_itter::upool_itter( uniform_pool *pool )
{

  pool_ptr = pool;
  pos      = 0;
  max      = pool->pointer_count;

  end = false;
}

uniform_pool::upool_itter::upool_itter( uniform_pool::upool_itter &pool )
{

  pos      = pool.pos;
  max      = pool.max;
  pool_ptr = pool.pool_ptr;

  end = pool.end;
}
void *&uniform_pool::upool_itter::get_data() { return pool_ptr->pointer_list[ pos ]; }

uniform_pool::upool_itter &uniform_pool::upool_itter::operator++()
{
  ( *this )++;

  return *this;
}

uniform_pool::upool_itter &uniform_pool::upool_itter::operator++( int )
{

  if ( pos + 1 < max ) {
    pos++;
  }
  else {
    end = true;
  }

  return *this;
}

uniform_pool::upool_itter uniform_pool::Get_ittr() { return upool_itter( this ); }

// non uniform pool
non_uniform_pool::non_uniform_pool( U32 element_count )
{
  Type          = nonuniform_pool_type;
  pointer_count = element_count;
}

bool non_uniform_pool::modify_push( void *ptr, U64 Size ) { return false; }

void *non_uniform_pool::next() { return ( void * )( ( uintptr_t )( Type ) + calc_offset_fixed( Type, pointer_count ) + ( sizeof( pointer ) * pointer_count ) ); }

U64 non_uniform_pool::pool_size() { return calc_offset_fixed( Type, pointer_count ) + ( sizeof( pointer ) * pointer_count ); }

non_uniform_pool::nupool_itter::nupool_itter( non_uniform_pool *pool )
{

  pool_ptr = pool;
  pos      = 0;
  max      = pool->pointer_count;

  end = false;
}

non_uniform_pool::nupool_itter::nupool_itter( non_uniform_pool::nupool_itter &pool )
{

  pos      = pool.pos;
  max      = pool.max;
  pool_ptr = pool.pool_ptr;
  end      = pool.end;
}
pointer &non_uniform_pool::nupool_itter::get_data() { return pool_ptr->pointer_list[ pos ]; }

non_uniform_pool::nupool_itter &non_uniform_pool::nupool_itter::operator++()
{
  ( *this )++;
  return *this;
}

non_uniform_pool::nupool_itter &non_uniform_pool::nupool_itter::operator++( int )
{

  if ( pos + 1 < max ) {
    pos++;
  }
  else {
    end = true;
  }

  return *this;
}

non_uniform_pool::nupool_itter non_uniform_pool::Get_ittr() { return nupool_itter( this ); }
