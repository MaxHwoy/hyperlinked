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

        static inline std::uint32_t& visual_treatment = *reinterpret_cast<std::uint32_t*>(0x00A65394);

        static inline std::int32_t& shader_detail = *reinterpret_cast<std::int32_t*>(0x00A63E60);

        static inline bool& motion_blur_enabled = *reinterpret_cast<bool*>(0x00A65360);

        static inline std::uint32_t& world_detail = *reinterpret_cast<std::uint32_t*>(0x00A65370);

        static inline std::uint32_t& shadow_detail = *reinterpret_cast<std::uint32_t*>(0x00A65398);

        static inline bool& rain_enabled = *reinterpret_cast<bool*>(0x00A65380);

        static inline bool& sky_enabled = *reinterpret_cast<bool*>(0x00A73358);

        static inline bool& particles_enabled = *reinterpret_cast<bool*>(0x00A6536C);

        static inline bool& bone_animations_enabled = *reinterpret_cast<bool*>(0x00A65354);

        static inline std::uint32_t& road_reflection_detail = *reinterpret_cast<std::uint32_t*>(0x00A65358);

        static inline bool& envmap_textures_enabled = *reinterpret_cast<bool*>(0x00A63E77);

        static inline std::uint32_t& brightness = *reinterpret_cast<std::uint32_t*>(0x00B1EBA0);

        static inline std::uint32_t& vehicle_reflection_rate = *reinterpret_cast<std::uint32_t*>(0x00A65340);

        static inline std::uint32_t& max_vehicle_reflection_rate = *reinterpret_cast<std::uint32_t*>(0x00A653A8);

        static inline std::uint32_t& rain_supported = *reinterpret_cast<std::uint32_t*>(0x00A653E8);

        static inline std::uint32_t& motion_blur_supported = *reinterpret_cast<std::uint32_t*>(0x00A653C8);

        static inline std::uint32_t& texture_filter_supported = *reinterpret_cast<std::uint32_t*>(0x00A653EC);

        static inline std::uint32_t& bloom_effect_supported = *reinterpret_cast<std::uint32_t*>(0x00A653FC);

        static inline std::uint32_t& texture_blend_supported = *reinterpret_cast<std::uint32_t*>(0x00AB0B00);

        static inline std::uint32_t& bone_animations_supported = *reinterpret_cast<std::uint32_t*>(0x00A653BC);

        static inline bool& texture_animations_enabled = *reinterpret_cast<bool*>(0x00A65368);
        
        static inline bool& flares_and_streaks_enabled = *reinterpret_cast<bool*>(0x00A65364);

        static inline bool& nis_anim_scene_disabled = *reinterpret_cast<bool*>(0x00AB0A9C);

        static inline bool& car_guys_camera = *reinterpret_cast<bool*>(0x00A9E6C8);

        static inline array<char, 260u> screenshot_directory = array<char, 260u>(0x00AB0738);
    };
}
