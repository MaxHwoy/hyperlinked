#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct racer_info
    {
        char data[0x384];
    };

    class g_race final
    {
    public:
        enum class mode : std::uint32_t
        {
            none,
            grip,
            high_speed_challenge,
            drag,
            drift,
            count,
        };

        enum class context : std::uint32_t
        {
            quick_race,
            online,
            challenge,
            career,
        };

        struct status
        {
        public:
            enum class play_mode : std::uint32_t
            {
                roaming,
                racing,
            };

            static bool is_drift();

            static bool is_checkpoint();

            static bool is_racing();

            static bool is_in_pursuit();

        public:
            char vtables[0x18];
            racer_info racer_infos[30];
            racer_info* lazy_rankings[30];
            std::uint32_t racer_count;
            float race_handicap;
            bool is_loading;
            play_mode mode;
            context context;
            char data[0x55B0];

            static inline status*& instance = *reinterpret_cast<status**>(0x00A98284);
        };
    };

    ASSERT_SIZE(racer_info, 0x384);
    ASSERT_SIZE(g_race::status, 0xBFCC);
}
