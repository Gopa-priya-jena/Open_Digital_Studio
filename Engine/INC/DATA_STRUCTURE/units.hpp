#pragma once
#include <cstdint>
#include <string_view>

enum class Binary_Storage : std::uint8_t
{

    bytes = 3,  // Bytes
    Bytes = 0,  // Bytes
    KB = 10,    // Kilobytes
    MB = 20,    // Megabytes
    GB = 30,    // Gigabytes
    TB = 40,    // Terabytes
    PB = 50     // petabyte
};

consteval std::uint64_t operator""_B(unsigned long long value)
{
    return value;
}

consteval std::uint64_t operator""_KB(long double value)
{
    return (std::uint64_t)(value *
                           (1ULL << static_cast<int>(Binary_Storage::KB)));
}
consteval std::uint64_t operator""_MB(long double value)
{
    return (std::uint64_t)(value *
                           (1ULL << static_cast<int>(Binary_Storage::MB)));
}

consteval std::uint64_t operator""_GB(long double value)
{
    return (std::uint64_t)(value *
                           (1ULL << static_cast<int>(Binary_Storage::GB)));
}

consteval std::uint64_t operator""_TB(long double value)
{
    return (std::uint64_t)(value *
                           (1ULL << static_cast<int>(Binary_Storage::TB)));
}
consteval std::uint64_t operator""_PB(long double value)
{
    return (std::uint64_t)(value *
                           (1ULL << static_cast<int>(Binary_Storage::PB)));
}

consteval std::uint64_t operator""_KB(unsigned long long value)
{
    return (std::uint64_t)(value << static_cast<int>(Binary_Storage::KB));
}
consteval std::uint64_t operator""_MB(unsigned long long value)
{
    return (std::uint64_t)(value << static_cast<int>(Binary_Storage::MB));
}

consteval std::uint64_t operator""_GB(unsigned long long value)
{
    return (std::uint64_t)(value << static_cast<int>(Binary_Storage::GB));
}

consteval std::uint64_t operator""_TB(unsigned long long value)
{
    return (std::uint64_t)(value << static_cast<int>(Binary_Storage::TB));
}
consteval std::uint64_t operator""_PB(unsigned long long value)
{
    return (std::uint64_t)(value << static_cast<int>(Binary_Storage::PB));
}
