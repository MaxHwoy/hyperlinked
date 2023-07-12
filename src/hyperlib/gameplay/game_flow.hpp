#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class game_flow final
    {
    public:
        enum class state : std::uint32_t
        {
            none = 0x0,
            loading_frontend = 0x1,
            unloading_frontend = 0x2,
            in_frontend = 0x3,
            loading_region = 0x4,
            loading_track = 0x5,
            racing = 0x6,
            unloading_track = 0x7,
            unloading_region = 0x8,
            exit_demo_disc = 0x9,
        };

        struct manager
        {
            void(__cdecl* single_function)(std::int32_t);
            std::int32_t single_function_param;
            const char* single_function_name;
            void(__cdecl* looping_function)();
            const char* looping_function_name;
            bool waiting_for_callback;
            const char* callback_name;
            int callback_phase;
            state current_state;

            static inline manager& instance = *reinterpret_cast<game_flow::manager*>(0x00A99B9C);
        };
    };

    ASSERT_SIZE(game_flow::manager, 0x24);
}
