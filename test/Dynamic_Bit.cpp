#include "Dynamic_Bit.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <tuple>
// TODO: forgot about edge case of masking and acting on the first byte
std::uint64_t ADVANCED_BITSET::Get_position(std::uint32_t Block64,
                                            std::uint8_t  Block8,
                                            std::uint8_t  bit)
{
    // PERF: fully optimised
    // 1
    // 2
    // 2

    std::uint64_t position = bit;
    position += Block64 << 6;
    position += Block8 << 3;
    return position;
}

void ADVANCED_BITSET::set_sizes(std::uint64_t Size)
{
    BLOCK64_Size = (Size >> 6);
    BLOCK8_Size = (((Size & 0x3F) + 7) >> 3);
    // offset_mask = (1 << (Size & 0x7)) - 1;
    offset_mask = Eight_bit_trailing_1_mask[Size & 0X7];

    if (BLOCK8_Size != 0)
    {
        BITS_8 = reinterpret_cast<uint8_t*>(&BITS[BLOCK64_Size]);
    }
}
// for stack based bitset

ADVANCED_BITSET::ADVANCED_BITSET(BIT_SET*                            pointer,
                                 std::uint64_t                       Size,
                                 Allocator_Defragmentation_Stratergy defrag =
                                     Allocator_Defragmentation_Stratergy::NONE)
{
    alloc_info.Set_Allocation_Type(Allocation_Type::FIXED);
    alloc_info.Set_Allocation_Stratergy_Type(defrag);
    alloc_info.set_Destructor(Destructor::NO_destruct);
    BITS = pointer;
    set_sizes(Size);

    Bitset_size = Size;
}
ADVANCED_BITSET::ADVANCED_BITSET(
    std::uint64_t Size, Destructor destructor = Destructor::Default_destruct)
{
    alloc_info.Set_Allocation_Type(Allocation_Type::FIXED);
    alloc_info.Set_Allocation_Stratergy_Type(
        Allocator_Defragmentation_Stratergy::FIXED);
    alloc_info.set_Destructor(destructor);

    BITS = static_cast<std::uint64_t*>(std::malloc(Bits_to_Bytes(Size)));
    set_sizes(Size);
    Bitset_size = Size;
}
ADVANCED_BITSET::~ADVANCED_BITSET()
{
    if (alloc_info.destruct()) free(BITS);
}

void ADVANCED_BITSET::set_all()
{
    std::memset(BITS, 0xFF, (Bitset_size) >> 3);
    // setting last bits;
    if (BLOCK8_Size)

    {
        BITS_8[BLOCK8_Size - 1] |= offset_mask;
    }
}
void ADVANCED_BITSET::clear_all()
{
    std::memset(BITS, 0x00, (Bitset_size) >> 3);
    if (BLOCK8_Size)

    {
        BITS_8[BLOCK8_Size - 1] &= ~offset_mask;
    }
}
void ADVANCED_BITSET::set(std::uint64_t position)
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    BIT[(position) >> 3] |= filter_mask(position & 0x7);
}

bool ADVANCED_BITSET::at(std ::uint64_t position)
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    return ((BIT[position >> 3] & (1 << (position & 0x7))));
}

void ADVANCED_BITSET::clear(std::uint64_t position)
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    BIT[position >> 3] &= (~filter_mask(position & 0x7));
}
void ADVANCED_BITSET::Toggle(std::uint64_t position)
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    BIT[position >> 3] ^= filter_mask(position & 0x7);
}
bool ADVANCED_BITSET::last_bit()
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    return BIT[(Bitset_size) >> 3] & (1 << (Bitset_size & 0X7));
};
bool ADVANCED_BITSET::first_bit()
{
    std::uint8_t* BIT = reinterpret_cast<uint8_t*>(&BITS[0]);
    return BIT[0] & 0X1;

    //(~(FF & (~offset_mask - 1)) >> 1);
}

bool ADVANCED_BITSET::check_set_any()
{
    // first for BLOCK64_Size
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        if (BITS[i] != 0) return true;
    }

    // for BLOCK8_Size
    for (auto i = 0; i < (BLOCK8_Size - 1); i++)
    {
        if (BITS_8[i] != 0) return true;
    }
    // last block   is special case in a array
    if ((BITS_8[BLOCK8_Size - 1] & offset_mask) != 0) return true;
    return false;
}

void ADVANCED_BITSET::Set_trailling_0_to_1()
{
    auto procedure = [&]<class Type>(Type* DATA, int&& i) -> bool
    {
        DATA[i] = DATA[i] - 1;
        if (~DATA[i])
        {
            Type temp = DATA[i] + 1;
            DATA[i] = (DATA[i]) | temp;
            return true;
        }
        return false;
    };

    if (BITS[0] & 0x1) return;
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        if (procedure(BITS, i)) return;
    }
    for (std::uint8_t i = 0; i < (BLOCK8_Size - 1); i++)
    {
        if (procedure(BITS_8, i)) return;
    }
    if (BLOCK8_Size)
        BITS_8[BLOCK8_Size - 1] |=
            (((BITS_8[BLOCK8_Size - 1] & offset_mask) - 1) |
             BITS_8[BLOCK8_Size - 1] & offset_mask) &
            offset_mask;
}
void ADVANCED_BITSET::Set_trailling_1_to_0()
{
    auto procedure = []<class Type>(Type* DATA, int&& i) -> bool
    {
        DATA[i] = ~DATA[i];
        if (DATA[i])
        {
            DATA[i] = ~((DATA[i] - 1) | DATA[i]);
            return true;
        }
        return false;
    };

    if ((BITS[0] & 0x1) == 0) return;
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        if (procedure(BITS, i)) return;
    }
    for (std::uint8_t i = 0; i < (BLOCK8_Size - 1); i++)
    {
        if (procedure(BITS_8, i)) return;
    }
    if ((BITS_8[BLOCK8_Size - 1] & offset_mask) & 0X1)
        BITS_8[BLOCK8_Size - 1] |=
            offset_mask & (~(BITS_8[BLOCK8_Size - 1] & offset_mask) - 1) |
            ~((BITS_8[BLOCK8_Size - 1] & offset_mask));
}
std::int64_t ADVANCED_BITSET::Get_lsb()
{
    auto procedure = []<class Type>(Type* DATA, int&& i) -> int
    {
        if (DATA[i] != 0)
        {
            return BitScanForward(DATA[i]);
        }

        return -1;
    };

    if ((BITS[0] & 0x1)) return 1;
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        int bitpos = procedure(BITS, i);
        if (bitpos != -1) return Get_position(i, 0, bitpos);
    }
    for (std::uint8_t i = 0; i < (BLOCK8_Size - 1); i++)
    {
        int bitpos = procedure(BITS_8, i);
        if (bitpos != -1) return Get_position(BLOCK64_Size - 1, i, bitpos);
    }

    // handle last bit
    if (offset_mask & BITS_8[BLOCK64_Size - 1])
    {
        auto pos = BitScanForward(BITS_8[BLOCK8_Size - 1] & offset_mask);
        return Get_position(BLOCK64_Size - 1, BLOCK8_Size - 1, pos);
    }

    return -1;
}
std::int64_t ADVANCED_BITSET::Get_msb()
{
    auto procedure = []<class Type>(Type* DATA, int&& i) -> int
    {
        if (DATA[i] != 0)
        {
            return BitScanReverse(DATA[i]);
        }

        return -1;
    };

    if ((BITS_8[BLOCK8_Size - 1] & offset_mask))

        return Get_position(BLOCK64_Size - 1,
                            BLOCK8_Size - 1,
                            BitScanReverse(BITS_8[BLOCK8_Size]));

    for (std::uint8_t i = BLOCK8_Size - 2; i >= 0; i++)
    {
        int bitpos = procedure(BITS_8, i);
        if (bitpos != -1) return Get_position(i, 0, bitpos);
    }
    for (std::uint32_t i = BLOCK64_Size - 1; i >= 0; i++)
    {
        int bitpos = procedure(BITS, i);
        if (bitpos != -1) return Get_position(i, 0, bitpos);
    }
    // handle last bit

    return -1;
}

void ADVANCED_BITSET::Swap_bits(std::uint64_t position1,
                                std::uint64_t position2)
{
    if (at(position1) != at(position2))
    {
        Toggle(position1);
        Toggle(position2);
    }
}

std::uint64_t ADVANCED_BITSET::count_set_bits()
{
    // TODO:optimise for sse operation

    std::uint64_t result1{0}, result2{0};

    auto procedure = []<class Type>(Type* DATA, int&& i) -> uint8_t
    {
        if (DATA[i] != 0)
        {
            return POPCOUNT(DATA[i]);
        }

        return -1;
    };

    for (std::uint8_t i = 0; i < (BLOCK8_Size - 1); i++)
    {
        result1 += procedure(BITS_8, i);
    }
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        result2 += procedure(BITS, i);
    }

    if (BLOCK8_Size)
        return result1 + result2 + POPCOUNT(BITS_8[BLOCK8_Size - 1]);

    // handle last bit

    return result1 + result2;
}

std::uint64_t ADVANCED_BITSET::count_clear_bits()
{
    return Bitset_size - count_set_bits();
}
std::uint64_t ADVANCED_BITSET::count_consecutive_set_bits()
{
    // prsently horrible prfomance
    // TODO:   optimize
    std::uint64_t result{0};
    auto procedure = [&]<class Type>(Type* DATA, int&& i) -> uint_fast8_t
    {
        Type temp = DATA[i] ^ ((DATA[i] << 1));
        return POPCOUNT(temp);
    };

    for (std::uint32_t i = 0; i < BLOCK64_Size - 1; i++)
    {
        result += procedure(BITS, i) + (int)(((BITS[i + 1] & 0x1) == 0) &&
                                             (BITS[i] & 0x8000000000000000));
    }

    if (BLOCK8_Size)
    {
        result += procedure(BITS, BLOCK64_Size - 1) +
                  (int)(((BITS_8[0] & 0x1) == 0) &&
                        (BITS[BLOCK64_Size - 1] & 0x8000000000000000));

        for (std::uint32_t i = 0; i < BLOCK8_Size - 1; i++)
        {
            result += procedure(BITS, i) +
                      (int)(((BITS_8[i + 1] & 0x1) == 0) &&
                            (BITS_8[i] & 0x8000000000000000));  /// wrong
        }

        result += POPCOUNT(std::uint8_t(BITS_8[BLOCK8_Size - 1] ^
                                        (BITS_8[BLOCK8_Size - 1] << 1))) +
                  (BITS_8[BLOCK8_Size - 1] & 0x80);
    }
    result += POPCOUNT(std::uint64_t(BITS[BLOCK64_Size - 1] ^
                                     (BITS[BLOCK64_Size - 1] << 1))) +
              (BITS_8[BLOCK8_Size - 1] & 0x8000000000000000);

    return result;
}
std::uint64_t ADVANCED_BITSET::count_consecutive_clear_bits()
{
    std::uint64_t result = count_consecutive_set_bits();

    switch ((int)first_bit() + (int)last_bit())
    {
        case 0:
        {
            result++;
            break;
        }

        case 1:
        {
            break;
        }
        case 2:
        {
            result--;
            break;
        }
    }

    return result;
}

std::tuple<std::uint64_t, std::uint64_t>
ADVANCED_BITSET::MAX_consecutive_set_bits()
{
    std::uint64_t actual_position{0};
    std::uint8_t  block8{0}, block64{0}, bit_position{0}, max_Size{0};
    std::uint32_t last_block{0};
    std::uint8_t  last_max{0}, last_bitpos{0};
    bool          last = 0;
    std::uint8_t* last_block_pointer{nullptr};

    auto procedure = [&]<class Type>(Type* DATA, int&& i, std::uint8_t& block)
    {
        Type temp = DATA[i];
        auto pos = 0;
        while (temp)
        {
            last_max = BitScanForward(std::uint64_t(~temp));
            temp = temp >> (last_max + 1);
            pos += last_max + 1;
            if (max_Size < last_max)
            {
                max_Size = last_max;
                if (last)
                {
                    bit_position = last_bitpos;
                    *last_block_pointer = last_block;
                }
                else
                {
                    bit_position = pos - last_max - 1;
                    block = i;
                }
            }
        }

        if (pos >= (sizeof(Type) * 8) && last_max)
        {
            // TODO:: complete state  managment logic

            last_bitpos = pos - last_max - 1;
            last_block = i;
            if (!last) last_block_pointer = &block;
            last = true;
        }
        else
        {
            last_max = 0;
            last = false;
        }
    };
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        procedure(BITS, i, block8);
    }
    for (std::uint8_t i = 0; i < BLOCK8_Size - 1; i++)
    {
        procedure(BITS_8, i, block8);
    }
    if (BLOCK8_Size)
    {
        uint8_t data = BITS_8[BLOCK8_Size - 1] & offset_mask;
        procedure(&data, BLOCK8_Size - 1, block8);
    }
    if (block8)
    {
        block64 = BLOCK64_Size - 1;
    }
    actual_position = Get_position(block64, block8, bit_position);
    return {actual_position, max_Size};
}
std::tuple<std::uint64_t, std::uint64_t>
ADVANCED_BITSET::MAX_consecutive_clear_bits()
{
    std::uint64_t bit_position{0}, max_Size{0}, actual_position{0};
    std::uint8_t  block8{0}, block64{0};
    std::uint8_t  last_max{0};
    bool          block_change = 0;

    auto procedure = [&]<class Type>(Type* DATA, int&& i, std::uint8_t& block)
    {
        Type temp = ~DATA[i];
        auto pos = 0;
        while (temp)
        {
            last_max = BitScanForward(std::uint64_t(~temp));
            temp = temp >> (last_max + 1);
            pos += last_max + 1;
            if (max_Size < last_max)
            {
                max_Size = last_max;
                bit_position = pos - last_max - 1;
                block_change = true;
            }
        }

        if (pos <= (sizeof(Type) * 8))
        {
            last_max = 0;
        }
        if (block_change)
        {
            block8 = block64 = 0;
            block = i;
        }
    };
    for (std::uint32_t i = 0; i < BLOCK64_Size; i++)
    {
        procedure(BITS, i, block8);
    }
    for (std::uint8_t i = 0; i < BLOCK8_Size - 1; i++)
    {
        procedure(BITS_8, i, block8);
    }
    if (BLOCK8_Size)
    {
        uint8_t data = BITS_8[BLOCK8_Size - 1] & offset_mask;
        procedure(&data, BLOCK8_Size - 1, block8);
    }
    if (block8)
    {
        block64 = BLOCK64_Size - 1;
    }
    actual_position = Get_position(block64, block8, bit_position);
    return {actual_position, max_Size};
}
std::tuple<std::uint64_t, std::uint64_t>
ADVANCED_BITSET::MIN_consecutive_set_bits()
{
}
std::uint64_t ADVANCED_BITSET::Get_0_permutaion_count()
{
    std::uint64_t result = 1;

    for (auto i = Bitset_size; i > Bitset_size - count_clear_bits(); i--)
        result *= i;
    return result;
}
std::uint64_t ADVANCED_BITSET::Get_1_permutaion_count()
{
    std::uint64_t result = 1;

    for (auto i = Bitset_size; i > Bitset_size - count_set_bits(); i--)
        result *= i;
    return result;
}
void ADVANCED_BITSET::Rotate_left(const std::uint16_t position)
{
}
void ADVANCED_BITSET::Rotate_right(const std::uint16_t position)
{
}
void ADVANCED_BITSET::Slide_left_by_0(const std::uint16_t position)
{
}
void ADVANCED_BITSET::Slide_Right_by_0(const std::uint16_t position)
{
}
void ADVANCED_BITSET::Slide_left_by_1(const std::uint16_t position)
{
}
void ADVANCED_BITSET::Slide_Right_by_1(const std::uint16_t position)
{
}
void ADVANCED_BITSET::reverse()
{
}
void ADVANCED_BITSET::flip()
{
    // TODO:smid optimise

    for (auto i = 0; i < BLOCK64_Size; i++)
    {
        BITS[i] = BITS[i] ^ 0xFFFFFFFFFFFFFFFF;
    }

    for (auto i = 0; i <= BLOCK8_Size - 1; i++)
    {
        BITS[i] = BITS[i] ^ 0xFF;
    }
    if (BLOCK8_Size)
    {
        BITS[BLOCK8_Size - 1] |= offset_mask & (BITS[BLOCK8_Size - 1] ^ 0xFF);
    }
}
bool ADVANCED_BITSET::check_parity()
{
    bool parity;

    {
        std::uint64_t tmp = 0;
        for (auto i = 0; i < BLOCK64_Size; i++)
        {
            tmp = BITS[i];
        }

        parity = PARITY(tmp);
    }

    if (BLOCK8_Size)
    {
        std::uint8_t tmp = (BITS[BLOCK8_Size - 1] & offset_mask);

        for (auto i = 1; i < BLOCK8_Size - 1; i++)
        {
            tmp ^= BITS[i];
        }

        parity = PARITY(tmp) & parity;
    }

    return parity;
}

void ADVANCED_BITSET::find_next_set_bit(std::uint64_t pos)
{
}
void ADVANCED_BITSET::find_next_clear_bit(std::uint64_t pos)
{
}

void ADVANCED_BITSET::replace_bit_field(const ADVANCED_BITSET& field,
                                        std::uint64_t          start_pos,
                                        std::uint64_t          length)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator+(ADVANCED_BITSET& bitset)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator-(ADVANCED_BITSET& bitset)
{
}

ADVANCED_BITSET& ADVANCED_BITSET::operator=(ADVANCED_BITSET& bitset)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator=(std::uint64_t&& unsigned_integer)
{
}

ADVANCED_BITSET& ADVANCED_BITSET::operator^(std::uint64_t&& uinsigned_integer)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator&(std::uint64_t&& unsigned_integer)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator|(std::uint64_t&& unsigned_integer)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator^(std::uint64_t& uinsigned_integer)
{
}
ADVANCED_BITSET& ADVANCED_BITSET::operator&(std::uint64_t& unsigned_integer)
{
}

ADVANCED_BITSET& ADVANCED_BITSET::operator~()
{
}
ADVANCED_BITSET ADVANCED_BITSET::operator<<(std::uint64_t shift_amount)
{
}
ADVANCED_BITSET ADVANCED_BITSET::operator>>(std::uint64_t shift_amount)
{
}
std::uint64_t ADVANCED_BITSET::Get_mask_copy(ADVANCED_BITSET& data,
                                             ADVANCED_BITSET& filter)
{
}
