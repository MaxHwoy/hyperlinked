#pragma once

#include <cstdint>

namespace hyper
{
    class global final
    {
    public:
        static inline std::uint32_t& frame_counter = *reinterpret_cast<std::uint32_t*>(0x00AB0A78);
    };
}
