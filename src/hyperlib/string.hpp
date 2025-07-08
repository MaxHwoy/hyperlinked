#pragma once

#include <memory>
#include <string>
#include <cstdarg>
#include <hyperlib/math.hpp>

namespace hyper
{
    class string final
    {
    public:
        static inline auto format(const char* fmt, ...) -> const char*
        {
            ::va_list va = {};

            va_start(va, fmt);

            auto& buffer = string::buffer_;

            ::vsnprintf(buffer, std::size(buffer), fmt, va);

            va_end(va);

            return buffer;
        }

        template <typename T, typename std::char_traits<T>* = nullptr> constexpr static auto length(const T* string) -> size_t
        {
            size_t length = 0u;

            for (/* empty */; *string != 0; ++string, ++length) {}

            return length;
        }

        template <typename T, size_t N, typename std::char_traits<T>* = nullptr> static void copy_s(T (&dst)[N], const T* src);

        template <typename T, typename std::char_traits<T>* = nullptr> static auto to_uint(const T* string, std::uint32_t default_value) -> std::uint32_t;

    private:
        static inline char buffer_[1024 + 1]{};
    };
}

namespace hyper
{
    template <typename T, size_t N, typename std::char_traits<T>*> void string::copy_s(T (&dst)[N], const T* src)
    {
        size_t length = math::min(string::length(src), N - 1);

        ::memcpy(dst, src, length);

        dst[length] = 0;
    }

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
