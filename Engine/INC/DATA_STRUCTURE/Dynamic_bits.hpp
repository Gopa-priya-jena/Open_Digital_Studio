
#include <Global.hpp>
#pragma once
#include <cstdint>
namespace DATA_STRUCTURE
{
inline unsigned int bits_to_bytes(unsigned short& bites);
#define BIT(X) &uint8_t[bits_to_bytes((X))]
void set_bit(unsigned short bit, void* data);
bool get_bit(unsigned short bit, void* data);
}  // namespace DATA_STRUCTURE
