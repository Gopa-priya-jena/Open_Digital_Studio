
#include <Types.hpp> 
#pragma once
#include <cstddef>
#include <cstdlib>
namespace DATA_STRUCTURE

{

  template <class Type>

  class Fixed_List
  {
    Type       *List_Data{ nullptr };
    std::size_t data_size{ 0 }, last_position = 0;
    bool        modify = 1;

  public:
    Fixed_List()
    {
    }
    Fixed_List( std::size_t Element_number ) : data_size( Element_number )
    {
      if ( data_size )
      {
        List_Data = new Type[ data_size ];
      }
    }
    ~Fixed_List()
    {
      if ( data_size )
      {
        delete[] List_Data;
      }
    }
    void reserve( std::size_t Element_number )
    {
      if ( !data_size && Element_number )
      {
        data_size = Element_number;

        List_Data =
            reinterpret_cast<Type *>( malloc( data_size * sizeof( Type ) ) );
      }
    }
    bool fill_next( Type &&data )
    {
      if ( !modify )
        return false;
      if ( last_position < data_size )
      {
        List_Data[ last_position ] = data;
        last_position++;
      }
      else
      {
        return false;
      }
      return true;
    }
    void push( std::size_t data )
    {
      if ( !modify )
        return;

      fill_next( data );
    }
    void pop()
    {
      if ( !modify )
        return;

      last_position--;
    }

    Type &operator[]( std::size_t index )
    {
      return List_Data[ index ];
    }

    std::size_t size()
    {
      return last_position;
    }

    std::size_t capacity()
    {
      return data_size;
    }
    void clear_all()
    {
      if ( !modify )
        return;

      last_position = 0;
    }

    void set_modification( bool value )
    {
      modify = value;
    }
  };
} // namespace DATA_STRUCTURE
