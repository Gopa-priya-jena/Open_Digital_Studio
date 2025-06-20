#pragma once
#include <DATA_STRUCTURE/SIMD/SIMD_helper_types.hpp>
#include <Types.hpp>

#include "ALGORITHMS/SIMD/ARCH/X64/SSE/Types.hpp"

namespace SIMD {

  namespace AVX {

    F_INLINE inline constexpr VF256 BLDA_V128_X2( V128& v128 ) { return _mm256_broadcast_ps( &v128 ); }
    F_INLINE inline constexpr VF256 BLDU_F64_X4( F64& f64 ) { return _mm256_broadcast_sd( &f64 ); }
    F_INLINE inline constexpr VF128 BLDU_F32_X4( F32& f32 ) { return _mm_broadcast_ss( &f32 ); }
    F_INLINE inline constexpr VF256 BLDU_F32_X8( F32& f32 ) { return _mm256_broadcast_ss( &f32 ); }
    F_INLINE inline constexpr VI256 LDU_VI256_X1( VI256& i256 ) { return _mm256_lddqu_si256( &i256 ); }
    F_INLINE inline constexpr VI256 LDA_VI256_X1( VI256& i256 ) { return _mm256_load_si256( &i256 ); }
    F_INLINE inline constexpr VF256 LDA_F64_X4( F64& f64 ) { return _mm256_load_pd( &f64 ); }
    F_INLINE inline constexpr VF256 LDU_F64_X4( F64& f64 ) { return _mm256_loadu_pd( &f64 ); }
    F_INLINE inline constexpr VF256 LDA_F32_X8( F32& f32 ) { return _mm256_load_ps( &f32 ); }
    F_INLINE inline constexpr VF256 LDU_F32_X8( F32& f32 ) { return _mm256_loadu_ps( &f32 ); }
    F_INLINE inline constexpr VF128 FLDA_F64_X2( F64& f64, Mask< VI128, 64 > mask ) { return _mm_maskload_pd( &f64, mask.value() ); }
    F_INLINE inline constexpr VF256 FLDA_F64_X4( F64& f64, Mask< VI256, 64 > mask ) { return _mm256_maskload_pd( &f64, mask.value() ); }
    F_INLINE inline constexpr VF128 FLDA_F32_X4( F32& f32, Mask< VI128, 32 > mask ) { return _mm_maskload_ps( &f32, mask.value() ); }
    F_INLINE inline constexpr VF256 FLDA_F64_X8( F32& f32, Mask< VI256, 32 > mask ) { return _mm256_maskload_ps( &f32, mask.value() ); }

    // _mm256_broadcast_pd _mm256_broadcast_ps

  }  // namespace AVX

  namespace AVX2 {

    F_INLINE inline constexpr VI128 LDU_I32_X4( I32& Base_adrr, VIndex< VI128, I32 >& index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i32gather_epi32( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i32gather_epi32( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i32gather_epi32( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i32gather_epi32( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI128 FLDU_I32_X4( I32& Base_adrr, Mask< VI128, 32 > mask, VIndex< VI128, I32 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VI256 LDU_I32_X8( I32& Base_adrr, VIndex< VI256, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i32gather_epi32( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i32gather_epi32( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i32gather_epi32( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i32gather_epi32( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI256 FLDU_I32_X8( I32& Base_adrr, Mask< VI256, 32 > mask, VIndex< VI256, I32 > index, scale_selection scale, VI256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i32gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    // ---------------------------------- i64
    F_INLINE inline constexpr VI128 LDU_I64_X2( I64& Base_adrr, VIndex< VI128, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i32gather_epi64( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i32gather_epi64( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i32gather_epi64( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i32gather_epi64( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI128 FLDU_I64_X2( I64& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I32 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VI256 LDU_I64_X4( I64& Base_adrr, VIndex< VI128, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i32gather_epi64( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i32gather_epi64( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i32gather_epi64( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i32gather_epi64( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI256 FLDU_I64_X4( I64& Base_adrr, Mask< VI256, 64 > mask, VIndex< VI128, I32 > index, scale_selection scale, VI256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i32gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    F_INLINE inline constexpr VF128 LDU_F64_X2( F64& Base_adrr, VIndex< VI128, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i32gather_pd( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i32gather_pd( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i32gather_pd( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i32gather_pd( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF128 FLDU_F64_X2( F64& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I32 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VF256 LDU_F64_X4( F64& Base_adrr, VIndex< VI128, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i32gather_pd( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i32gather_pd( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i32gather_pd( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i32gather_pd( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF256 FLDU_F64_X4( F64& Base_adrr, Mask< VI256, 64 > mask, VIndex< VI128, I32 > index, scale_selection scale, VF256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i32gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    //-------------------------f32

    F_INLINE inline constexpr VF128 LDU_F32_X4( F32& Base_adrr, VIndex< VI128, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i32gather_ps( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i32gather_ps( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i32gather_ps( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i32gather_ps( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF128 FLDU_F32_X4( F32& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I32 > index, scale_selection scale, V128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VF256 LDU_F32_X8( F32& Base_adrr, VIndex< VI256, I32 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i32gather_ps( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i32gather_ps( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i32gather_ps( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i32gather_ps( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF256 FLDU_F32_X8( F32& Base_adrr, Mask< VI256, 64 > mask, VIndex< VI256, I32 > index, scale_selection scale, V256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i32gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    // gather 64

    F_INLINE inline constexpr VI128 LDU_I32_X2_L01( I32& Base_adrr, VIndex< VI128, I64 >& index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i64gather_epi32( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i64gather_epi32( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i64gather_epi32( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i64gather_epi32( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI128 FLDU_I32_X2_L01( I32& Base_adrr, Mask< VI128, 32 > mask, VIndex< VI128, I64 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VI128 LDU_I32_X4_L0123( I32& Base_adrr, VIndex< VI256, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i64gather_epi32( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i64gather_epi32( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i64gather_epi32( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i64gather_epi32( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI128 FLDU_I32_X4_L0123( I32& Base_adrr, Mask< VI128, 32 > mask, VIndex< VI256, I64 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i64gather_epi32( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    // ---------------------------------- i64
    F_INLINE inline constexpr VI128 LDU_I64_X2( I64& Base_adrr, VIndex< VI128, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i64gather_epi64( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i64gather_epi64( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i64gather_epi64( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i64gather_epi64( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI128 FLDU_I64_X2( I64& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I64 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VI256 LDU_I64_X2_L01( I64& Base_adrr, VIndex< VI256, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i64gather_epi64( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i64gather_epi64( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i64gather_epi64( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i64gather_epi64( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VI256 FLDU_I64_X2_L01( I64& Base_adrr, Mask< VI256, 64 > mask, VIndex< VI256, I64 > index, scale_selection scale, VI256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i64gather_epi64( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    F_INLINE inline constexpr VF128 LDU_F64_X2( F64& Base_adrr, VIndex< VI128, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i64gather_pd( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i64gather_pd( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i64gather_pd( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i64gather_pd( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF128 FLDU_F64_X2( F64& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I64 > index, scale_selection scale, VI128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VF256 LDU_F64_X4( F64& Base_adrr, VIndex< VI256, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i64gather_pd( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i64gather_pd( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i64gather_pd( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i64gather_pd( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF256 FLDU_F64_X4( F64& Base_adrr, Mask< VI256, 64 > mask, VIndex< VI256, I64 > index, scale_selection scale, VF256 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i64gather_pd( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    //-------------------------f32

    F_INLINE inline constexpr VF128 LDU_F32_X2_L01( F32& Base_adrr, VIndex< VI128, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm_i64gather_ps( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm_i64gather_ps( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm_i64gather_ps( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm_i64gather_ps( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF128 FLDU_F32_X2_L01( F32& Base_adrr, Mask< VI128, 64 > mask, VIndex< VI128, I64 > index, scale_selection scale, V128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }

    F_INLINE inline constexpr VF128 LDU_F32_X4( F32& Base_adrr, VIndex< VI256, I64 > index, scale_selection scale )
    {
      switch ( scale ) {

        case scale_selection::x1: return _mm256_i64gather_ps( &Base_adrr, index.Index_List, 1 );
        case scale_selection::x2: return _mm256_i64gather_ps( &Base_adrr, index.Index_List, 2 );
        case scale_selection::x4: return _mm256_i64gather_ps( &Base_adrr, index.Index_List, 4 );
        case scale_selection::x8: return _mm256_i64gather_ps( &Base_adrr, index.Index_List, 8 );
      }
    }

    F_INLINE inline constexpr VF128 FLDU_F32_X4_L0123( F32& Base_adrr, Mask< V128, 64 > mask, VIndex< VI256, I64 > index, scale_selection scale, V128 src )
    {
      switch ( scale ) {
        case scale_selection::x1: return _mm256_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 1 );
        case scale_selection::x2: return _mm256_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 2 );
        case scale_selection::x4: return _mm256_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 4 );
        case scale_selection::x8: return _mm256_mask_i64gather_ps( src, &Base_adrr, index.Index_List, mask.value(), 8 );
      }
    }
    F_INLINE inline constexpr VI128 FLDA_I32_X4( I32& Base_adrr, Mask< VI128, 32 > mask ) { return _mm_maskload_epi32( &Base_adrr, mask.value() ); }
    F_INLINE inline constexpr VI256 FLDA_I32_X8( I32& Base_adrr, Mask< VI256, 32 > mask ) { return _mm256_maskload_epi32( &Base_adrr, mask.value() ); }
    F_INLINE inline constexpr VI128 FLDA_I64_X2( I64& Base_adrr, Mask< VI128, 64 > mask ) { return _mm_maskload_epi64( reinterpret_cast< long long* >( &Base_adrr ), mask.value() ); }
    F_INLINE inline constexpr VI256 FLDA_I64_X4( I64& Base_adrr, Mask< VI256, 64 > mask ) { return _mm256_maskload_epi64( reinterpret_cast< long long* >( &Base_adrr ), mask.value() ); }
    F_INLINE inline constexpr V256 SLDA( void* Base_adrr ) { return _mm256_stream_load_si256( Base_adrr ); }

  }  // namespace AVX2
}  // namespace SIMD
