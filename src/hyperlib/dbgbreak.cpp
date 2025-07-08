#include <assert.h>
#include <Windows.h>
#include <platform.hpp>
#include <hyperlib/dbgbreak.hpp>

namespace hyper
{
#ifdef PLATFORM_WINDOWS
    template <typename TAction, typename TFailure> static auto dbgbreak::detail::update_thread_context(TAction action, TFailure failure)
    {
        CONTEXT ctx { 0 };

        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if (::GetThreadContext(::GetCurrentThread(), &ctx) == FALSE)
        {
            return failure(result::cant_get_thread_context);
        }

        std::array<bool, 4> busy_debug_register { { false, false, false, false } };

        auto check_busy_register = [&](std::size_t index, DWORD64 mask)
        {
            if (ctx.Dr7 & mask)
            {
                busy_debug_register[index] = true;
            }
        };

        check_busy_register(0, 1);
        check_busy_register(1, 4);
        check_busy_register(2, 16);
        check_busy_register(3, 64);

        const auto action_result = action(ctx, busy_debug_register);

        if (::SetThreadContext(::GetCurrentThread(), &ctx) == FALSE)
        {
            return failure(result::cant_set_thread_context);
        }

        return action_result;
    }

    auto dbgbreak::set(const void* on_pointer, std::uint8_t size, when when) -> dbgbreak::breakpoint
    {
        return dbgbreak::detail::update_thread_context([&](CONTEXT& ctx, const std::array<bool, 4>& busy_debug_register) -> breakpoint
        {
            const auto found = std::find(std::begin(busy_debug_register), std::end(busy_debug_register), false);

            if (found == std::end(busy_debug_register))
            {
                return breakpoint::make_failed(result::no_available_registers);
            }

            const auto register_index = static_cast<std::uint16_t>(std::distance(std::begin(busy_debug_register), found));

            switch (register_index)
            {
                case 0:
                    ctx.Dr0 = reinterpret_cast<DWORD_PTR>(const_cast<void*>(on_pointer));
                    break;

                case 1:
                    ctx.Dr1 = reinterpret_cast<DWORD_PTR>(const_cast<void*>(on_pointer));
                    break;

                case 2:
                    ctx.Dr2 = reinterpret_cast<DWORD_PTR>(const_cast<void*>(on_pointer));
                    break;

                case 3:
                    ctx.Dr3 = reinterpret_cast<DWORD_PTR>(const_cast<void*>(on_pointer));
                    break;

                default:
                    assert(false);
                    break;
            }

            std::bitset<sizeof(ctx.Dr7) * 8> dr7;
            std::memcpy(&dr7, &ctx.Dr7, sizeof(ctx.Dr7));

            dr7.set(register_index * 2); // flag to enable 'local' debugging for each of 4 registers. second bit is for global debugging, not working.

            switch (when)
            {
                case when::read_or_written:
                    dr7.set(16 + register_index * 4 + 1, true);
                    dr7.set(16 + register_index * 4, true);
                    break;

                case when::written:
                    dr7.set(16 + register_index * 4 + 1, false);
                    dr7.set(16 + register_index * 4, true);
                    break;

                case when::executed:
                    dr7.set(16 + register_index * 4 + 1, false);
                    dr7.set(16 + register_index * 4, false);
                    break;

                default:
                    return breakpoint::make_failed(result::bad_when);
            }

            switch (size)
            {
                case 1:
                    dr7.set(16 + register_index * 4 + 3, false);
                    dr7.set(16 + register_index * 4 + 2, false);
                    break;

                case 2:
                    dr7.set(16 + register_index * 4 + 3, false);
                    dr7.set(16 + register_index * 4 + 2, true);
                    break;

                case 8:
                    dr7.set(16 + register_index * 4 + 3, true);
                    dr7.set(16 + register_index * 4 + 2, false);
                    break;

                case 4:
                    dr7.set(16 + register_index * 4 + 3, true);
                    dr7.set(16 + register_index * 4 + 2, true);
                    break;

                default:
                    return breakpoint::make_failed(result::bad_size);
            }

            std::memcpy(&ctx.Dr7, &dr7, sizeof(ctx.Dr7));

            return breakpoint { static_cast<char>(register_index), result::success };
        },
        [](auto failure_code)
        {
            return breakpoint::make_failed(failure_code);
        });
    }

    void dbgbreak::remove(const breakpoint& bp)
    {
        if (bp.error != result::success)
        {
            return;
        }

        dbgbreak::detail::update_thread_context([&](CONTEXT& ctx, const std::array<bool, 4>&) -> breakpoint
        {
            std::bitset<sizeof(ctx.Dr7) * 8> dr7;
            std::memcpy(&dr7, &ctx.Dr7, sizeof(ctx.Dr7));

            dr7.set(bp.register_index * 2, false); // flag to enable 'local' debugging for each of 4 registers. second bit is for global debugging, not working.

            std::memcpy(&ctx.Dr7, &dr7, sizeof(ctx.Dr7));

            return breakpoint { -1, result::uninited };
        },
        [](auto failure_code)
        {
            return breakpoint::make_failed(failure_code);
        });
    }
#else
    template <typename TAction, typename TFailure> static auto dbgbreak::detail::update_thread_context(TAction action, TFailure failure)
    {
    }

    dbgbreak::breakpoint dbgbreak::set(const void* on_pointer, std::uint8_t size, when when)
    {
        return breakpoint::make_failed(result::unsupported_platform);
    }

    void dbgbreak::remove(const breakpoint& bp)
    {
    }
#endif
}
