#pragma once

#include <cstdint>

namespace hyper
{
    class math final
    {
    public:
        template <typename T> constexpr inline static auto align_pow_2(T value, T align) -> T
        {
            return (value + align - 1) & ~(align - 1);
        }

        template <typename T> constexpr inline static auto round_pow_2(T value, T round) -> T
        {
            return value & ~(round - 1);
        }

        template <typename T> constexpr inline static bool is_pow_2(T value)
        {
            return (value & (value - 1)) == 0 && value > 0;
        }
    };
}
