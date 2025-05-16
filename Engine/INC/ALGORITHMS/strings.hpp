#include <DATA_STRUCTURE/units.hpp>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <string_view>
// #include <variant>
#include <vector>
namespace ALGORITHM {
namespace STRING {

std::uint64_t String_To_Bytes(std::string_view Str);
std::tuple<std::vector<unsigned int>, bool>
Knuth_Morris_Pratt(std::string_view Search_space, std::string_view elements);

bool Aho_Corasick(std::string_view Search_Space, bool *checklist,
                  std::initializer_list<std::string_view> elements);

std::string_view get_line_with(std::string_view str,
                               std::string_view &search_space);
std::string_view getlast_stringwith(char check, std::string_view data);

float getlast_numberf(std::string_view data, char check);
uint64_t getlast_number(std::string_view data, char check = ' ');
bool find_substring(std::string_view Search_Space, bool *checklist,
                    std::initializer_list<std::string_view> elements);

} // namespace STRING

} // namespace ALGORITHM
