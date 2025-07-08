#pragma once

#include <cstdint>
#include <array>
#include <bitset>

namespace hyper
{
    class dbgbreak final
    {
    public:
        enum class result
        {
            uninited = -1,
            success,
            unsupported_platform,
            cant_get_thread_context,
            cant_set_thread_context,
            no_available_registers,
            bad_when, // unsupported value of when passed
            bad_size, // size can only be 1, 2, 4, 8
        };

        enum class when
        {
            read_or_written,
            written,
            executed,
        };

        struct breakpoint
        {
            static inline breakpoint make_failed(result r)
            {
                return { -1, r };
            }

            inline breakpoint() : register_index(-1), error(result::uninited) {}
            inline breakpoint(const breakpoint& b) : register_index(b.register_index), error(b.error) {}
            inline breakpoint(breakpoint&& b) noexcept : register_index(b.register_index), error(b.error) {}
            inline breakpoint(char r, result e) : register_index(r), error(e) {}
            inline breakpoint& operator=(const breakpoint& b) { this->register_index = b.register_index; this->error = b.error; return *this; }

            char register_index;
            result error;
        };

    private:
        class detail final
        {
        public:
            template <typename TAction, typename TFailure> static auto update_thread_context(TAction action, TFailure failure);
        };

    public:
        static auto set(const void* on_pointer, std::uint8_t size, when when) -> breakpoint;

        static void remove(const breakpoint& bp);
    };
}
