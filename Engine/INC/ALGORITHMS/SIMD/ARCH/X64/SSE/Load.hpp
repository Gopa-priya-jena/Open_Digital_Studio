#pragma once
#include <Types.hpp>

namespace SIMD {
  namespace SSE {

    F_INLINE inline constexpr V128 LDA_F32_X4( F32 &fvec ) { return _mm_load_ps( &fvec ); }
    F_INLINE inline constexpr V128 LDU_F32_X4( F32 &fvec ) { return _mm_loadu_ps( &fvec ); }
    F_INLINE inline constexpr V128 BLD_F32_X4( F32 &fvec ) { return _mm_load_ps1( &fvec ); }
    F_INLINE inline constexpr V128 LDU_F32_X1_L0( F32 &fvec ) { return _mm_load_ss( &fvec ); }
    F_INLINE inline constexpr V128 LDU_F32_X4_L0123( V128 Lx2, F32 &H ) { return _mm_loadh_pi( Lx2, reinterpret_cast< __m64 * >( &H ) ); }
    F_INLINE inline constexpr V128 LDU_F32_X4_L2301( V128 Hx2, F32 &L ) { return _mm_loadl_pi( Hx2, reinterpret_cast< __m64 * >( &L ) ); }
    F_INLINE inline constexpr V128 LDA_F64_X4_r( F32 &fvec ) { return _mm_loadr_ps( &fvec ); }
  }  // namespace SSE

  namespace SSE2 {

    F_INLINE inline constexpr VF128 LDA_F64_X2( F64 &f1, F64 &f2 )
    {
      F64 arr[ 2 ]{ f1, f2 };
      return _mm_load_pd( arr );
    }
    F_INLINE inline constexpr VF128 LDA_F64_X2( F64 &fvec ) { return _mm_load_pd( &fvec ); }
    F_INLINE inline constexpr VF128 LDU_F64_X2( F64 &fvec ) { return _mm_loadu_pd( &fvec ); }
    F_INLINE inline constexpr VF128 BLD_F64_X1( F64 &fvec ) { return _mm_load_pd1( &fvec ); }
    F_INLINE inline constexpr VF128 LDU_F64_X1_L0( F64 &fvec ) { return _mm_load_sd( &fvec ); }
    F_INLINE inline constexpr VF128 LDU_I128_X2_L01( VF128 &L, F64 &H ) { return _mm_loadh_pd( L, &H ); }
    F_INLINE inline constexpr VF128 LDU_I128_X2_L10( VF128 &H, F64 &L ) { return _mm_loadl_pd( H, &L ); }
    F_INLINE inline constexpr VF128 LDA_F64_X2_r( F64 &fvec ) { return _mm_loadr_pd( &fvec ); }

    // integer type loads
    F_INLINE inline constexpr VI128 LDA_I128_X1( I128 &i128 ) { return _mm_load_si128( reinterpret_cast< VI128 * >( &i128 ) ); }
    F_INLINE inline constexpr VI128 LDU_I128_X1( I128 &i128 ) { return _mm_loadu_si128( reinterpret_cast< VI128 * >( &i128 ) ); }
    F_INLINE inline constexpr VI128 LDU_I64_X1_L1( I128 &i128 ) { return _mm_loadl_epi64( reinterpret_cast< VI128 * >( &i128 ) ); }
    F_INLINE inline constexpr VI128 LDU_I16_X1_L0( I16 &i16 ) { return _mm_loadu_si16( &i16 ); }
    F_INLINE inline constexpr VI128 LDU_I32_X1_L0( I32 &i32 ) { return _mm_loadu_si32( &i32 ); }
    F_INLINE inline constexpr VI128 LDU_I164_X1_L0( I64 &i64 ) { return _mm_loadu_si64( &i64 ); }
  }  // namespace SSE2

  namespace SSE3 {
    F_INLINE inline constexpr VI128 LDU_I128_X1( I128 &i128 ) { return _mm_lddqu_si128( reinterpret_cast< VI128 * >( &i128 ) ); }

  }  // namespace SSE3
  namespace SSE4_1 {
    template < typename T >  //
    F_INLINE inline constexpr V128 SLDA_X128( T &i128 )
    {
      return _mm_stream_load_si128( &i128 );
    }

  }  // namespace SSE4_1
}  // namespace SIMD
