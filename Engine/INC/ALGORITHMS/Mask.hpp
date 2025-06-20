#pragma once
#include <Types.hpp>
#include <bitset>

template < U64 Bits, U64 Lanes >
inline consteval std::bitset< Bits > MSB_mask()
{
  std::bitset< Bits > res;
  auto msb_bit = Bits / Lanes;
  for ( auto i = msb_bit - 1; i < Bits; i += msb_bit ) {
    res[ i ] = true;
  }

  return res;
}
template < U64 Bits, typename T >
inline consteval std::bitset< Bits > MSB_mask()
{
  return MSB_mask< Bits, Bits / sizeof( T ) >();
}
template < U64 Bits, U64 Lanes >
inline consteval std::bitset< Bits > LSB_mask()
{
  std::bitset< Bits > res;
  auto lsb_bit = Bits / Lanes;
  for ( U64 i = 0; i < Bits; i += lsb_bit ) {
    res[ i ] = true;
  }

  return res;
}
template < U64 Bits, typename T >
inline consteval std::bitset< Bits > LSB_mask()
{
  return LSB_mask< Bits, Bits / sizeof( T ) >();
}

template < U64 Bits >
consteval std::bitset< Bits > mask_gen( const U64 Lane_size, std::initializer_list< bool > L_H )
{
  auto i = Lane_size - 1;
  std::bitset< Bits > res;
  res.reset();
  for ( auto j = L_H.begin(); ( i < Bits ) && ( j != L_H.end() ); i += Lane_size, j++ ) {
    if ( *j ) res[ i ] = 1;
  }
  return res;
}
