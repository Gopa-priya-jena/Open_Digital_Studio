#pragma once
// sse  version Detection
#include <Compiler_Intrinsics.hpp>
#include <immintrin.h>
//
// #if GCC
// #pragma GCC target "arch=native"
// #endif
//
// #if CLANG
// #pragma clang attribute push(__attribute__((target("native"))), \
//                              apply_to = functions)
// #endif

#ifdef __SSE2__
#define SSE2_ENABLE 1
#endif

#ifdef __SSE3__
#define SSE3_ENABLE 1
#endif

#ifdef __SSSE3__
#define SSSE3_ENABLE 1
#endif

#ifdef __SSE4_1__
#define SSE4_1_ENABLE 1
#endif

#ifdef __SSE4_2__
#define SSE4_2_ENABLE 1
#endif

// AVX version detection

#ifdef __AVX__
#define AVX_ENABLE 1
#endif //
//
//
#ifdef __AVX2__
#define AVX2_ENABLE 1
#endif //

#ifdef __AVX512F__
#define AVX512_ENABLE 1
#endif //
//
// #ifdef __AVX__
// #define AVX_ENABLE 1
// #endif //
//
//
// #ifdef __AVX__
// #define AVX_ENABLE 1
// #endif //
//
//
// #ifdef __AVX__
// #define AVX_ENABLE 1
// #endif //
