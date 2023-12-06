#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/vault/attrib.hpp>

namespace hyper
{
    class visual_treatment
    {
    public:
        enum class look : std::uint32_t
        {
            heat,
            cop_cam,
            fe,
        };

        struct look_effect
        {
            attrib::gen::visuallookeffect* effect;
            float start_time;
            float pulse_length;
            bool use_world_time;
            bool stop_if_heat_falls;
            bool stop_after_length;
            bool pad;
        };

        struct look_effect_target
        {
            attrib::gen::visuallookeffect* effect;
            float start_world_time;
            float current;
            float target;
        };

    public:
        look state;
        float desaturation;
        float uves_value;
        std::uint32_t pad1;
        color fade_color;
        color fade_color_over_cliff;
        float pulse_brightness;
        float radial_blur;
        float pursuit_breaker_blend;
        float nos_radial_blur_amount;
        float over_cliff_fade_start_time;
        float over_cliff_fade_delay_start_time;
        attrib::gen::visuallook uves_visual_look;
        attrib::gen::visuallook cop_cam_visual_look;
        look_effect_target* uves_pulse;
        look_effect_target* uves_radialblur;
        look_effect* uves_transition;
        look_effect* camera_flash;
        look_effect* pursuitbreaker;
        look_effect* nos_radialblur;
        float current_target;
        float desaturation_target;
        float color_bloom_intensity_target;
        float heat_neter;
        float blur_amount;
        std::uint32_t is_being_pursued;
        bool uves_over_cliff_enabled;
        bool light_streaks_enabled;
        struct light_streak* light_streaks[3];
        vector2 gradient_radius;
        texture::info* uves_vignette_texture;
        vector2 gradient;
        std::uint32_t pad2;
        vector3pad gradient_rgbs;

    public:
        static inline visual_treatment*& instance = *reinterpret_cast<visual_treatment**>(0x00AB0B80);
    };

    const int a = sizeof(visual_treatment);

    ASSERT_SIZE(visual_treatment::look_effect, 0x10);
    ASSERT_SIZE(visual_treatment::look_effect_target, 0x10);
    ASSERT_SIZE(visual_treatment, 0xD0);
}
