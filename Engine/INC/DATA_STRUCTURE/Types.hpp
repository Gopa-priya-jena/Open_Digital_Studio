/*
 * i donot intend it to be in a namespace as i intend to use these heavily for
 * all types
 *
 *
 *  */
#pragma once
#include <Compiler_Intrinsics.hpp>
#include <cstdint>

// unsigned  I32 variants

typedef std::uint8_t  U8;
typedef std::uint16_t U16;
typedef std::uint32_t U32;
typedef std::uint64_t U64;
// typedef unsigned __int128 u128;

// signed  I32 variants
typedef std::int8_t  I8;
typedef std::int16_t I16;
typedef std::int32_t I32;
typedef std::int64_t I64;
// typedef __int128     i128;

// floating types
#if GCC
#  include <stdfloat>
typedef std::float16_t  F16;
typedef std::float32_t  F32;
typedef std::float64_t  F64;
typedef std::float128_t F128;
#elif CLANG
#  include <bits/floatn.h>
typedef _Float16  F16;
typedef _Float32  F32;
typedef _Float64  F64;
typedef _Float128 F128;
#endif
