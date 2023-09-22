#pragma once

#include <cstdint>

namespace hyper
{
    class global final
    {
    public:
        static inline bool& should_exit_game = *reinterpret_cast<bool*>(0x00A99560);

        static inline std::uint32_t& frame_counter = *reinterpret_cast<std::uint32_t*>(0x00AB0A78);

        static inline std::int32_t& real_time_frames = *reinterpret_cast<std::int32_t*>(0x00A62EE8);

        static inline std::uint32_t& allow_duplicate_solids = *reinterpret_cast<std::uint32_t*>(0x00A8FF90);

        static inline std::uint32_t& duplicate_texture_warning_enabled = *reinterpret_cast<std::uint32_t*>(0x00A5E008);
    };
}
