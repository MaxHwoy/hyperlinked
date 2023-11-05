#pragma once

#include <cstdint>
#include <hyperlib/math.hpp>

namespace hyper
{
    class trolli final
    {
    public:
        volatile std::uint32_t use_matrix;
        volatile std::uint32_t Pad1;
        volatile std::uint32_t Pad2;
        volatile std::uint32_t Pad3;
        matrix4x4 camera_matrix;

    public:
        static inline bool& disable_communication = *reinterpret_cast<bool*>(0x00A9E6BC);

        static inline trolli& instance = *reinterpret_cast<trolli*>(0x00A8AB10);
    };
}
