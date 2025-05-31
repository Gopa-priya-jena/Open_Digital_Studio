#include <cstring>
#if 1

#include <Compiler_Intrinsics.hpp>
#include <cstdint>
#include <tuple>
typedef U64   BIT_SET;
// optimization for allocator
//  this is used too dermine  the type of
// space  usage

enum class Allocation_Type : short
{
    FIXED = 0x0,    // 00 fixed size
    DYNAMIC = 0x2,  // 01  not fixed and requires pointer
    STACK = 0x4,    // 10 implemented on a stack
    TEMP = 0x6,     // 11 one function call only use
};
enum class Allocator_Defragmentation_Stratergy : short
{
    FIXED = 0x0,  // 00 can be defraged
    SELF_DEFRAG =
        0x8,  // 01 the defagmentation required data strutures help and
              //  allocator
    AUTO_DEFRAG = 0x10,  // 10 NO help from allocator is needed
    NONE = 0x18          // 11 no defragmentation needed
};
enum class Destructor : short
{
    Default_destruct = 0x1,  // 1
    NO_destruct = 0x0,       // 0

};

/// these are Compiler_Intrinsics functions and can be replaced
constexpr  U8 BitScanForward_no_intrinsic( U64   data)
{
    for (auto i = 0; i < 64; i++, data >>= 1)
        if ((data & 0X1)) return i;
    return 0;
}

constexpr  U8 BitScanReverse_no_intrinsic( U64   data)
{
    data >>= 1;
    for (auto i = 1; i < 63; i++, data >>= 1)
        if (data == 0) return i;
    return 0;
}
constexpr  U8 POPCOUNT_no_intrinsic( U64   data)
{
     U8 result{0};
    while (data != 0)
    {
        data = data & (data - 1);
        result++;
    }
    return result;
}
inline constexpr bool PARITY_no_intrinsic( U8 data)
{
    data = (data & ~0xF) ^ (data << 4);
    data = (data & ~0x3F) ^ (data << 2);
    data = (data & ~0x7F) ^ (data << 1);
    return (data == 0x80);
}
inline constexpr bool PARITY_no_intrinsic( U64   data)
{
    data = (data & ~0xFFFFFFFF) ^ (data << 32);
    data = (data & ~0xFFFFFFFFFFFF) ^ (data << 16);
    data = (data & ~0xFFFFFFFFFFFFFF) ^ (data << 8);
    return PARITY_no_intrinsic( U8 (data));
}

inline constexpr bool PARITY( U8 data)
{
#if (GCC | CLANG)
    return !__builtin_parity(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY( U16  data)
{
#if (GCC | CLANG)
    return !__builtin_parity(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY( U32  data)
{
#if (GCC | CLANG)
    return !__builtin_parityl(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY( U64   data)
{
#if (GCC | CLANG)
    return !__builtin_parityll(data);
#else
    PARITY_no_intrinsic(data);
#endif
}

inline constexpr  U8 BitScanForward( U8 data)
{
#if (GCC | CLANG)
    return __builtin_ctz(data);
#elif MSVC
    int r = 0;
    _BitScanForward(&r, data);
    return r;
#else
    BitScaneForward_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanForward( U16  data)
{
#if (GCC | CLANG)
    return __builtin_ctz(data);
#elif MSVC
    int r = 0;
    _BitScanForward(&r, data);
    return r;
#else
    BitScaneForward_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanForward( U32  data)
{
#if (GCC | CLANG)
    return __builtin_ctzl(data);
#elif MSVC
    int r = 0;
    _BitScanForward(&r, data);
    return r;
#else
    BitScaneForward_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanForward( U64   data)
{
#if (GCC | CLANG)
    return __builtin_ctzll(data);
#elif MSVC
    int r = 0;
    _BitScanForward(&r, data);
    return r;
#else
    BitScaneForward_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanReverse( U8 data)
{
#if (GCC | CLANG)
    return __builtin_clz(data);
#elif MSVC
    int r = 0;
    _BitScanReverse(&r, data);
    return r;
#else
    BitScanReverse_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanReverse( U16  data)
{
    return __builtin_clz(data);

#if (GCC | CLANG)
    return __builtin_clz(data);
#elif MSVC
    int r = 0;
    _BitScanReverse(&r, data);
    return r;
#else
    BitScanReverse_no_intrinsic(data);
#endif
}

inline constexpr  U8 BitScanReverse( U32  data)
{
#if (GCC | CLANG)
    return __builtin_clzl(data);
#elif MSVC
    int r = 0;
    _BitScanReverse(&r, data);
    return r;
#else
    BitScanReverse_no_intrinsic(data);
#endif
}
inline constexpr  U8 BitScanReverse( U64   data)
{
#if (GCC | CLANG)
    return __builtin_clzll(data);
#elif MSVC
    int r = 0;
    _BitScanReverse64(&r, data);
    return r;
#else
    BitScanReverse_no_intrinsic(data);
#endif
}

inline constexpr  U8 POPCOUNT( U8 data)
{
#if (GCC | CLANG)
    return __builtin_popcount(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}
inline constexpr  U8 POPCOUNT( U16  data)
{
#if (GCC | CLANG)
    return __builtin_popcount(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}

inline constexpr  U8 POPCOUNT( U32  data)
{
#if (GCC | CLANG)
    return __builtin_popcountl(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}

inline constexpr  U8 POPCOUNT( U64   data)
{
#if (GCC | CLANG)
    return __builtin_popcountll(data);
#elif MSVC
    return _mm_popcnt_u64(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}
// some bitmask making functions these are for grnaral bit utility in compile
// time
consteval unsigned long long CompTime_GET_BIT( U64   position)
{
    return (1ULL << position);
}
consteval unsigned long long CompTime_SET_BIT(unsigned long long Mask,
                                              U64        position)
{
    return (Mask | (1ULL << position));
}
////// for general bitset mmanipulation

constexpr U64   Bits_to_Bytes( U64   Bits)
{
    return (Bits + 7) >> 3;
}
constexpr  U8 Reverse_filter_mask(std::uint_fast8_t position)
{
    return 0x80 >> position;
}

constexpr U64   filter_mask( U64   position)
{
    return 1ULL << position;
};

constexpr  U8 Eight_bit_trailing_1_mask[9]{
    0x0,
    0x1,
    0x3,
    0x7,
    0xF,
    0x1F,
    0x3F,
    0x7F,
    0XFF,

};
//
struct ALLOCATION_DEALLOCATION_INFO
{
     U8 INFO{0};  // allocation and deallocation
                           // stratergies
                           // working
                           // |8||7||6||5||4||3||2||1|
    //___________/54defragmentation/32Allocation_Type/1Destructor
    ////
    ALLOCATION_DEALLOCATION_INFO() : INFO(0)
    {
    }
    inline bool destruct()
    {
        return INFO & 0x1;
    }
    inline void set_Destructor(Destructor destructor)
    {
        INFO = INFO & ~0x1;

        INFO |= static_cast< U8 >(destructor);
    }
    inline void Set_Allocation_Type(Allocation_Type type)
    {
        INFO |= static_cast<uint8_t>(type);
    }

    inline void Reset_Allocation_Type(Allocation_Type type)
    {
        INFO = INFO & ~0x6;
        INFO |= static_cast<uint8_t>(type);
    }

    inline void Set_Allocation_Stratergy_Type(
        Allocator_Defragmentation_Stratergy stratergies)
    {
        INFO |= static_cast<uint8_t>(stratergies);
    }
    inline void Reset_Allocation_Stratergy_Type(
        Allocator_Defragmentation_Stratergy stratergies)
    {
        INFO = INFO & ~0x18;

        INFO |= static_cast<uint8_t>(stratergies);
    }
};

class ADVANCED_BITSET
{
     U64  *               BITS{nullptr};    // fast for large  data
     U8 *                BITS_8{nullptr};  // fast for small data
    ALLOCATION_DEALLOCATION_INFO alloc_info;
    U8  BLOCK8_Size{0};   // small byte only adressable memory
    U32   BLOCK64_Size{0};  // for 8byte adressable memory
    U32   Bitset_size{0};   // total number of bits
    U64   offset_mask{
        0};  // for small data of size smaller than byte/ not divisble by 8
             //

    void          set_sizes( U64   Size);
    U64   Get_position( U32  Block64,
                               U8  Block8,
                               U8  bit);

    inline U64   get_actual_position( U64   position)
    {
        return Bitset_size - position - 1;
    }

   public:
    ADVANCED_BITSET() = delete;
    ~ADVANCED_BITSET();
    ADVANCED_BITSET(BIT_SET* pointer,

                    U64   Size,

                    Allocator_Defragmentation_Stratergy defra);
    ADVANCED_BITSET( U64   size, Destructor destructor);
    U64   byteoffset( U64   position);
    const bool    operator[]( U64   position);
    void          set_all();
    void          clear_all();
    void          set( U64   position);
    bool          at(std ::uint64_t position);
    void          clear( U64   position);
    void          Toggle( U64   position);
    bool          last_bit();
    bool          first_bit();
    bool          check_set_any();

    void          Set_trailling_0_to_1();
    void          Set_trailling_1_to_0();
    std::int64_t  Get_lsb();
    std::int64_t  Get_msb();
    U64   Get_mask_copy(ADVANCED_BITSET& data, ADVANCED_BITSET& filter);
    void          Swap_bits( U64   position1, U64   position2);
    U64   count_set_bits();
    U64   count_clear_bits();
    U64   count_consecutive_set_bits();
    U64   count_consecutive_clear_bits();
    std::tuple< U64  ,  U64  > MAX_consecutive_clear_bits();
    std::tuple< U64  ,  U64  > MAX_consecutive_set_bits();

    std::tuple< U64  ,  U64  > MIN_consecutive_clear_bits();
    std::tuple< U64  ,  U64  > MIN_consecutive_set_bits();
    U64                              Get_1_permutaion_count();
    U64                              Get_0_permutaion_count();
    void Rotate_left(const U16 position);
    void Rotate_right(const U16 position);
    void Slide_left_by_0(const U16 position);
    void Slide_Right_by_0(const U16 position);
    void Slide_left_by_1(const U16 position);
    void Slide_Right_by_1(const U16 position);

    void reverse();
    void flip();
    bool check_parity();
    void find_next_set_bit( U64   pos);
    void find_next_clear_bit( U64   pos);

    ADVANCED_BITSET extract_bit_field( U64   start_pos,
                                      U64   length);
    void            insert_bit_field(const ADVANCED_BITSET& field,
                                     U64            start_pos);
    void            replace_bit_field(const ADVANCED_BITSET& field,
                                      U64            start_pos,
                                      U64            length);

    // some necesary operator overlading
    ADVANCED_BITSET& operator+(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator-(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator=(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator=( U64  && unsigned_integer);
    ADVANCED_BITSET& operator^(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator&(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator|(ADVANCED_BITSET& bitset);

    ADVANCED_BITSET& operator^( U64  && uinsigned_integer);
    ADVANCED_BITSET& operator&( U64  && unsigned_integer);
    ADVANCED_BITSET& operator|( U64  && unsigned_integer);
    ADVANCED_BITSET& operator^( U64  & uinsigned_integer);
    ADVANCED_BITSET& operator&( U64  & unsigned_integer);

    ADVANCED_BITSET& operator~();
    ADVANCED_BITSET  operator<<( U64   shift_amount);
    ADVANCED_BITSET  operator>>( U64   shift_amount);

    friend bool operator<(const ADVANCED_BITSET& lhs,
                          const ADVANCED_BITSET& rhs);
    friend bool operator>(const ADVANCED_BITSET& lhs,
                          const ADVANCED_BITSET& rhs);
    friend bool operator<=(const ADVANCED_BITSET& lhs,
                           const ADVANCED_BITSET& rhs);
    friend bool operator>=(const ADVANCED_BITSET& lhs,
                           const ADVANCED_BITSET& rhs);
    friend bool operator==(const ADVANCED_BITSET& lhs,
                           const ADVANCED_BITSET& rhs);
    friend bool operator!=(const ADVANCED_BITSET& lhs,
                           const ADVANCED_BITSET& rhs);
};
inline bool operator<(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs);
inline bool operator>(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs);
inline bool operator<=(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs);
inline bool operator>=(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs);
inline bool operator==(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs)
{
    if (lhs.Bitset_size != rhs.Bitset_size) return false;

    if (std::memcmp(lhs.BITS,
                    rhs.BITS,
                    sizeof( U8 ) * Bits_to_Bytes(lhs.Bitset_size)))
        return true;
    else
    {
        return false;
    }
}
inline bool operator!=(const ADVANCED_BITSET& lhs, const ADVANCED_BITSET& rhs)
{
    return !(lhs == rhs);
}
#endif
