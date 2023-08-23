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

        static auto get_debug_real_time() -> float;

        static void thread_yield(std::uint32_t ms);

        static auto scan_hash_table_key16(std::uint16_t key, void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*;

        static auto scan_hash_table_key32(std::uint32_t key, void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*;

        template <typename T, typename Compare> inline static void sort(T* ptr, size_t count, Compare comparer)
        {
            std::sort(ptr, ptr + count, comparer);
        }
    };
}
