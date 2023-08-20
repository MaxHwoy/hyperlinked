#pragma once

#include <string>

namespace hyper
{
    class string final
    {
    public:
        template <typename T, typename std::char_traits<T>* = nullptr> constexpr static auto length(const T* string) -> std::uint32_t
        {
            std::uint32_t length = 0u;

            for (/* empty */; *string != 0; ++string, ++length) {}

            return length;
        }

        template <typename T, typename std::char_traits<T>* = nullptr> static auto to_uint(const T* string, std::uint32_t default_value) -> std::uint32_t;
    };
}

namespace hyper
{
    template <typename T, typename std::char_traits<T>*> auto string::to_uint(const T* string, std::uint32_t default_value) -> std::uint32_t
    {
        std::uint32_t result = 0u;

        while (*string != 0)
        {
            if (*string < '0' || *string > '9')
            {
                return default_value;
            }

            result *= 10u;
            result += *string++ - '0';
        }

        return result;
    }
}
