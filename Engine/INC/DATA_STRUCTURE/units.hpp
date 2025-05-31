#pragma once
#include <cstdint>
#include <string_view>

#include <DATA_STRUCTURE/Types.hpp>
enum class Binary_Storage : U8
{

  bytes = 3, // Bytes
  Bytes = 0, // Bytes
  KB = 10,   // Kilobytes
  MB = 20,   // Megabytes
  GB = 30,   // Gigabytes
  TB = 40,   // Terabytes
  PB = 50    // petabyte
};

consteval U64 operator""_B( unsigned long long value )
{
  return value;
}

consteval U64 operator""_KB( long double value )
{
  return ( U64 )( value * ( 1ULL << static_cast<unsigned long long>(
                                Binary_Storage::KB ) ) );
}
consteval U64 operator""_MB( long double value )
{
  return ( U64 )( value * ( 1ULL << static_cast<unsigned long long>(
                                Binary_Storage::MB ) ) );
}

consteval U64 operator""_GB( long double value )
{
  return ( U64 )( value * ( 1ULL << static_cast<unsigned long long>(
                                Binary_Storage::GB ) ) );
}

consteval U64 operator""_TB( long double value )
{
  return ( U64 )( value * ( 1ULL << static_cast<unsigned long long>(
                                Binary_Storage::TB ) ) );
}
consteval U64 operator""_PB( long double value )
{
  return ( U64 )( value * ( 1ULL << static_cast<unsigned long long>(
                                Binary_Storage::PB ) ) );
}

consteval U64 operator""_KB( unsigned long long value )
{
  return (
      U64 )( value << static_cast<unsigned long long>( Binary_Storage::KB ) );
}
consteval U64 operator""_MB( unsigned long long value )
{
  return (
      U64 )( value << static_cast<unsigned long long>( Binary_Storage::MB ) );
}

consteval U64 operator""_GB( unsigned long long value )
{
  return (
      U64 )( value << static_cast<unsigned long long>( Binary_Storage::GB ) );
}

consteval U64 operator""_TB( unsigned long long value )
{
  return (
      U64 )( value << static_cast<unsigned long long>( Binary_Storage::TB ) );
}
consteval U64 operator""_PB( unsigned long long value )
{
  return (
      U64 )( value << static_cast<unsigned long long>( Binary_Storage::PB ) );
}
