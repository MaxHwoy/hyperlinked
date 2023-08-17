#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class utils final
    {
    public:
        static void init_ticker();

        static auto get_ticker() -> std::uint32_t;

        static auto get_ticker_difference(std::uint32_t start_ticks, std::uint32_t end_ticks) -> float;

        static void thread_yield(std::uint32_t ms);

        static auto scan_hash_table_key32(std::uint32_t key, void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*;
    };
}
