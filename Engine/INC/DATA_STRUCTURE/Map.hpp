#pragma once
#include <Types.hpp>
#include <utility>
template < class Key, class Value, I32 N >
class FMap {

  Key KEYs[ N ];
  Value VALUEs[ N ];
  static Value KeyNotFound()  // not constexpr
  {
    return {};
  }
  // template < std::size_t... Is >
  // consteval FMap( std::index_sequence< Is... >, const std::pair< Key, Value > ( &&pairs )[ N ] ) : KEYs{ std::get< 0 >( pairs[ Is ] )... }, VALUEs{ std::get< 1 >( pairs[ Is ] )... }
  // {
  // }
  struct KV {
    Key k;
    Value v;
  };

 public:
  consteval FMap( std::initializer_list< KV > init_list )
  {
    // static_assert( init_list.size() == N, "Initializer list size must exactly match N for consteval FMap construction." );
    auto i = 0;
    for ( auto kvs = init_list.begin(); i < N; i++, kvs++ ) {
      KEYs[ i ]   = kvs->k;
      VALUEs[ i ] = kvs->v;
    }
  }

  constexpr void fill( const Key k, const Value v, const I32 i )
  {
    KEYs[ i ]   = k;
    VALUEs[ i ] = v;
  }
  constexpr Value operator[]( Key key ) const
  {

    for ( I32 i = 0; i < N; i++ )
      if ( KEYs[ i ] == key ) {
        return VALUEs[ i ];
      }

    return KeyNotFound();
  }
};
