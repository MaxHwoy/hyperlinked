#pragma once

#include <cstdint>

namespace hyper
{
    struct vector2
    {
        float x;
        float y;
    };

    struct vector3
    {
        float x;
        float y;
        float z;
    };

    struct vector4
    {
        float x;
        float y;
        float z;
        float w;
    };

    __declspec(align(0x10)) struct vector3pad
    {
        float x;
        float y;
        float z;
    };

    struct matrix4x4
    {
        float m11;
        float m12;
        float m13;
        float m14;
        float m21;
        float m22;
        float m23;
        float m24;
        float m31;
        float m32;
        float m33;
        float m34;
        float m41;
        float m42;
        float m43;
        float m44;
    };

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

        template <typename T> constexpr inline static auto set_flag(T value, T flag) -> T
        {
            return value; // #TODO
        }
    };
}
