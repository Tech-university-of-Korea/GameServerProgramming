#pragma once

#include <stdint.h>
#include <type_traits>
#include <numeric>

#undef max
#undef min

using SessionIdType = uint8_t;
using PacketSizeType = uint8_t;

inline constexpr SessionIdType INVALID_SESSION = std::numeric_limits<SessionIdType>::max();

template <typename T> requires std::is_arithmetic_v<T>
struct Point {
    T x;
    T y;
};

struct Byte2 {
    int8_t x;
    int8_t y;
};