#include <OS/OS_TERMINAL.hpp>
#include <iostream>
namespace OS
{

namespace TERMINAL

{

// some global varibales
constexpr std::string_view Escape("\033"), CSI("\x9B"), FOREGROUND("38;2;"),
    BACKGROAUND("48;2;");

void OS_console_out(std::string_view str,
                    color            Foreground,
                    color            Background,
                    Type             type)
{
    // FUNCTION_PROFILE;

    std::cout << std::format("{}[{};{}{};{};{};{}{};{};{}m{}{}[0m",
                             Escape,
                             static_cast<I32>(type),
                             FOREGROUND,
                             Foreground.R,
                             Foreground.G,
                             Foreground.B,
                             BACKGROAUND,
                             Background.R,
                             Background.G,
                             Background.B,
                             str,
                             Escape)
              << std::flush;
}

}  // namespace TERMINAL

}  // namespace OS
