#pragma once

#include <cstdint>
#include <memory>

namespace hyper
{
    class hook final
    {
    private:
        enum class instruction : std::uint8_t
        {
            nop = 0x90,
            mov = 0xB8,
            jmp = 0xE9,
            call = 0xE8,
            retn = 0xC3,
            retn_imm16 = 0xC2,
        };

    public:
        template <typename T> static void set(uintptr_t address, T value)
        {
            *reinterpret_cast<T*>(address) = value;
        }

        template <typename F> static void jump(uintptr_t address, F function)
        {
            hook::set<hook::instruction>(address, hook::instruction::jmp);
            hook::set<uintptr_t>(address + 1u, reinterpret_cast<uintptr_t>(function) - address - 5u);
        }

        template <typename F> static inline void call(uintptr_t address, F function)
        {
            hook::set<hook::instruction>(address, hook::instruction::call);
            hook::set<uintptr_t>(address + 1u, reinterpret_cast<uintptr_t>(function) - address - 5u);
        }

        template <typename T> static inline void return_value(uintptr_t address, T value)
        {
            hook::set<hook::instruction>(address, hook::instruction::mov);
            hook::set<uintptr_t>(address + 1u, reinterpret_cast<uintptr_t>(value));
            hook::set<hook::instruction>(address + 5u, hook::instruction::retn);
        }

        inline static void nop(uintptr_t address, size_t size)
        {
            ::memset(reinterpret_cast<void*>(address), static_cast<std::int32_t>(hook::instruction::nop), size);
        }

        inline static void retn(uintptr_t address)
        {
            hook::set<hook::instruction>(address, hook::instruction::retn);
        }

        inline static void retn(uintptr_t address, std::uint16_t size)
        {
            hook::set<hook::instruction>(address, hook::instruction::retn_imm16);
            hook::set<std::uint16_t>(address + 1u, size);
        }

        inline static void write(uintptr_t address, const std::initializer_list<char>& bytes)
        {
            ::memcpy(reinterpret_cast<char*>(address), bytes.begin(), bytes.size());
        }

        inline static void write_string(uintptr_t address, const char* string)
        {
            ::strcpy(reinterpret_cast<char*>(address), string);
        }
    };
}
