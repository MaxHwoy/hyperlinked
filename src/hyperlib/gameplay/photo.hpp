#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class photo final
    {
    public:
        enum class state : std::uint32_t
        {
            none,
            initializing,
            initialized,
        };

        class listener
        {
            virtual void on_photo_ready() = 0;
        };

        class mode final
        {
        public:
            auto get_state() -> state;

            void take_photo();

        private:
            state state_;
            span<char> buffer_;
            listener* listener_;
            void* buffer_handle_;

        public:
            static inline mode& instance = *reinterpret_cast<mode*>(0x00B479DC);
        };
    };

    ASSERT_SIZE(photo::mode, 0x14);
}
