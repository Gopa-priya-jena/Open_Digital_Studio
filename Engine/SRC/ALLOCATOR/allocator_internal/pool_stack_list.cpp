
#include <ALLOCATOR/allocator_internal/memory_pool.hpp>
#include <ALLOCATOR/allocator_internal/pool_stack_list.hpp>
#include <cstddef>
#include <new>

pool_stack_list::pool_stack_list()
{

  head    = new ( alloc( 100 ) ) pool_stack( nullptr, 100 );
  tail    = head;
  present = head;
}

void pool_stack_list::update_tail()
{

  pool_stack *temp = tail;

  while ( temp->next != nullptr ) {
    temp = temp->next;
  }

  tail = temp;
}
pool_stack *pool_stack_list::get_present()
{
  // update_tail();
  return present;
}

pool_stack_list::~pool_stack_list()
{

  auto free_uniform = []( uniform_pool *up ) {
    // return;

    auto i  = 0;
    auto it = up->Get_ittr();
    while ( !it.end ) {

      // dumb_dwalloc( it.get_data(), up->pointer_size );
      // it++;
    }
  };

  auto free_nonuniform = []( non_uniform_pool *nup ) {
    // return;
    auto i  = 0;
    auto it = nup->Get_ittr();
    while ( !it.end ) {

      // dumb_dwalloc( it.get_data().ptr, it.get_data().Size );
      // std::cout << "\n non uniform dumb_free" << i++ << std::endl;
      // dealloc( it.get_dat().ptr, it.get_dat().Size );
      it++;
    }
  };

  auto free_data = [ & ]( pool_stack *ptr ) {
    auto it = ptr->Get_ittr();
    while ( !it.end )

    {
      if ( check_type( it.Get_data() ) == uniform_pool_type ) {
        free_uniform( reinterpret_cast< uniform_pool * >( it.Get_data() ) );
      }
      else {

        free_nonuniform( reinterpret_cast< non_uniform_pool * >( it.Get_data() ) );
      }
      it++;
    }
  };
  auto free_self = []( pool_stack *ptr ) { dealloc( ptr, ptr->Size ); };

  update_tail();

  pool_stack *last = tail;
  pool_stack *prev = last->prev;
  while ( last != nullptr ) {

    prev = last->prev;
    free_data( last );
    free_self( last );
    last = prev;
  }
}

pool_stack_list &pool_stack_list::operator++()
{
  update_tail();
  if ( tail != present ) present = present->next;

  return *this;
}

pool_stack_list &pool_stack_list::operator++( int )
{

  ++( *this );

  return *this;
}

pool_stack_list &pool_stack_list::operator--()
{

  if ( head != present ) present = present->prev;

  return *this;
}
pool_stack_list &pool_stack_list::operator--( int )
{

  --( *this );

  return *this;
}
