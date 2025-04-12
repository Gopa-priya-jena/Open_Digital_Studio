#include <cstdint>
#include <string_view>
namespace OS

{

namespace TERMINAL
{

struct color
{
    uint8_t R, G, B;
};
enum Type : unsigned int
{
    BOLD = 1,
    ITALIC = 3,
    UNDERLINE = 4,
    STRIKETHROUGH = 9,

};

void OS_console_out(std::string_view str,
                    color            Foreground,
                    color            Background,
                    Type             type);

}  // namespace TERMINAL
}  // namespace OS
