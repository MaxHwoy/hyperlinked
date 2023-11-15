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

        static inline std::int32_t& world_time_frames = *reinterpret_cast<std::int32_t*>(0x00A62F04);

        static inline float& world_time_elapsed = *reinterpret_cast<float*>(0x00A996F8);

        static inline std::uint32_t& real_time_packed = *reinterpret_cast<std::uint32_t*>(0x00A99C60);

        static inline std::uint32_t& world_time_packed = *reinterpret_cast<std::uint32_t*>(0x00A99C3C);

        static inline std::uint32_t& allow_duplicate_solids = *reinterpret_cast<std::uint32_t*>(0x00A8FF90);

        static inline std::uint32_t& duplicate_texture_warning_enabled = *reinterpret_cast<std::uint32_t*>(0x00A5E008);

        static inline bool& disable_fixup_tables = *reinterpret_cast<bool*>(0x00A8FF94);

        static inline bool& dirty_solids = *reinterpret_cast<bool*>(0x00A8FF98);

        static inline bool& dirty_textures = *reinterpret_cast<bool*>(0x00A8FF9C);

        static inline bool& dirty_animations = *reinterpret_cast<bool*>(0x00A8FFA0);
    };
}
