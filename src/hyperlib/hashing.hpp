#pragma once

#include <cstdint>

namespace hyper
{
    class hashing final
    {
    public:
        constexpr static auto bin(const char* string) -> std::uint32_t;
    };
}

namespace hyper
{
    constexpr auto hashing::bin(const char* string) -> std::uint32_t
    {
        if (string == nullptr || *string == 0)
        {
            return 0u;
        }

        std::uint32_t result = std::numeric_limits<std::uint32_t>::max();

        for (std::uint32_t i = 0u; string[i] != 0; ++i)
        {
            result *= 0x21u;
            result += string[i];
        }

        return result;
    }
}
