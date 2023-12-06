#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/vault/attrib.hpp>

namespace hyper
{
    class time_of_day final
    {
    public:
        enum class type : std::int32_t
        {
            error = -1,
            midday,
            sunset,
            count,
        };

        struct __declspec(align(0x10)) params
        {
            color diffuse_color;
            color ambient_color;
            color specular_color;
            color car_shadow_color;
            float car_spec_scale;
            float env_sky_brightness;
            std::int32_t pad[2];
            color fog_sky_color;
            color fog_haze_color;
            float fog_sky_color_scale;
            float fog_haze_color_scale;
            float fog_in_light_scatter;
            float fog_sun_falloff;
            float fog_distance_scale;
        };

    public:
        time_of_day();

        void update(float intensity);

        void update_time();

    public:
        static void init();

    public:
        float update_rate;
        std::int32_t update_direction;
        float current_time_of_day;
        float sun_azimuth;
        float latitude;
        params current;
        color some_color;
        attrib::gen::timeofdaylighting sunny;
        attrib::gen::timeofdaylighting dusk_sunny;
        attrib::gen::timeofdaylighting overcast;
        attrib::gen::timeofdaylighting dusk_overcast;
        attrib::gen::timeofdaylighting _0xA01A3942;
        attrib::gen::timeofdaylighting _0x0E1C797C;
        attrib::gen::timeofdaylighting _0x94ABDFF0;
        attrib::gen::timeofdaylighting _0x919B6689;
        float cloud_intensity;
        float cloud_desired;
        std::int32_t pad[2];
        vector4 sun_position;
        vector4 sun_direction;

    public:
        static inline time_of_day*& instance = *reinterpret_cast<time_of_day**>(0x00B77F34);
    };

    ASSERT_SIZE(time_of_day::params, 0x90);
    ASSERT_SIZE(time_of_day, 0x170);
}
