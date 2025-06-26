/*
 * i donot intend it to be in a namespace as i intend to use these heavily for
 * all types
 *
 *
 *  */
#pragma once
#include <Compiler_Intrinsics.hpp>
#include <cstdint>
// other types declared other places
#include <ALGORITHMS/SIMD/ARCH/X64/AVX/Types.hpp>
#include <ALGORITHMS/SIMD/ARCH/X64/SSE/Types.hpp>
#include <Define.hpp>
// unsigned  integer variants

using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
#if GCC
using U128 = unsigned __int128;
#endif

// signed  integer  variants
using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
#if GCC
using I128 = __int128;
#endif

// floating types
#if GCC
  #include <stdfloat>
using F16  = std::float16_t;
using F32  = std::float32_t;
using F64  = std::float64_t;
using F128 = std::float128_t;
#elif CLANG
  #include <bits/floatn.h>
using F16  = _Float16;
using F32  = _Float32;
using F64  = _Float64;
using F128 = _Float128;
#elif MSVC
// using F16  = _Float16;
using F32  = float;
using F64  = double;
using F128 = long double;
#endif
// simd specific unions
