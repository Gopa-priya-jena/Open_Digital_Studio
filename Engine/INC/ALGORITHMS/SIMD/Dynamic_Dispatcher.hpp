#pragma once
#include "DATA_STRUCTURE/BitBools.hpp"
#include <DATA_STRUCTURE/DATA_STRUCTURE.hpp>
BITBOOLS(enum CPU_Feature{
    SSE2 = 0,
    SSE3,
    SSSE3,
    SSE4_1,
    SSE4_2,
    AVX,
    AVX2,
    AVX512,
    SHA,
    AES,

};)
CPU_FEATURES_AVAILABLE;
