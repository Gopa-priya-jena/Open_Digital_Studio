#include <cstring>
#if 1

#include <Compiler_Intrinsics.hpp>
#include <cstdint>
#include <tuple>
typedef std::uint64_t BIT_SET;
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
constexpr std::uint8_t BitScanForward_no_intrinsic(std::uint64_t data)
{
    for (auto i = 0; i < 64; i++, data >>= 1)
        if ((data & 0X1)) return i;
    return 0;
}

constexpr std::uint8_t BitScanReverse_no_intrinsic(std::uint64_t data)
{
    data >>= 1;
    for (auto i = 1; i < 63; i++, data >>= 1)
        if (data == 0) return i;
    return 0;
}
constexpr std::uint8_t POPCOUNT_no_intrinsic(std::uint64_t data)
{
    std::uint8_t result{0};
    while (data != 0)
    {
        data = data & (data - 1);
        result++;
    }
    return result;
}
inline constexpr bool PARITY_no_intrinsic(std::uint8_t data)
{
    data = (data & ~0xF) ^ (data << 4);
    data = (data & ~0x3F) ^ (data << 2);
    data = (data & ~0x7F) ^ (data << 1);
    return (data == 0x80);
}
inline constexpr bool PARITY_no_intrinsic(std::uint64_t data)
{
    data = (data & ~0xFFFFFFFF) ^ (data << 32);
    data = (data & ~0xFFFFFFFFFFFF) ^ (data << 16);
    data = (data & ~0xFFFFFFFFFFFFFF) ^ (data << 8);
    return PARITY_no_intrinsic(std::uint8_t(data));
}

inline constexpr bool PARITY(std::uint8_t data)
{
#if (GCC | CLANG)
    return !__builtin_parity(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY(std::uint16_t data)
{
#if (GCC | CLANG)
    return !__builtin_parity(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY(std::uint32_t data)
{
#if (GCC | CLANG)
    return !__builtin_parityl(data);
#else
    PARITY_no_intrinsic(data);
#endif
}
inline constexpr bool PARITY(std::uint64_t data)
{
#if (GCC | CLANG)
    return !__builtin_parityll(data);
#else
    PARITY_no_intrinsic(data);
#endif
}

inline constexpr std::uint8_t BitScanForward(std::uint8_t data)
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
inline constexpr std::uint8_t BitScanForward(std::uint16_t data)
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
inline constexpr std::uint8_t BitScanForward(std::uint32_t data)
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
inline constexpr std::uint8_t BitScanForward(std::uint64_t data)
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
inline constexpr std::uint8_t BitScanReverse(std::uint8_t data)
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
inline constexpr std::uint8_t BitScanReverse(std::uint16_t data)
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

inline constexpr std::uint8_t BitScanReverse(std::uint32_t data)
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
inline constexpr std::uint8_t BitScanReverse(std::uint64_t data)
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

inline constexpr std::uint8_t POPCOUNT(std::uint8_t data)
{
#if (GCC | CLANG)
    return __builtin_popcount(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}
inline constexpr std::uint8_t POPCOUNT(std::uint16_t data)
{
#if (GCC | CLANG)
    return __builtin_popcount(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}

inline constexpr std::uint8_t POPCOUNT(std::uint32_t data)
{
#if (GCC | CLANG)
    return __builtin_popcountl(data);
#elif MSVC
    return _mm_popcnt_u32(data);
#else
    POPCOUNT_no_intrinsic(data);
#endif
}

inline constexpr std::uint8_t POPCOUNT(std::uint64_t data)
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
consteval unsigned long long CompTime_GET_BIT(std::uint64_t position)
{
    return (1ULL << position);
}
consteval unsigned long long CompTime_SET_BIT(unsigned long long Mask,
                                              std::uint64_t      position)
{
    return (Mask | (1ULL << position));
}
////// for general bitset mmanipulation

constexpr std::uint64_t Bits_to_Bytes(std::uint64_t Bits)
{
    return (Bits + 7) >> 3;
}
constexpr std::uint8_t Reverse_filter_mask(std::uint_fast8_t position)
{
    return 0x80 >> position;
}

constexpr std::uint64_t filter_mask(std::uint64_t position)
{
    return 1ULL << position;
};

constexpr std::uint8_t Eight_bit_trailing_1_mask[9]{
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
    std::uint8_t INFO{0};  // allocation and deallocation
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

        INFO |= static_cast<std::uint8_t>(destructor);
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
    std::uint64_t*               BITS{nullptr};    // fast for large  data
    std::uint8_t*                BITS_8{nullptr};  // fast for small data
    ALLOCATION_DEALLOCATION_INFO alloc_info;
    std::uint8_t  BLOCK8_Size{0};   // small byte only adressable memory
    std::uint32_t BLOCK64_Size{0};  // for 8byte adressable memory
    std::uint32_t Bitset_size{0};   // total number of bits
    std::uint64_t offset_mask{
        0};  // for small data of size smaller than byte/ not divisble by 8
             //

    void          set_sizes(std::uint64_t Size);
    std::uint64_t Get_position(std::uint32_t Block64,
                               std::uint8_t  Block8,
                               std::uint8_t  bit);

    inline std::uint64_t get_actual_position(std::uint64_t position)
    {
        return Bitset_size - position - 1;
    }

   public:
    ADVANCED_BITSET() = delete;
    ~ADVANCED_BITSET();
    ADVANCED_BITSET(BIT_SET* pointer,

                    std::uint64_t Size,

                    Allocator_Defragmentation_Stratergy defra);
    ADVANCED_BITSET(std::uint64_t size, Destructor destructor);
    std::uint64_t byteoffset(std::uint64_t position);
    const bool    operator[](std::uint64_t position);
    void          set_all();
    void          clear_all();
    void          set(std::uint64_t position);
    bool          at(std ::uint64_t position);
    void          clear(std::uint64_t position);
    void          Toggle(std::uint64_t position);
    bool          last_bit();
    bool          first_bit();
    bool          check_set_any();

    void          Set_trailling_0_to_1();
    void          Set_trailling_1_to_0();
    std::int64_t  Get_lsb();
    std::int64_t  Get_msb();
    std::uint64_t Get_mask_copy(ADVANCED_BITSET& data, ADVANCED_BITSET& filter);
    void          Swap_bits(std::uint64_t position1, std::uint64_t position2);
    std::uint64_t count_set_bits();
    std::uint64_t count_clear_bits();
    std::uint64_t count_consecutive_set_bits();
    std::uint64_t count_consecutive_clear_bits();
    std::tuple<std::uint64_t, std::uint64_t> MAX_consecutive_clear_bits();
    std::tuple<std::uint64_t, std::uint64_t> MAX_consecutive_set_bits();

    std::tuple<std::uint64_t, std::uint64_t> MIN_consecutive_clear_bits();
    std::tuple<std::uint64_t, std::uint64_t> MIN_consecutive_set_bits();
    std::uint64_t                            Get_1_permutaion_count();
    std::uint64_t                            Get_0_permutaion_count();
    void Rotate_left(const std::uint16_t position);
    void Rotate_right(const std::uint16_t position);
    void Slide_left_by_0(const std::uint16_t position);
    void Slide_Right_by_0(const std::uint16_t position);
    void Slide_left_by_1(const std::uint16_t position);
    void Slide_Right_by_1(const std::uint16_t position);

    void reverse();
    void flip();
    bool check_parity();
    void find_next_set_bit(std::uint64_t pos);
    void find_next_clear_bit(std::uint64_t pos);

    ADVANCED_BITSET extract_bit_field(std::uint64_t start_pos,
                                      std::uint64_t length);
    void            insert_bit_field(const ADVANCED_BITSET& field,
                                     std::uint64_t          start_pos);
    void            replace_bit_field(const ADVANCED_BITSET& field,
                                      std::uint64_t          start_pos,
                                      std::uint64_t          length);

    // some necesary operator overlading
    ADVANCED_BITSET& operator+(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator-(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator=(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator=(std::uint64_t&& unsigned_integer);
    ADVANCED_BITSET& operator^(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator&(ADVANCED_BITSET& bitset);
    ADVANCED_BITSET& operator|(ADVANCED_BITSET& bitset);

    ADVANCED_BITSET& operator^(std::uint64_t&& uinsigned_integer);
    ADVANCED_BITSET& operator&(std::uint64_t&& unsigned_integer);
    ADVANCED_BITSET& operator|(std::uint64_t&& unsigned_integer);
    ADVANCED_BITSET& operator^(std::uint64_t& uinsigned_integer);
    ADVANCED_BITSET& operator&(std::uint64_t& unsigned_integer);

    ADVANCED_BITSET& operator~();
    ADVANCED_BITSET  operator<<(std::uint64_t shift_amount);
    ADVANCED_BITSET  operator>>(std::uint64_t shift_amount);

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
                    sizeof(std::uint8_t) * Bits_to_Bytes(lhs.Bitset_size)))
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
