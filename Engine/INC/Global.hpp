#pragma once
#include <OS/OS.hpp>
#include <format>
#include <string_view>
// some predefined color terminals
constexpr OS::TERMINAL::color RED{255, 0, 0},  // red
    GREEN{0, 255, 0},                          // green
    BLUE{0, 0, 255},                           // blue
    BLACK{0, 0, 0},                            // black
    violet{143, 0, 255},                       // violete
    YELLOW{255, 255, 0},                       // violete
    VividRed{255, 0, 0},                       //
    BrightOrange{255, 127, 0},                 //
    SunsetYellow{255, 215, 0},                 //
    LimeGreen{50, 205, 50},                    //
    ElectricGreen{0, 255, 0},                  //
    Turquoise{64, 224, 208},                   //
    Cyan{0, 255, 255},                         //
    SkyBlue{135, 206, 235},                    //
    RoyalBlue{65, 105, 225},                   //
    VividViolet{138, 43, 226},                 //
    Magenta{255, 0, 255},                      //
    HotPink{255, 105, 180},                    //
    DeepPink{255, 20, 147},                    //
    GoldenYellow{255, 223, 0},                 //
    Tangerine{255, 160, 0},                    //
    CrimsonRed{220, 20, 60},                   //
    FireBrick{178, 34, 34},                    //
    TomatoRed{255, 99, 71},                    //
    SalmonPink{250, 128, 114},                 //
    Coral{255, 127, 80},                       //
    Peach{255, 218, 185},                      //
    Apricot{255, 205, 178},                    //
    LemonYellow{255, 247, 0},                  //
    Chartreuse{127, 255, 0},                   //
    SpringGreen{0, 255, 127},                  //
    SeaGreen{46, 139, 87},                     //
    ForestGreen{34, 139, 34},                  //
    EmeraldGreen{0, 134, 139},                 //
    Teal{0, 128, 128},                         //
    Aquamarine{127, 255, 212},                 //
    AzureBlue{0, 127, 255},                    //
    DodgerBlue{30, 144, 255},                  //
    CornflowerBlue{100, 149, 237},             //
    Indigo{75, 0, 130},                        //
    Purple{128, 0, 128},                       //
    Fuchsia{255, 0, 255},                      //
    Orchid{218, 112, 214},                     //
    Plum{221, 160, 221},                       //
    Maroon{128, 0, 0},                         //
    DarkRed{139, 0, 0},                        //
    RubyRed{224, 17, 95},                      //
    Rose{255, 0, 127},                         //
    Blush{255, 182, 193},                      //
    MintGreen{189, 252, 201},                  //
    Lavender{230, 230, 250},                   //
    BabyBlue{137, 207, 240},                   //
    PowderBlue{176, 224, 230},                 //
    SteelBlue{70, 130, 180};                   //

//
constexpr OS::TERMINAL::Type Bold = OS::TERMINAL::Type::BOLD;
constexpr OS::TERMINAL::Type Italic = OS::TERMINAL::Type::ITALIC;
constexpr OS::TERMINAL::Type Underline = OS::TERMINAL::Type::UNDERLINE;
constexpr OS::TERMINAL::Type strikeout = OS::TERMINAL::Type::STRIKETHROUGH;

inline void ALERT(std::string_view msg)
{
    OS::TERMINAL::OS_console_out(msg, VividRed, BLACK, Bold);
}

// inline void prnt(std::string_view msg)
// {
//     OS::TERMINAL::OS_console_out(
//         std::format("\n printing \t{}\n", msg), Plum, BLACK, Bold);
// }
