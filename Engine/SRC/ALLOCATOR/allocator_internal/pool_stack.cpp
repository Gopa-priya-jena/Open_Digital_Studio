
#include <ALLOCATOR/allocator_internal/pool_stack.hpp>
#include <new>
void *pool_stack::get_data() { return ( void * )( ( uintptr_t )&next + sizeof( pool_stack ) ); }
U64 pool_stack::available_space() { return Size - ( ( uintptr_t )( stack_ptr ) - ( uintptr_t )( &next ) ); }
void pool_stack::create_new( U64 hint )
{
  U64 allocation_size = Size * 2 + hint;

  next = new ( alloc( allocation_size ) ) pool_stack( this, allocation_size );
}

pool_stack::pool_stack( pool_stack *previous, U64 alloc_Size )
{
  next          = nullptr;
  prev          = previous;
  stack_ptr     = reinterpret_cast< U8 * >( get_data() );
  Size          = alloc_Size;
  element_count = 0;
}

void pool_stack::push( void *ptr, U64 Size )
{
  auto it    = Get_ittr();
  void *last = it.get_last();

  if ( available_space() >= sizeof( ptr ) || available_space() >= sizeof( pointer ) ) {
    if ( check_type( last ) == uniform_pool_type ) {
      uniform_pool *uptr = reinterpret_cast< uniform_pool * >( last );
      if ( uptr->pointer_size == Size ) {
        uptr->modify_push( ptr, Size );
        return;
      }
    }
    non_uniform_pool *uptr = reinterpret_cast< non_uniform_pool * >( last );
    uptr->modify_push( ptr, Size );
  }
  else {

    create_new();
    non_uniform_pool *pool       = &next->create_non_uniform_pool_entry( 1 );
    pool->pointer_list[ 0 ].Size = Size;
    pool->pointer_list[ 0 ].ptr  = ptr;
  }
}
uniform_pool &pool_stack::create_uniform_pool_entry( const U32 uniform_element_count, const U64 ptr_size )
{

  if ( available_space() >= calc_sizeof_uniform_pool( uniform_element_count ) ) {
    element_count++;
    return *new ( stack_ptr ) uniform_pool( uniform_element_count, ptr_size );
  }
  else {
    create_new( calc_sizeof_uniform_pool( uniform_element_count ) );
    return next->create_uniform_pool_entry( uniform_element_count, ptr_size );
  }
}
non_uniform_pool &pool_stack ::create_non_uniform_pool_entry( const U32 non_uniform_element_count )
{
  if ( available_space() >= calc_sizeof_non_uniform_pool( non_uniform_element_count ) ) {
    element_count++;
    return *new ( stack_ptr ) non_uniform_pool( non_uniform_element_count );
  }
  else {
    create_new( calc_sizeof_non_uniform_pool( non_uniform_element_count ) );
    return next->create_non_uniform_pool_entry( non_uniform_element_count );
  }
}

pool_stack::pool_stack_ittr::pool_stack_ittr( pool_stack_ittr &ittr )
{
  stack   = ittr.stack;
  Data    = ittr.Data;
  end     = ittr.end;
  end_ptr = ittr.end_ptr;
  ele     = ittr.ele;
}

pool_stack::pool_stack_ittr::pool_stack_ittr( pool_stack *st )
{
  stack   = st;
  Data    = st->get_data();
  end_ptr = ( U8 * )( st ) + st->Size;
  if ( st->element_count == 0 ) {
    end = true;
    ele = 0;
  }
  else {
    end = false;
    ele = st->element_count;
  }
}

pool_stack::pool_stack_ittr &pool_stack::pool_stack_ittr::operator++()
{

  auto find_setnext = [ & ]< typename T >( T *pool ) {
    // check size?

    if ( ( pool->next() < ( void * )( uintptr_t( &stack->next ) + stack->Size ) ) && ( ele + 1 < stack->element_count ) ) {
      Data = pool->next();
    }
    else {
      end = true;
    }

    // assign next
  };

  if ( check_type( Data ) == uniform_pool_type ) {

    uniform_pool *ptr = reinterpret_cast< uniform_pool * >( Data );
    find_setnext( ptr );
  }
  else {
    non_uniform_pool *ptr = reinterpret_cast< non_uniform_pool * >( Data );
    find_setnext( ptr );
  }

  return *this;
}
pool_stack::pool_stack_ittr &pool_stack::pool_stack_ittr::operator++( int )
{

  ++*this;
  return *this;
}

pool_stack::pool_stack_ittr pool_stack::Get_ittr() { return pool_stack_ittr( this ); }

void *pool_stack::pool_stack_ittr::get_last()
{

  while ( !end ) {
    ( *this )++;
  }
  return Data;
}
