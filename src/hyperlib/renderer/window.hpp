#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class window final
    {
    public:
        static void get_resolution(std::uint32_t& x, std::uint32_t& y);

    public:
        static inline window& instance = *reinterpret_cast<window*>(0x00B1DBF4);

        static inline std::uint32_t& racing_resolution = *reinterpret_cast<std::uint32_t*>(0x00A65388);
    };
}
