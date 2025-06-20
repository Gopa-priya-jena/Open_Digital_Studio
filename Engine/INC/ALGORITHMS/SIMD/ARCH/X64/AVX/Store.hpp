#pragma once
#include <DATA_STRUCTURE/SIMD/SIMD_helper_types.hpp>
#include <Types.hpp>

namespace SIMD {

  namespace AVX {

    F_INLINE inline constexpr void FSTR_F64_X2( F64& STR_loc, Mask< VI128, 64 > mask, VF128 src ) { _mm_maskstore_pd( &STR_loc, mask.value(), src ); }
    F_INLINE inline constexpr void FSTR_F64_X4( F64& STR_loc, Mask< VI256, 64 > mask, VF256 src ) { _mm256_maskstore_pd( &STR_loc, mask.value(), src ); }
    F_INLINE inline constexpr void FSTR_F32_X4( F32& STR_loc, Mask< VI128, 32 > mask, VF128 src ) { _mm_maskstore_ps( &STR_loc, mask.value(), src ); }
    F_INLINE inline constexpr void FSTR_F32_X8( F32& STR_loc, Mask< VI256, 32 > mask, VF256 src ) { _mm256_maskstore_ps( &STR_loc, mask.value(), src ); }

    F_INLINE inline constexpr void STRA_F32_X8( F32& STR_loc, VF256 src ) { _mm256_store_ps( &STR_loc, src ); }
    F_INLINE inline constexpr void STRA_F64_X4( F64& STR_loc, VF256 src ) { _mm256_store_pd( &STR_loc, src ); }
    F_INLINE inline constexpr void STRA_I256_X1( VI256& STR_loc, VI256 src ) { _mm256_store_si256( &STR_loc, src ); }
    F_INLINE inline constexpr void STRU_F64_X4( F64& STR_loc, VF256 src ) { _mm256_storeu_pd( &STR_loc, src ); }
    F_INLINE inline constexpr void STRU_F32_X8( F32& STR_loc, VF256 src ) { _mm256_storeu_ps( &STR_loc, src ); }
    F_INLINE inline constexpr void STRU_I256_X1( VI256& STR_loc, VI256 src ) { _mm256_storeu_si256( &STR_loc, src ); }
    F_INLINE inline constexpr void STRU_F32_X8( F32& high, F32& low, VF256 src ) { _mm256_storeu2_m128( &high, &low, src ); }
    F_INLINE inline constexpr void STRU_F64_X2( F64& high, F64& low, VF256 src ) { _mm256_storeu2_m128d( &high, &low, src ); }
    F_INLINE inline constexpr void STRU_I128_X8( I128& high, I128& low, VF256 src ) { _mm256_storeu2_m128i( reinterpret_cast< VI128* >( &high ), reinterpret_cast< VI128* >( &low ), src ); }

    F_INLINE inline constexpr void SSTRU_F64_X4( F64& STR_loc, VF256 src ) { _mm256_stream_pd( &STR_loc, src ); }
    F_INLINE inline constexpr void SSTRU_F32_X8( F32& STR_loc, VF256 src ) { _mm256_stream_ps( &STR_loc, src ); }
    F_INLINE inline constexpr void SSTRU( void* STR_loc, VF256 src ) { _mm256_stream_si256( STR_loc, src ); }

  }  // namespace AVX

  namespace AVX2 {

    F_INLINE inline constexpr void FSTR_I32_X4( I32& STR_loc, Mask< VI128, 32 > mask, VI128 src ) { _mm_maskstore_epi32( &STR_loc, mask.value(), src ); }
    F_INLINE inline constexpr void FSTR_I32_X8( I32& STR_loc, Mask< VI256, 32 > mask, VI256 src ) { _mm256_maskstore_epi32( &STR_loc, mask.value(), src ); }

    F_INLINE inline constexpr void FSTR_I64_X2( I64& STR_loc, Mask< VI128, 64 > mask, VI128 src ) { _mm_maskstore_epi64( reinterpret_cast< long long* >( &STR_loc ), mask.value(), src ); }
    F_INLINE inline constexpr void FSTR_I64_X4( I64& STR_loc, Mask< VI256, 64 > mask, VI256 src ) { _mm256_maskstore_epi64( reinterpret_cast< long long* >( &STR_loc ), mask.value(), src ); }

  }  // namespace AVX2

}  // namespace SIMD
