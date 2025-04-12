#include <DATA_STRUCTURE/Dynamic_bits.hpp>
namespace DATA_STRUCTURE
{

inline unsigned int bits_to_bytes(unsigned short& bites)
{
    if (bites < 8)
    {
        return 1;
    }
    else if (bites % 8)
    {
        return (bites << 3);
    }
    else
    {
        return ((bites + (8 - bites % 8)) << 3);
    }
}

void set_bit(unsigned short& bit, void* data)

{
}

// namespace DATA_STRUCTURE
}  // namespace DATA_STRUCTURE
