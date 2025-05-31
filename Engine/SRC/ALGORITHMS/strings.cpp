#include <ALGORITHMS/strings.hpp>
#include <DATA_STRUCTURE/Map.hpp>
#include <DATA_STRUCTURE/Types.hpp>
#include <DEBUG.hpp>
#include <cmath>
namespace ALGORITHM
{
  namespace STRING
  {

    void skip_type( std::string_view &str, uint64_t &index,
                    I32 ( *check )( I32 ) )
    {
      while ( ( index < str.length() ) && ( check( str[ index ] ) ) )
      {
        index++;
      }
      index--;
    }

    void skip_space( std::string_view &str, uint64_t &index )
    {
      skip_type( str, index, std ::isspace );
    }
    void skip_digit( std::string_view &str, uint64_t &index )
    {
      skip_type( str, index, std ::isdigit );
    }
    void skip_char( std::string_view &str, uint64_t &index )
    {
      skip_type( str, index, std ::isalpha );
    }
    void skip_alpha_num( std::string_view &str, uint64_t &index )
    {
      skip_type( str, index, std ::isalnum );
    }
    void skip_till( std::string_view &str, uint64_t &index, char ch )
    {
      auto start = index;

      while ( ( index < str.length() ) && ( ch != ( str[ index ] ) ) )
      {
        index++;
      }
      if ( ch == ( str[ index ] ) )
        index = start;
      else
        index--;
    }
    void skip_till( std::string_view &str, uint64_t &index,
                    I32 ( *check )( I32 ) )
    {
      auto start = index;

      while ( ( index < str.length() ) && ( !check( str[ index ] ) ) )
      {
        index++;
      }
      if ( index == str.length() )
        index = start;
      else
        index--;
    }
    U64 String_To_Bytes( std::string_view Str )
    {
      FUNCTION_PROFILE_ARG( Str );

      if ( Str.length() == 0 )
        return 0;

      uint64_t Bytes{ 0 };
      uint64_t shift = 0;
      uint64_t pos = 0;
      uint64_t start = 0;

      skip_till( Str, pos, std::isdigit );

      pos++;
      start = pos;
      skip_digit( Str, pos );

      char byte[ 22 ];
      Str.copy( byte, pos - start + 1, start );
      Bytes = atoll( byte );
      while ( pos < Str.length() )
      {
        switch ( Str[ pos ] )
        {
        case 'b':

        case 'B':
        {
          shift = static_cast<U8>( Binary_Storage::Bytes );
          break;
        }

        case 'k':

        case 'K':
        {
          shift = static_cast<U8>( Binary_Storage::KB );
          break;
        }
        case 'm':

        case 'M':
        {
          shift = static_cast<U8>( Binary_Storage::MB );
          break;
        }
        case 'g':

        case 'G':
        {
          shift = static_cast<U8>( Binary_Storage::GB );
          break;
        }
        case 't':

        case 'T':
        {
          shift = static_cast<U8>( Binary_Storage::TB );
          break;
        }
        case 'p':

        case 'P':
        {
          shift = static_cast<U8>( Binary_Storage::PB );
          break;
        }
        }
        pos++;
      }
      LOG( "\n DATA->\t", Str, "\tBytes\t", Bytes, "\t shift \t", shift,
           "\tVALUE\t", Bytes << shift )
      return ( Bytes << shift );
    }

    std::string_view get_line_with( std::string_view  str,
                                    std::string_view &search_space )
    {
      // FUNCTION_PROFILE_ARG(str.data(), search_space.data());

      auto start_pos = 0ULL;
      auto end_pos = 0ULL;
      auto search_newline = [ & ]( U64 pos ) -> std::string_view
      {
        start_pos = pos;
        end_pos = 0;
        while ( search_space[ pos++ ] != '\n' )
        {
          end_pos++;
        }

        return search_space.substr( start_pos, end_pos );
      };

      auto check = [ & ]( std::string_view &search ) -> bool
      {
        if ( search.find( str, 0 ) == std::string_view::npos )
          return false;
        else
          return true;
      };

      auto i = 0;
      while ( i < search_space.length() )
      {
        std::string_view data = search_newline( i );
        if ( check( data ) )
          return data;
        i += ( end_pos + 1 );
      }
      return {};
    }
    std::string_view getlast_stringwith( char check, std::string_view data )
    {
      FUNCTION_PROFILE_ARG( check, data );

      auto pos = data.rfind( check );
      return ( data.substr( pos + 1 ) );
    };

    uint64_t getlast_number( std::string_view data, char check )
    {
      FUNCTION_PROFILE_ARG( data, check );

      auto     res = getlast_stringwith( check, data );
      char     temp[ 30 ];
      uint64_t pos = 0;
      auto     last_pos = res.copy( temp, res.length(), pos );
      temp[ last_pos ] = '\0';
      LOG( "DATA\t", data, "\tchar\t", temp, "\tnumber\t",
           std::to_string( atoll( temp ) ) );
      return atoll( temp );
    }

    F32 getlast_numberf( std::string_view data, char check )
    {
      FUNCTION_PROFILE_ARG( data, check );

      auto     res = getlast_stringwith( check, data );
      char     temp[ 30 ];
      uint64_t pos = 0;
      auto     last_pos = res.copy( temp, res.length(), pos );
      temp[ last_pos ] = '\0';
      LOG( "DATA\t", data, "\tchar\t", temp, "\tnumber\t",
           std::to_string( atof( temp ) ) );
      return atof( temp );
    }

    bool find_substring( std::string_view Search_Space, bool *checklist,
                         std::initializer_list<std::string_view> elements )
    {
      const char *check_array[ elements.size() ];
      char        map[ elements.size() ];
      bool        state[ elements.size() ];
      short       map_list[ elements.size() + 100 ];
      char        xorbox;
      char        xorbox_copy;

      // initializing all check array and boolean table

      if ( elements.size() == 1 )
      {
        if ( std::get<1>(
                 Knuth_Morris_Pratt( Search_Space, *elements.begin() ) ) )
        {
          return true;
        }
      }
      else
      {
        return Aho_Corasick( Search_Space, checklist, elements );
      }
    }
    //

    inline void prefix_lps( U8 *lps_array, std::string_view elements )
    {
      lps_array[ 0 ] = 0;
      U8 index = 0;
      U8 search_size = 0;

      for ( index = 1; index < elements.size(); )
      {
        if ( elements[ index ] == elements[ search_size ] )
        {
          search_size++;
          lps_array[ index ] = search_size;
          index++;
        }
        else
        {
          if ( search_size != 0 )
          {
            search_size = lps_array[ search_size - 1 ];
          }
          else
          {
            lps_array[ index ] = 0;
            index++;
          }
        }
      }
    }

    std::tuple<std::vector<U32>, bool>
    Knuth_Morris_Pratt( std::string_view Search_space,
                        std::string_view element )
    {
      U8               LPS[ element.size() ];
      std::vector<U32> list;
      prefix_lps( LPS, element );
      bool found = 0;
      // searching for pattern
      for ( U8 i = 0, j = 0; i < Search_space.size(); )
      {
        if ( Search_space[ i ] == element[ i ] )
        {
          i++;
          j++;

          if ( j == element.size() )
          {
            found = true;
            list.push_back( i - j );
          }
          else
          {
            if ( j != 0 )
            {
              j = LPS[ j - 1 ];
            }
            else
              i++;
          }
        }
      }
      return { list, found };
    }

    bool Aho_Corasick( std::string_view Search_Space, bool *checklist,
                       std::initializer_list<std::string_view> elements )
    {
      return false;
    }

    Token_iterator::Token_iterator( std::string_view search_space,
                                    std::string_view search )
    {
      for ( auto i : search )
      {
        char_test[ U8( i ) ] = 1;
      }
      _search_space = search_space;
      Index = 0;
    }

    bool Token_iterator::end()
    {
      if ( Index >= _search_space.size() )
      {
        Index--;
        return true;
      }
      else if ( Index == _search_space.size() )
      {

        return true;
      }
      else
      {
        return false;
      }
    }
    bool Token_iterator::begin()
    {
      return ( Index == 0 );
    }

    std::size_t Token_iterator::empty_ch_f( std::size_t &start )
    {
      std::size_t i = start;
      while ( i < _search_space.size() &&
              char_test[ ( U8 )_search_space[ i ] ] )
      {
        i++;
      }

      return i;
    }
    std::size_t Token_iterator::empty_ch_r( std::size_t &start )
    {
      I64 i = start;
      while ( i != 0 && ( char_test[ ( U8 )_search_space[ i ] ] ) )
      {
        i--;
      }

      if ( i == -1 ) [[unlikely]]
        i = 0;
      return i;
    }
    std::size_t Token_iterator::Tkn_end( std::size_t &start )
    {
      std::size_t i = start;
      while ( i < _search_space.size() &&
              !char_test[ ( U8 )_search_space[ i ] ] )
      {
        i++;
      };
      return i;
    }
    std::size_t Token_iterator::Tkn_start( std::size_t &start )
    {
      I64 i = start;

      while ( i >= 0 && !char_test[ ( U8 )_search_space[ i ] ] )
      {
        i--;
      };
      if ( i == -1 ) [[unlikely]]
        i = 0;
      return i;
    }

    std::string_view Token_iterator::next_token()
    {
      if ( Index > _search_space.size() ) [[unlikely]]
        return "";
      auto i = empty_ch_f( Index );
      Index = Tkn_end( i );
      // std::cout << "Index \t" << Index << std::endl;
      return _search_space.substr( i, Index - i );
    }
    std::string_view Token_iterator::prev_token()
    {
      if ( Index == 0 ) [[unlikely]]
        return "";
      auto i = empty_ch_r( Index );
      Index = Tkn_start( i );
      auto pos = 0;
      auto end_size = 0;
      if ( Index == 0 )
      {
        pos = 0;
        end_size = i - Index + 1;
      }
      else
      {
        pos = Index + 1;
        end_size = i - Index;
      }
      return _search_space.substr( pos, end_size );
    }

    void Token_iterator::set_begin()
    {

      Index = 0;
    }

    void Token_iterator::set_end()
    {

      Index = _search_space.size() - 1;
    }
    std::string_view Token_iterator::next_token( std::string_view ele )
    {
      bool flag = true;
      set_begin();
      while ( flag && !end() )
      {
        if ( ele == next_token() )
        {
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

      bool flag = true;
      set_end();
      while ( flag && !begin() )
      {

        if ( ele == prev_token() )
        {
          flag = false;
        }
      }
      if ( flag == 0 )
        return prev_token();
      else
        return "";
    }
    I64 Str_to_int( std::string_view view )
    {
      FMap<char, I32, 10> Number_Map;
      Number_Map.fill( '0', 0, 0 );
      Number_Map.fill( '9', 9, 1 );
      Number_Map.fill( '8', 8, 2 );
      Number_Map.fill( '7', 7, 3 );
      Number_Map.fill( '6', 6, 4 );
      Number_Map.fill( '5', 5, 5 );
      Number_Map.fill( '4', 4, 6 );
      Number_Map.fill( '3', 3, 7 );
      Number_Map.fill( '2', 2, 8 );
      Number_Map.fill( '1', 1, 9 );

      I64 result = 0;
      for ( auto i : view )
      {
        result *= 10;
        result += Number_Map[ i ];
      }
      return result;
    }
    F32 Str_to_F32( std::string_view view )
    {

      F32 decimal{ 0 }, frac{ 0 };

      Token_iterator tk( view, "." );

      decimal = Str_to_int( tk.next_token() );
      auto frac_str = tk.next_token();
      auto frac_size = frac_str.size();
      frac = Str_to_int( frac_str );
      F32 res = decimal;
      res += frac / std::pow( 10, frac_size );
      return res;
    }

  } // namespace STRING
} // namespace ALGORITHM
