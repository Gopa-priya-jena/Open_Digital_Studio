#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
template <class Type, class Type_size = std::uint8_t>
struct Offset_ptr
{
    Type_size Offset;
    Type      ptr[];
};
template <class Type, class Type_size = std::uint8_t, std::size_t N = 0>
class STACK
{
    std::uint8_t  BUFFER[(sizeof(Type) * N) + (sizeof(Type_size) * N)];
    std::uint64_t Buffer_size = (sizeof(Type) * N) + (sizeof(Type_size) * N);

    Offset_ptr<Type, Type_size>*HEAD_ptr{nullptr}, *TAIl_ptr{nullptr};
};
