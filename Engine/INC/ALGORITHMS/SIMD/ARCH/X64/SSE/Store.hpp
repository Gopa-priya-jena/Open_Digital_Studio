#include <ALGORITHMS/SIMD/ARCH/X64/SSE/Types.hpp>
#include <Types.hpp>

#include "DATA_STRUCTURE/SIMD/SIMD_helper_types.hpp"
namespace SIMD {
  namespace SSE {
    F_INLINE inline constexpr void FSTRA_U8( U8& STR_loc, Mask< V64, 8 > mask, V64 val ) { _mm_maskmove_si64( val, mask.value(), reinterpret_cast< char* >( &STR_loc ) ); }
    F_INLINE inline constexpr void STRA_F32_X4( F32& STR_loc, V128 vf128 ) { _mm_store_ps( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void STRU_F32_X4( F32& STR_loc, V128 vf128 ) { _mm_storeu_ps( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void BSTRA_F32_X4_IL( F32& STR_loc, V128 vf128_l ) { _mm_store_ps1( &STR_loc, vf128_l ); }
    F_INLINE inline constexpr void STRU_F32_X1_IL( F32& STR_loc, V128 vf128_l ) { _mm_store_ss( &STR_loc, vf128_l ); }
    F_INLINE inline constexpr void STRA_F32_X2_IH( V64& STR_loc, V128 vf128_h ) { _mm_storeh_pi( &STR_loc, vf128_h ); }
    F_INLINE inline constexpr void STRA_F32_X2_IL( V64& STR_loc, V128 vf128_l ) { _mm_storel_pi( &STR_loc, vf128_l ); }
    F_INLINE inline constexpr void SSTRA_V64( V64& STR_loc, V64 v64 ) { _mm_stream_pi( &STR_loc, v64 ); }
    F_INLINE inline constexpr void SSTRA_F32_X4( V64& STR_loc, V128 v128 ) { _mm_stream_ps( &STR_loc, v128 ); }
  }  // namespace SSE
  namespace SSE2 {
    // int
    F_INLINE inline constexpr void STRU_I32_X1_IL( VI128& STR_loc, VI128 vi128 ) { _mm_storeu_si32( &STR_loc, vi128 ); }
    F_INLINE inline constexpr void STRU_I64_X1_IL( VI128& STR_loc, VI128 vi128 ) { _mm_storeu_si64( &STR_loc, vi128 ); }
    F_INLINE inline constexpr void STRA_I64_X1_IL( VI128& STR_loc, VI128 vi128 ) { _mm_storel_epi64( &STR_loc, vi128 ); }
    F_INLINE inline constexpr void STRA_I128_X1( VI128& STR_loc, VI128 vi128 ) { _mm_store_si128( &STR_loc, vi128 ); }
    F_INLINE inline constexpr void STRU_I128_X1( VI128& STR_loc, VI128 vi128 ) { _mm_storeu_si128( &STR_loc, vi128 ); }
    // floating
    F_INLINE inline constexpr void STRA_F64_X2( F64& STR_loc, V128 vf128 ) { _mm_store_pd( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void STRU_F64_X2( F64& STR_loc, V128 vf128 ) { _mm_storeu_pd( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void STRU_F64_X1_IL( F64& STR_loc, V128 vf128 ) { _mm_store_sd( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void STRA_F64_X1_IH( F64& STR_loc, VF128 vf128 ) { _mm_storeh_pd( &STR_loc, vf128 ); }
    F_INLINE inline constexpr void STRA_F64_X1_IL( F64& STR_loc, VF128 vf128 ) { _mm_storel_pd( &STR_loc, vf128 ); }
    // streaming str
    F_INLINE inline constexpr void FSSTRU_U8( U8& STR_loc, Mask< VI128, 8 > mask, V128 val ) { _mm_maskmoveu_si128( val, mask.value(), reinterpret_cast< char* >( &STR_loc ) ); }

    F_INLINE inline constexpr void SSTRA_I128_X1( VI128& STR_loc, VF128 val ) { _mm_stream_si128( &STR_loc, val ); }

    F_INLINE inline constexpr void SSTRA_U32_X1_IL( U32& STR_loc, I32 val ) { _mm_stream_si32( &STR_loc, val ); }
    F_INLINE inline constexpr void SSTRA_I32_X1_IL( I32& STR_loc, I32 val ) { _mm_stream_si32( &STR_loc, val ); }
    F_INLINE inline constexpr void SSTRA_I64_X2( I64& STR_loc, I64 val ) { _mm_stream_si64( &STR_loc, val ); }
    F_INLINE inline constexpr void SSTRA_I64_X2( U64& STR_loc, U64 val ) { _mm_stream_si64( &STR_loc, reinterpret_cast< I64 >( &val ) ); }
  }  // namespace SSE2
  namespace SSE {
  }
  namespace SSE3 {
  }
  namespace SSE3 {
  }
}  // namespace SIMD
