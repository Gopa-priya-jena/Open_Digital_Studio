#include <strings.h>

#include <ALGORITHMS/strings.hpp>
#include <DEBUG.hpp>
#include <Global.hpp>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <string_view>
namespace ALGORITHM {
namespace STRING {

void skip_type(std::string_view &str, uint64_t &index, int (*check)(int)) {
  while ((index < str.length()) && (check(str[index]))) {
    index++;
  }
  index--;
}

void skip_space(std::string_view &str, uint64_t &index) {
  skip_type(str, index, std ::isspace);
}
void skip_digit(std::string_view &str, uint64_t &index) {
  skip_type(str, index, std ::isdigit);
}
void skip_char(std::string_view &str, uint64_t &index) {
  skip_type(str, index, std ::isalpha);
}
void skip_alpha_num(std::string_view &str, uint64_t &index) {
  skip_type(str, index, std ::isalnum);
}
void skip_till(std::string_view &str, uint64_t &index, char ch) {
  auto start = index;

  while ((index < str.length()) && (ch != (str[index]))) {
    index++;
  }
  if (ch == (str[index]))
    index = start;
  else
    index--;
}
void skip_till(std::string_view &str, uint64_t &index, int (*check)(int)) {
  auto start = index;

  while ((index < str.length()) && (!check(str[index]))) {
    index++;
  }
  if (index == str.length())
    index = start;
  else
    index--;
}
std::uint64_t String_To_Bytes(std::string_view Str) {
  FUNCTION_PROFILE_ARG(Str.data());

  if (Str.length() == 0)
    return 0;

  uint64_t Bytes{0};
  uint64_t shift = 0;
  uint64_t pos = 0;
  uint64_t start = 0;

  skip_till(Str, pos, std::isdigit);

  pos++;
  start = pos;
  skip_digit(Str, pos);

  char byte[22];
  Str.copy(byte, pos - start + 1, start);
  Bytes = atoll(byte);
  while (pos < Str.length()) {
    switch (Str[pos]) {
    case 'b':

    case 'B': {
      shift = static_cast<uint>(Binary_Storage::Bytes);
      break;
    }

    case 'k':

    case 'K': {
      shift = static_cast<uint>(Binary_Storage::KB);
      break;
    }
    case 'm':

    case 'M': {
      shift = static_cast<uint>(Binary_Storage::MB);
      break;
    }
    case 'g':

    case 'G': {
      shift = static_cast<uint>(Binary_Storage::GB);
      break;
    }
    case 't':

    case 'T': {
      shift = static_cast<uint>(Binary_Storage::TB);
      break;
    }
    case 'p':

    case 'P': {
      shift = static_cast<uint>(Binary_Storage::PB);
      break;
    }
    }
    pos++;
  }
  LOG("\n DATA->\t", Str, "\tBytes\t", Bytes, "\t shift \t", shift, "\tVALUE\t",
      Bytes << shift)
  return (Bytes << shift);
}

std::string_view get_line_with(std::string_view str,
                               std::string_view &search_space) {
  // FUNCTION_PROFILE_ARG(str.data(), search_space.data());

  auto start_pos = 0ULL;
  auto end_pos = 0ULL;
  auto search_newline = [&](std::uint64_t pos) -> std::string_view {
    start_pos = pos;
    end_pos = 0;
    while (search_space[pos++] != '\n') {
      end_pos++;
    }

    return search_space.substr(start_pos, end_pos);
  };

  auto check = [&](std::string_view &search) -> bool {
    if (search.find(str, 0) == std::string_view::npos)
      return false;
    else
      return true;
  };

  auto i = 0;
  while (i < search_space.length()) {
    std::string_view data = search_newline(i);
    if (check(data))
      return data;
    i += (end_pos + 1);
  }
  return {};
}
std::string_view getlast_stringwith(char check, std::string_view data) {
  FUNCTION_PROFILE_ARG(static_cast<unsigned char>(check), data.data());

  auto pos = data.rfind(check);
  return (data.substr(pos + 1));
};

uint64_t getlast_number(std::string_view data, char check) {
  FUNCTION_PROFILE_ARG(data, static_cast<unsigned char>(check));

  auto res = getlast_stringwith(check, data);
  char temp[30];
  uint64_t pos = 0;
  auto last_pos = res.copy(temp, res.length(), pos);
  temp[last_pos] = '\0';
  LOG("DATA\t", data, "\tchar\t", temp, "\tnumber\t",
      std::to_string(atoll(temp)));
  return atoll(temp);
}

float getlast_numberf(std::string_view data, char check) {
  FUNCTION_PROFILE_ARG(data, (char)check);

  auto res = getlast_stringwith(check, data);
  char temp[30];
  uint64_t pos = 0;
  auto last_pos = res.copy(temp, res.length(), pos);
  temp[last_pos] = '\0';
  LOG("DATA\t", data, "\tchar\t", temp, "\tnumber\t",
      std::to_string(atof(temp)));
  return atof(temp);
}

bool find_substring(std::string_view Search_Space, bool *checklist,
                    std::initializer_list<std::string_view> elements) {
  const char *check_array[elements.size()];
  char map[elements.size()];
  bool state[elements.size()];
  short map_list[elements.size() + 100];
  char xorbox;
  char xorbox_copy;

  // initializing all check array and boolean table

  if (elements.size() == 1) {
    if (std::get<1>(Knuth_Morris_Pratt(Search_Space, *elements.begin()))) {
      return true;
    }
  } else {
    return Aho_Corasick(Search_Space, checklist, elements);
  }
}
//

inline void prefix_lps(std::uint8_t *lps_array, std::string_view elements) {
  lps_array[0] = 0;
  std::uint8_t index = 0;
  std::uint8_t search_size = 0;

  for (index = 1; index < elements.size();) {
    if (elements[index] == elements[search_size]) {
      search_size++;
      lps_array[index] = search_size;
      index++;
    } else {
      if (search_size != 0) {
        search_size = lps_array[search_size - 1];
      } else {
        lps_array[index] = 0;
        index++;
      }
    }
  }
}

std::tuple<std::vector<unsigned int>, bool>
Knuth_Morris_Pratt(std::string_view Search_space, std::string_view element) {
  std::uint8_t LPS[element.size()];
  std::vector<unsigned int> list;
  prefix_lps(LPS, element);
  bool found = 0;
  // searching for pattern
  for (std::uint8_t i = 0, j = 0; i < Search_space.size();) {
    if (Search_space[i] == element[i]) {
      i++;
      j++;

      if (j == element.size()) {
        found = true;
        list.push_back(i - j);
      } else {
        if (j != 0) {
          j = LPS[j - 1];
        } else
          i++;
      }
    }
  }
  return {list, found};
}

bool Aho_Corasick(std::string_view Search_Space, bool *checklist,
                  std::initializer_list<std::string_view> elements) {
  return false;
}
} // namespace STRING
} // namespace ALGORITHM
