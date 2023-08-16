#include <hyperlib/utils/utils.hpp>

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

    auto utils::get_ticker_difference(std::uint32_t start_ticks, std::uint32_t end_ticks) -> float
    {
        return call_function<float(__cdecl*)(std::uint32_t, std::uint32_t)>(0x0046CEF0)(start_ticks, end_ticks);
    }

    auto utils::scan_hash_table_key32(std::uint32_t key, void* table_ptr, size_t table_size, size_t key_offset, size_t entry_size) -> void*
    {
        if (table_ptr == nullptr || table_size == 0u || (key_offset + sizeof(std::uint32_t) > entry_size))
        {
            return nullptr;
        }

        uintptr_t start = reinterpret_cast<uintptr_t>(table_ptr);
        uintptr_t end = start + entry_size * (table_size - 1u);

        while (start <= end)
        {
            uintptr_t middle = start + ((end - start) >> 1u);

            std::uint32_t compare = *reinterpret_cast<std::uint32_t*>(middle + key_offset);

            if (key == compare)
            {
                while (middle > start)
                {
                    uintptr_t previous = middle - entry_size;

                    std::uint32_t another = *reinterpret_cast<std::uint32_t*>(previous + key_offset);

                    if (key != another)
                    {
                        break;
                    }

                    middle = previous;
                }

                return reinterpret_cast<void*>(middle);
            }

            if (key < compare)
            {
                end = middle;
            }
            else
            {
                start = middle;
            }
        }

        return nullptr;
    }
}
