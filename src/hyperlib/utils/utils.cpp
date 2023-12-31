#include <thread>
#include <hyperlib/global_vars.hpp>
#include <hyperlib/utils/utils.hpp>
#include <hyperlib/gameplay/game_flow.hpp>

using signed_t = std::make_signed_t<size_t>;

namespace hyper
{
    void utils::init_ticker()
    {
        call_function<void(__cdecl*)()>(0x0046CE30)();
    }

    auto utils::get_ticker() -> std::uint32_t
    {
        return call_function<std::uint32_t(__cdecl*)()>(0x0046CEC0)();
    }

    auto utils::get_ticker_difference(std::uint32_t start_ticks) -> float
    {
        return call_function<float(__cdecl*)(std::uint32_t)>(0x00473170)(start_ticks);
    }

    auto utils::get_ticker_difference(std::uint32_t start_ticks, std::uint32_t end_ticks) -> float
    {
        return call_function<float(__cdecl*)(std::uint32_t, std::uint32_t)>(0x0046CEF0)(start_ticks, end_ticks);
    }

    auto utils::get_real_time() -> float
    {
        return static_cast<float>(global::real_time_packed) * 0.00025f;
    }

    auto utils::get_world_time() -> float
    {
        return static_cast<float>(global::world_time_packed) * 0.00025f;
    }

    auto utils::get_render_time() -> float
    {
        if (game_flow::manager::instance.current_state == game_flow::state::in_frontend)
        {
            return utils::get_real_time();
        }
        else
        {
            return utils::get_world_time();
        }
    }

    void utils::thread_yield(std::uint32_t ms)
    {
        call_function<void(__cdecl*)(std::uint32_t)>(0x0046D0E0)(ms);
    }

    auto utils::scan_hash_table_key16(std::uint16_t key, const void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*
    {
        if (table_ptr == nullptr || table_size == 0u || (key_offset + sizeof(std::uint32_t) > entry_size))
        {
            return nullptr;
        }

        signed_t start = 0;
        signed_t end = static_cast<signed_t>(table_size) - 1;

        while (start <= end)
        {
            size_t middle = static_cast<size_t>(start + ((end - start) >> 1u)); // do this instead of (end + start) >> 1 b/c no overflow

            uintptr_t address = reinterpret_cast<uintptr_t>(table_ptr) + middle * entry_size;

            std::uint16_t compare = *reinterpret_cast<std::uint16_t*>(address + key_offset);

            if (key == compare)
            {
                while (address > reinterpret_cast<uintptr_t>(table_ptr))
                {
                    uintptr_t previous = address - entry_size;

                    std::uint16_t another = *reinterpret_cast<std::uint16_t*>(previous + key_offset);

                    if (key != another)
                    {
                        break;
                    }

                    address = previous;
                }

                return reinterpret_cast<void*>(address);
            }

            if (key < compare)
            {
                end = static_cast<signed_t>(middle) - 1;
            }
            else
            {
                start = static_cast<signed_t>(middle) + 1;
            }
        }

        return nullptr;
    }

    auto utils::scan_hash_table_key32(std::uint32_t key, const void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*
    {
        if (table_ptr == nullptr || table_size == 0u || (key_offset + sizeof(std::uint32_t) > entry_size))
        {
            return nullptr;
        }

        signed_t start = 0;
        signed_t end = table_size - 1;

        while (start <= end)
        {
            size_t middle = static_cast<size_t>(start + ((end - start) >> 1u)); // do this instead of (end + start) >> 1 b/c no overflow

            uintptr_t address = reinterpret_cast<uintptr_t>(table_ptr) + middle * entry_size;

            std::uint32_t compare = *reinterpret_cast<std::uint32_t*>(address + key_offset);

            if (key == compare)
            {
                while (address > reinterpret_cast<uintptr_t>(table_ptr))
                {
                    uintptr_t previous = address - entry_size;

                    std::uint32_t another = *reinterpret_cast<std::uint32_t*>(previous + key_offset);

                    if (key != another)
                    {
                        break;
                    }

                    address = previous;
                }

                return reinterpret_cast<void*>(address);
            }

            if (key < compare)
            {
                end = static_cast<signed_t>(middle) - 1;
            }
            else
            {
                start = static_cast<signed_t>(middle) + 1;
            }
        }

        return nullptr;
    }
}
