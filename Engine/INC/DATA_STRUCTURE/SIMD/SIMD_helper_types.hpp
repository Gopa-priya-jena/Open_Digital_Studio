#pragma once
#include <Types.hpp>
#include <bitset>
#include <concepts>
#include <initializer_list>
namespace SIMD {

  template < typename T >
  inline consteval U64 count_bits()
  {
    return ( sizeof( T ) * 8 );
  }

  template < typename T, typename t >
  concept Packing_Feasibility = requires {
    ( std::same_as< T, V128 > || std::same_as< T, VI128 > || std::same_as< T, V256 > || std::same_as< T, VI256 > );
    ( sizeof( T ) % sizeof( t ) ) == 0;
    sizeof( t ) != 0;
    std::is_trivially_copyable_v< t >;
  };
  template < typename T, U64 t >
  concept Packing_Feasibility_size = requires {
    ( std::same_as< T, V128 > || std::same_as< T, VI128 > || std::same_as< T, V256 > || std::same_as< T, VI256 > );
    ( sizeof( T ) % t ) == 0;
    sizeof( t ) != 0;
  };
  template < typename Type, typename Index_type >
    requires Packing_Feasibility< Type, Index_type >
  struct VIndex {
    Type Index_List{ 0 };
    Index_type *Index = nullptr;
    inline VIndex() { Index = reinterpret_cast< Index_type * >( &Index_List ); };
    inline VIndex( VIndex &v ) noexcept
    {
      Index_List = v.Index_List;
      Index      = reinterpret_cast< Index_type * >( &Index_List );
    }
    inline VIndex( VIndex &&v ) noexcept
    {
      Index_List = v.Index_List;
      Index      = reinterpret_cast< Index_type * >( &Index_List );
    }
  };
  enum scale_selection : U8 {
    x1,
    x2,
    x4,
    x8

  };
  class scale {
    U8 index{ 0 };
    const scale_selection _scale[ 4 ] = { scale_selection::x1, scale_selection::x2, scale_selection::x4, scale_selection::x8 };

   public:
    scale();
    inline constexpr void operator++()
    {
      if ( index != 3 ) index++;
    }
    inline constexpr void operator--()
    {
      if ( index != 0 ) index--;
    }
    inline constexpr void rotate()
    {
      index++;
      index = index % 4;
    }
    inline constexpr void set_index( U8 val )
    {
      // static_assert( ( ( val <= 3 ) && ( val >= 0 ) ), "value should between0 and 3 only" );
      index = val;
    }

    inline constexpr auto value() { return _scale[ index ]; }
    inline constexpr auto x1() { return _scale[ 0 ]; }
    inline constexpr auto x2() { return _scale[ 1 ]; }
    inline constexpr auto x4() { return _scale[ 2 ]; }
    inline constexpr auto x8() { return _scale[ 3 ]; }
  };

  template < typename Mask_Type, U64 Lane_size >
    requires Packing_Feasibility_size< Mask_Type, Lane_size >
  class Mask {
   private:
    std::bitset< count_bits< Mask_Type >() > mask;

   public:
    Mask_Type value() { return reinterpret_cast< Mask_Type * >( &mask ); }
    inline constexpr Mask() { mask.reset(); }
    inline constexpr explicit Mask( Mask_Type mask_val ) { reinterpret_cast< Mask_Type * >( &mask ) = mask_val; }
    inline constexpr Mask( std::initializer_list< bool > L_H )
    {
      mask.reset();
      auto i = Lane_size - 1;
      for ( auto j = L_H.begin(); ( i < count_bits< Mask_Type >() ) && ( j != L_H.end() ); i += Lane_size, j++ ) {
        if ( *j ) mask[ i ] = 1;
      }
    }

    inline constexpr Mask( Mask &m ) { mask = m.mask; }
    inline constexpr Mask( Mask &&m ) noexcept { mask = m.mask; }
  };

}  // namespace SIMD
