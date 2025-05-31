#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
template <class Type, class Type_size =  U8 >
struct Offset_ptr
{
    Type_size Offset;
    Type      ptr[];
};
template <class Type, class Type_size =  U8 , std::size_t N = 0>
class STACK
{
    U8  BUFFER[(sizeof(Type) * N) + (sizeof(Type_size) * N)];
    U64   Buffer_size = (sizeof(Type) * N) + (sizeof(Type_size) * N);

    Offset_ptr<Type, Type_size>*HEAD_ptr{nullptr}, *TAIl_ptr{nullptr};
};
