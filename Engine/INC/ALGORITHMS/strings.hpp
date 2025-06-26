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

    U64 String_To_Bytes( std::string_view Str );
    // std::tuple< std::vector< U32 >, bool > Knuth_Morris_Pratt( std::string_view Search_space, std::string_view elements );

    // bool Aho_Corasick( std::string_view Search_Space, bool *checklist, std::initializer_list< std::string_view > elements );

    std::string_view get_line_with( std::string_view str, std::string_view &search_space );
    std::string_view getlast_stringwith( char check, std::string_view data );

    F32 getlast_numberf( std::string_view data, char check );
    uint64_t getlast_number( std::string_view data, char check = ' ' );
    // bool find_substring( std::string_view Search_Space, bool *checklist, std::initializer_list< std::string_view > elements );

    // a better solution
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

      void set_begin();

      void set_end();
      Token_iterator( std::string_view search_space, std::string_view search );

      bool end();
      bool begin();
    };

    I64 Str_to_int( std::string_view view );
    F32 Str_to_F32( std::string_view view );

  }  // namespace STRING

}  // namespace ALGORITHM
// #endif
