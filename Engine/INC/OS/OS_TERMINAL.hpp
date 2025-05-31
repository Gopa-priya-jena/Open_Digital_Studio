#include <cstdint>
#include <string_view>

#include <DATA_STRUCTURE/Types.hpp>

namespace OS

{

  namespace TERMINAL
  {

    struct color
    {
      uint8_t R, G, B;
    };
    enum Type : U32
    {
      BOLD = 1,
      ITALIC = 3,
      UNDERLINE = 4,
      STRIKETHROUGH = 9,
    };

    void OS_console_out( std::string_view str, color Foreground,
                         color Background, Type type );

  } // namespace TERMINAL
} // namespace OS
