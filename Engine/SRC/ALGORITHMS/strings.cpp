// #if 0
#include <ALGORITHMS/strings.hpp>
#include <DATA_STRUCTURE/Map.hpp>
#include <DEBUG.hpp>
#include <Types.hpp>
#include <cctype>
#include <cmath>

namespace ALGORITHM {
  namespace STRING {

    Token_iterator::Token_iterator( std::string_view search_space, std::string_view search )
    {
      FUNCTION_PROFILE_ARG( search_space, search )
      for ( auto i : search ) {
        char_test[ U8( i ) ] = 1;
      }
      _search_space = search_space;
      Index         = 0;
    }

    Token_iterator::Token_iterator( std::string_view search_space, std::bitset< 256 > test )
    {
      FUNCTION_PROFILE_ARG( search_space )

      Index = 0;

      _search_space = search_space;
      char_test     = test;
    }
    bool Token_iterator::end()
    {
      if ( Index > _search_space.size() ) {
        Index--;
        return true;
      }
      else if ( Index == _search_space.size() ) {

        return true;
      }
      else {
        return false;
      }
    }
    bool Token_iterator::begin() { return ( Index == 0 ); }

    std::size_t Token_iterator::empty_ch_f( std::size_t &start )
    {
      std::size_t i = start;
      while ( i < _search_space.size() && char_test[ ( U8 )_search_space[ i ] ] ) {
        i++;
      }

      return i;
    }
    std::size_t Token_iterator::empty_ch_r( std::size_t &start )
    {
      FUNCTION_PROFILE
      I64 i = start;
      while ( i != 0 && ( char_test[ ( U8 )_search_space[ i ] ] ) ) {
        i--;
      }

      if ( i == -1 ) [[unlikely]]
        i = 0;
      return i;
    }
    std::size_t Token_iterator::Tkn_end( std::size_t &start )
    {
      std::size_t i = start;
      while ( i < _search_space.size() && !char_test[ ( U8 )_search_space[ i ] ] ) {
        i++;
      };
      return i;
    }
    std::size_t Token_iterator::Tkn_start( std::size_t &start )
    {
      I64 i = start;

      while ( i >= 0 && !char_test[ ( U8 )_search_space[ i ] ] ) {
        i--;
      };
      if ( i == -1 ) [[unlikely]]
        i = 0;
      return i;
    }

    std::string_view Token_iterator::next_token()
    {
      // FUNCTION_PROFILE
      if ( Index > _search_space.size() ) [[unlikely]]
        return "";
      auto i = empty_ch_f( Index );
      Index  = Tkn_end( i );
      // std::cout << "Index \t" << Index << std::endl;
      return _search_space.substr( i, Index - i );
    }
    std::string_view Token_iterator::prev_token()
    {
      if ( Index == 0 ) [[unlikely]]
        return "";
      auto i        = empty_ch_r( Index );
      Index         = Tkn_start( i );
      auto pos      = 0;
      auto end_size = 0;
      if ( Index == 0 ) {
        pos      = 0;
        end_size = i - Index + 1;
      }
      else {
        pos      = Index + 1;
        end_size = i - Index;
      }
      return _search_space.substr( pos, end_size );
    }

    void Token_iterator::set_begin()
    {
      // FUNCTION_PROFILE
      Index = 0;
    }

    void Token_iterator::set_end()
    {
      FUNCTION_PROFILE
      Index = _search_space.size() - 1;
    }
    std::string_view Token_iterator::next_token( std::string_view ele )
    {
      FUNCTION_PROFILE_ARG( ele )
      bool flag = true;
      set_begin();
      while ( flag && !end() ) {
        if ( ele == next_token() ) {
          flag = false;
        }
      }
      if ( flag == 0 )
        return next_token();
      else
        return "";
    }
    std::string_view Token_iterator::prev_token( std::string_view ele )
    {
      FUNCTION_PROFILE_ARG( ele )
      bool flag = true;
      set_end();
      while ( flag && !begin() ) {

        if ( ele == prev_token() ) {
          flag = false;
        }
      }
      if ( flag == 0 )
        return prev_token();
      else
        return "";
    }
    std::string_view Token_iterator::next_token_p( std::string_view ele )
    {
      FUNCTION_PROFILE_ARG( ele )

      Token_iterator tk( ele, char_test );
      tk.set_begin();

      auto next = next_token( tk.next_token() );
      for ( auto i = 0; !( tk.end() || end() ); ) {
        if ( next == tk.next_token() ) {
          next = next_token();
        }
        else {
          tk.set_begin();
          next = next_token( tk.next_token() );
        }
        // LOG_DEBUG( "next tk->", next )
      }
      if ( tk.end() ) {
        return next;
      }
      else {
        return "";
      }
    }
    std::string_view Token_iterator::prev_token_p( std::string_view ele )
    {
      FUNCTION_PROFILE_ARG( ele )

      Token_iterator tk( ele, char_test );
      tk.set_end();

      auto next = prev_token( tk.prev_token() );
      for ( auto i = 0; !tk.begin() && !begin(); ) {
        if ( next == tk.prev_token() ) {
          next = prev_token();
        }
        else {
          tk.set_end();
          next = prev_token( tk.prev_token() );
        }
      }
      if ( tk.begin() ) {
        return next_token();
      }
      else {
        return "";
      }
    }
    constexpr FMap< char, U8, 10 > Number_Map( { { '0', 0 }, { '9', 9 }, { '8', 8 }, { '7', 7 }, { '6', 6 }, { '5', 5 }, { '4', 4 }, { '3', 3 }, { '2', 2 }, { '1', 1 } } );
    U64 Str_to_uint( std::string_view view )
    {

      FUNCTION_PROFILE_ARG( view )
      // LOG_CRITICAL( view )
      I64 result = 0;
      for ( auto i : view ) {
        if ( std::isdigit( i ) ) {
          result *= 10;
          result += Number_Map[ i ];
        }
      }
      return result;
    }
    F32 Str_to_F32( std::string_view view )
    {

      FUNCTION_PROFILE_ARG( view )
      F32 decimal{ 0 }, frac{ 0 };

      Token_iterator tk( view, "." );

      decimal        = Str_to_uint( tk.next_token() );
      auto frac_str  = tk.next_token();
      auto frac_size = frac_str.size();
      frac           = Str_to_uint( frac_str );
      F32 res        = decimal;
      res += frac / std::pow( 10, frac_size );
      return res;
    }

    constexpr FMap< char, U8, 6 > unit_size( { { 'k', 10 }, { 'K', 10 }, { 'M', 20 }, { 'm', 20 }, { 'g', 30 }, { 'G', 30 } } );
    U64 Str_to_mem_size( std::string_view sv )
    {
      FUNCTION_PROFILE_ARG( sv )

      U64 num = 0;
      Token_iterator tk( sv, "0123456789 " );
      auto unit = ( tk.next_token() );
      // LOG_CRITICAL( unit );
      num = Str_to_uint( sv );
      if ( num == 0 ) return 0;
      auto shift_ = unit_size[ unit[ 0 ] ];
      num         = num << shift_;
      return num;
    }

  }  // namespace STRING
}  // namespace ALGORITHM
// #endif
