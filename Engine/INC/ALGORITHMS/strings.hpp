#pragma once
#include <Types.hpp>

// #if 0
#include <DATA_STRUCTURE/units.hpp>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string_view>
#include <vector>
namespace ALGORITHM {
  namespace STRING {

    class Token_iterator {
      std::string_view _search_space;
      std::bitset< 256 > char_test{ 0 };
      std::size_t Index;

      inline std::size_t empty_ch_r( std::size_t &start );
      inline std::size_t empty_ch_f( std::size_t &start );
      inline std::size_t Tkn_end( std::size_t &start );

      inline std::size_t Tkn_start( std::size_t &start );

     public:
      std::string_view next_token();
      std::string_view prev_token();
      std::string_view next_token( std::string_view ele );
      std::string_view prev_token( std::string_view ele );

      std::string_view next_token_p( std::string_view ele );
      std::string_view prev_token_p( std::string_view ele );

      void set_begin();
      void set_end();

      Token_iterator( std::string_view search_space, std::string_view search );
      Token_iterator( std::string_view search_space, std::bitset< 256 > test );

      bool end();
      bool begin();
    };

    U64 Str_to_uint( std::string_view view );
    F32 Str_to_F32( std::string_view view );
    U64 Str_to_mem_size( std::string_view sv );
  }  // namespace STRING

}  // namespace ALGORITHM
// #endif
