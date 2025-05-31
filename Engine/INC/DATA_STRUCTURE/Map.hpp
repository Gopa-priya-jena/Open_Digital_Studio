
#pragma once

template <class Key, class Value,  I32 N>
class FMap
{

  Key          KEYs[ N ];
  Value        VALUEs[ N ];
  static Value KeyNotFound() // not constexpr
  {
    return {};
  }

public:
  constexpr FMap() = default;
  constexpr void fill( const Key k, const Value v, const  I32 i )
  {
    KEYs[ i ] = k;
    VALUEs[ i ] = v;
  }
  constexpr Value operator[]( Key key ) const
  {

    for (  I32 i = 0; i < N; i++ )
      if ( KEYs[ i ] == key )
        return VALUEs[ i ];

    return KeyNotFound();
  }
};
