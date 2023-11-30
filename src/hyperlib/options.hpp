#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class options final
    {
    public:
        static inline std::uint32_t& texture_filtering = *reinterpret_cast<std::uint32_t*>(0x00A65384);

        static inline bool& allow_antialias = *reinterpret_cast<bool*>(0x00A63D2C);

        static inline std::int32_t& antialias_level = *reinterpret_cast<std::int32_t*>(0x00A6537C);

        static inline bool& vsync_on = *reinterpret_cast<bool*>(0x00A65390);

        static inline bool& visual_treatment = *reinterpret_cast<bool*>(0x00A65394);

        static inline std::int32_t& shader_detail = *reinterpret_cast<std::int32_t*>(0x00A63E60);

        static inline bool& motion_blur_enabled = *reinterpret_cast<bool*>(0x00A65360);

        static inline std::uint32_t& world_detail = *reinterpret_cast<std::uint32_t*>(0x00A65370);

        static inline std::uint32_t& shadow_detail = *reinterpret_cast<std::uint32_t*>(0x00A65398);

        static inline bool& rain_enabled = *reinterpret_cast<bool*>(0x00A65380);

        static inline bool& road_reflection_enabled = *reinterpret_cast<bool*>(0x00A65358);

        static inline bool& envmap_textures_enabled = *reinterpret_cast<bool*>(0x00A63E77);

        static inline std::uint32_t& vehicle_reflection_rate = *reinterpret_cast<std::uint32_t*>(0x00A65340);

        static inline std::uint32_t& max_vehicle_reflection_rate = *reinterpret_cast<std::uint32_t*>(0x00A653A8);

        static inline std::uint32_t& pc_rain_option = *reinterpret_cast<std::uint32_t*>(0x00A653E8);

        static inline std::uint32_t& pc_motion_blur_option = *reinterpret_cast<std::uint32_t*>(0x00A653C8);
    };
}
