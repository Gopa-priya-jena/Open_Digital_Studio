#pragma once
#include <Types.hpp> 
#include <bitset>
#include <string_view>

namespace DATA_STRUCTURE {
namespace BITS {

template <U64 N> class BitBools {
public:
  std::bitset<N> _config;

public:
  BitBools() { clear_all(); }
  // auto operator->( U64 member )
  // {
  //   return _config[ member ]
  // }
  inline constexpr void set_all() { _config.set(); }
  inline constexpr void clear_all() { _config.reset(); }
  inline constexpr void toggle_all() { _config.flip(); }

  inline constexpr void toggle(U64 member) { _config.flip(member); }

  inline constexpr auto get(U64 member) { return _config[member]; }

  inline constexpr auto operator[](U64 member) { return _config[member]; }
};
consteval U64 count_enum_member(std::string_view data) {
  U64 count = 0;
  bool alpha = false;
  bool comma = false;
  bool start = false;
  bool end = false;
  for (auto i = 0; i < data.size(); i++) {
    if (end) {
      break;
    }

    if (!start) {
      if (data[i] == '{') {
        start = true;
      }
    } else {
      std::uint8_t num = static_cast<std::uint8_t>(data[i]);
      if ((num >= 48 && num <= 57) || (num >= 65 && num <= 90) ||
          (num >= 97 && num <= 122)) {
        alpha = true;
      }
      if (data[i] == ',') {
        comma = true;
      }
      if (data[i] == '}') {
        comma = true;
        end = true;
      }
      if (alpha && comma) {
        comma = alpha = false;
        count++;
      }
    }
  }
  return count;
}
#define BITBOOLS(...)                                                          \
  __VA_ARGS__;                                                                 \
  inline DATA_STRUCTURE::BITS::BitBools<([]() -> U64 {                         \
    constexpr std::string_view constexpr_input_string = #__VA_ARGS__;          \
    return DATA_STRUCTURE::BITS::count_enum_member(constexpr_input_string);    \
  })()>

} // namespace BITS
} // namespace DATA_STRUCTURE
