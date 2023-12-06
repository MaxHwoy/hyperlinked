#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct unplug_allowable
    {
        virtual bool is_unplug_message_allowed() = 0;
        virtual ~unplug_allowable() = default;
    };

    class movie final : public unplug_allowable
    {
    public:
        enum class status : std::uint32_t
        {
            completed,
            interrupted,
            error,
            starting,
            shutting,
            playing,
            count,
        };

    public:
        bool is_playing();

    private:
        std::uint32_t volume_;
        std::uint32_t buffer_size_;
        std::uint32_t active_controller_;
        std::int32_t movie_id_;
        std::int32_t idk_;
        bool preload_;
        bool sound_;
        bool loop_;
        bool pal_;
        char name_[0x100];
        std::uint32_t paused_;
        status curr_status_;
        status live_status_;
        char data_38_[0x34];

    public:
        static inline movie*& instance = *reinterpret_cast<movie**>(0x00A97BB4);
    };

    ASSERT_SIZE(movie, 0x15C);
}
