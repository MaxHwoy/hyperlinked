#pragma once

#include <cstdint>
#include <hyperlib/string.hpp>

namespace hyper
{
    class hashing final
    {
    public:
        constexpr static inline std::uint32_t initial_bin_hash = 0xFFFFFFFFu;
        constexpr static inline std::uint32_t initial_vlt_hash = 0xABCDEF00u;

    private:
        constexpr static auto bin_internal(const char* string, std::uint32_t prefix) -> std::uint32_t;
        
        constexpr static auto vlt_internal(const char* string, std::uint32_t prefix) -> std::uint32_t;

    public:
        constexpr static auto bin(const char* string) -> std::uint32_t;

        constexpr static auto vlt(const char* string) -> std::uint32_t;

        constexpr static auto bin(const char* string, std::uint32_t prefix) -> std::uint32_t;

        constexpr static auto vlt(const char* string, std::uint32_t prefix) -> std::uint32_t;

        consteval static auto bin_const(const char* string) -> std::uint32_t;

        consteval static auto vlt_const(const char* string) -> std::uint32_t;

        consteval static auto bin_const(const char* string, std::uint32_t prefix) -> std::uint32_t;

        consteval static auto vlt_const(const char* string, std::uint32_t prefix) -> std::uint32_t;
    };
}

namespace hyper
{
    constexpr auto hashing::bin_internal(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        for (std::uint32_t i = 0u; string[i] != 0; ++i)
        {
            prefix *= 0x21u;
            prefix += string[i];
        }

        return prefix;
    }

    constexpr auto hashing::vlt_internal(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        std::uint32_t length = static_cast<std::uint32_t>(string::length(string));
        std::uint32_t koffs = 0u;
        std::uint32_t loffs = length;
        std::uint32_t a = 0x9E3779B9u;
        std::uint32_t b = a;
        std::uint32_t c = prefix;

        while (loffs >= 12)
        {
            a += (std::uint32_t)string[0x0 + koffs] + ((std::uint32_t)string[0x1 + koffs] << 8) + ((std::uint32_t)string[0x2 + koffs] << 16) + ((std::uint32_t)string[0x3 + koffs] << 24);
            b += (std::uint32_t)string[0x4 + koffs] + ((std::uint32_t)string[0x5 + koffs] << 8) + ((std::uint32_t)string[0x6 + koffs] << 16) + ((std::uint32_t)string[0x7 + koffs] << 24);
            c += (std::uint32_t)string[0x8 + koffs] + ((std::uint32_t)string[0x9 + koffs] << 8) + ((std::uint32_t)string[0xA + koffs] << 16) + ((std::uint32_t)string[0xB + koffs] << 24);

            a -= b;
            a -= c;
            a ^= (c >> 13);
            b -= c;
            b -= a;
            b ^= (a << 8);
            c -= a;
            c -= b;
            c ^= (b >> 13);
            a -= b;
            a -= c;
            a ^= (c >> 12);
            b -= c;
            b -= a;
            b ^= (a << 16);
            c -= a;
            c -= b;
            c ^= (b >> 5);
            a -= b;
            a -= c;
            a ^= (c >> 3);
            b -= c;
            b -= a;
            b ^= (a << 10);
            c -= a;
            c -= b;
            c ^= (b >> 15);

            koffs += 12u;
            loffs -= 12u;
        }

        c += length;

        switch (loffs)
        {
            case 0xB:
                c += (std::uint32_t)string[0xA + koffs] << 24;
                [[fallthrough]];

            case 0xA:
                c += (std::uint32_t)string[0x9 + koffs] << 16;
                [[fallthrough]];

            case 0x9:
                c += (std::uint32_t)string[0x8 + koffs] << 8;
                [[fallthrough]];

            case 0x8:
                b += (std::uint32_t)string[0x7 + koffs] << 24;
                [[fallthrough]];

            case 0x7:
                b += (std::uint32_t)string[0x6 + koffs] << 16;
                [[fallthrough]];

            case 0x6:
                b += (std::uint32_t)string[0x5 + koffs] << 8;
                [[fallthrough]];

            case 0x5:
                b += (std::uint32_t)string[0x4 + koffs];
                [[fallthrough]];

            case 0x4:
                a += (std::uint32_t)string[0x3 + koffs] << 24;
                [[fallthrough]];

            case 0x3:
                a += (std::uint32_t)string[0x2 + koffs] << 16;
                [[fallthrough]];

            case 0x2:
                a += (std::uint32_t)string[0x1 + koffs] << 8;
                [[fallthrough]];

            case 0x1:
                a += (std::uint32_t)string[0x0 + koffs];
                break;
        }

        a -= b;
        a -= c;
        a ^= (c >> 13);
        b -= c;
        b -= a;
        b ^= (a << 8);
        c -= a;
        c -= b;
        c ^= (b >> 13);
        a -= b;
        a -= c;
        a ^= (c >> 12);
        b -= c;
        b -= a;
        b ^= (a << 16);
        c -= a;
        c -= b;
        c ^= (b >> 5);
        a -= b;
        a -= c;
        a ^= (c >> 3);
        b -= c;
        b -= a;
        b ^= (a << 10);
        c -= a;
        c -= b;
        c ^= (b >> 15);

        return c;
    }

    constexpr auto hashing::bin(const char* string) -> std::uint32_t
    {
        if (string == nullptr || *string == 0)
        {
            return 0u;
        }

        return hashing::bin_internal(string, hashing::initial_bin_hash);
    }

    constexpr auto hashing::vlt(const char* string) -> std::uint32_t
    {
        if (string == nullptr || *string == 0)
        {
            return 0u;
        }

        return hashing::vlt_internal(string, hashing::initial_vlt_hash);
    }

    constexpr auto hashing::bin(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        if (string == nullptr || *string == 0)
        {
            return prefix;
        }

        return hashing::bin_internal(string, prefix);
    }

    constexpr auto hashing::vlt(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        if (string == nullptr || *string == 0)
        {
            return prefix;
        }

        return hashing::vlt_internal(string, prefix);
    }

    consteval auto hashing::bin_const(const char* string) -> std::uint32_t
    {
        return hashing::bin(string);
    }

    consteval auto hashing::vlt_const(const char* string) -> std::uint32_t
    {
        return hashing::vlt(string);
    }

    consteval auto hashing::bin_const(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        return hashing::bin(string, prefix);
    }

    consteval auto hashing::vlt_const(const char* string, std::uint32_t prefix) -> std::uint32_t
    {
        return hashing::vlt(string, prefix);
    }
}
