#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/lights.hpp>
#include <hyperlib/vault/attrib.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class lighting final
    {
    public:
        enum class time_of_day_type : std::int32_t
        {
            error = -1,
            midday,
            sunset,
            count,
        };

        enum class shaper_light_mode : std::int32_t
        {
            invalid = -1,
            world_space,
            camera_space,
            sun_direction,
            inverse_sun_direction,
            world_position,
            count,
        };

        struct time_of_day
        {
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
                float fog_inLight_scatter;
                float fog_sun_falloff;
                float fog_distance_scale;
            };

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
            float overcast_value;
            std::int32_t pad[3];
            vector4 sun_position;
            vector4 sun_direction;

            static inline time_of_day*& instance = *reinterpret_cast<time_of_day**>(0x00B77F34);
        };

        struct shaper_light
        {
            shaper_light_mode mode;
            float theta;
            float phi;
            color color;
        };

        struct __declspec(align(0x10)) shaper_light_rigorous
        {
            std::uint32_t key;
            shaper_light lights[4];
            vector3 padv3;
            vector3 position;
            std::uint32_t padi4;
            std::uint32_t override_slot_count;
        };

        struct dynamic_light : public linked_node<dynamic_light>, public light::instance
        {
        };

        struct dynamic_light_pack : linked_node<dynamic_light_pack>
        {
            char name[0x20];
            bool enabled;
            linked_list<dynamic_light> dynamic_light_list;
        };

        struct context
        {
            std::int32_t type;
        };

        struct dynamic_context : public context
        {
            vector3 padv3;
            matrix4x4 local_color;
            matrix4x4 local_direction;
            matrix4x4 local_eye;
            vector4 envmap_pos;
            vector4 idk_vector4;
            matrix4x4 envmap_rotation;
            vector4 harmonics[10];
        };

        struct particle_context : public context
        {
            vector3 padv3;
            vector4 color;
        };

        struct scenery_context : public context
        {
            char name[0x22];
            std::int16_t context_number;
            matrix4x4* local_lights;
            std::uint32_t light_count;
        };

        struct query
        {
            light::instance* lights[0x18];
            float influences[0x18];
            color ambience;
        };

        struct __declspec(align(0x08)) query_helper
        {
            color ambient_colors[11];
            vector4* root_ambient_colour;
            vector4* current_ambient_colour;
            vector4* ambient_sentinel;
            float total_ambient_influence;
            float smallest_root_ambient_distance;
            float total_light_influence;
            std::uint32_t light_hits;
        };

        struct irradiance
        {
            color coeffs[10];
        };

    private:
        static void make_world_space_light_direction(vector3& direction, const shaper_light& light);

        static void make_camera_space_light_direction(vector3& direction, const shaper_light& light, const matrix4x4& rotation);

        static void make_sun_direction_light_direction(vector3& direction, const shaper_light& light);

        static void make_inverse_sun_direction_light_direction(vector3& direction, const shaper_light& light);

        static void make_world_position_light_direction(vector3& direction, const shaper_light_rigorous& shaper, const vector3& inst_pos);

        static void make_light_direction_from_angles(vector3& direction, float theta, float phi, float scale);

        static void make_light_direction_from_sun(vector3& direction, const vector3& sun);

    public:
        static bool setup_light_context(dynamic_context& context, const shaper_light_rigorous& shaper, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base);

        static bool clone_light_context(dynamic_context& dst, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base, const dynamic_context& src);

        static void rotate_light_context(dynamic_context& dst, const dynamic_context& src, const matrix4x4& inst_trs);

        static bool setup_envmap(dynamic_context& context, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center);

        static void setup_lights(dynamic_context& context, const shaper_light_rigorous& shaper, const vector3& inst_pos, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const view::base* base);
    };

    ASSERT_SIZE(lighting::time_of_day, 0x170);
    ASSERT_SIZE(lighting::time_of_day::params, 0x90);
    ASSERT_SIZE(lighting::shaper_light, 0x1C);
    ASSERT_SIZE(lighting::shaper_light_rigorous, 0xA0);
    ASSERT_SIZE(lighting::dynamic_light, 0x68);
    ASSERT_SIZE(lighting::dynamic_light_pack, 0x34);
    ASSERT_SIZE(lighting::context, 0x04);
    ASSERT_SIZE(lighting::dynamic_context, 0x1D0);
    ASSERT_SIZE(lighting::particle_context, 0x20);
    ASSERT_SIZE(lighting::scenery_context, 0x30);
    ASSERT_SIZE(lighting::query, 0xD0);
    ASSERT_SIZE(lighting::query_helper, 0xD0);
    ASSERT_SIZE(lighting::irradiance, 0xA0);
}
