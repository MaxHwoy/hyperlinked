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
        public:
            std::uint32_t key;
            shaper_light lights[4];
            vector3 padv3;
            vector3 position;
            std::uint32_t padi4;
            std::uint32_t override_slot_count;

        public:
            static inline shaper_light_rigorous& world = *reinterpret_cast<shaper_light_rigorous*>(0x00A6C4A0);
        };

        struct dynamic_light : public linked_node<dynamic_light>, public light::instance
        {
        };

        struct dynamic_light_pack : linked_node<dynamic_light_pack>
        {
        public:
            char name[0x20];
            bool enabled;
            linked_list<dynamic_light> lights;

        public:
            static inline linked_list<dynamic_light_pack>& list = *reinterpret_cast<linked_list<dynamic_light_pack>*>(0x00B4CF20);
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
            const light::instance* lights[0x18];
            float influences[0x18];
            color ambience;
        };

        struct __declspec(align(0x08)) query_helper
        {
            color ambient_colors[11];
            color* root_ambient_colour;
            color* current_ambient_colour;
            color* ambient_sentinel;
            float total_ambient_influence;
            float smallest_root_ambient_distance;
            float total_light_influence;
            std::uint32_t light_hits;
        };

        struct irradiance
        {
            float coeffs[10][4];
        };

    private:
        static void make_world_space_light_direction(vector3& direction, const shaper_light& light);

        static void make_camera_space_light_direction(vector3& direction, const shaper_light& light, const matrix4x4& rotation);

        static void make_sun_direction_light_direction(vector3& direction, const shaper_light& light);

        static void make_inverse_sun_direction_light_direction(vector3& direction, const shaper_light& light);

        static void make_world_position_light_direction(vector3& direction, const shaper_light_rigorous& shaper, const vector3& inst_pos);

        static void make_light_direction_from_angles(vector3& direction, float theta, float phi, float scale);

        static void make_light_direction_from_sun(vector3& direction, const vector3& sun);

        static void query_light_internal(query& query, query_helper& helper, const light::instance& light, const vector3& position);

        static void update_irradiance(irradiance& radiance, const vector3& direction, const color& light_color);

        static void update_irradiance(irradiance& radiance, const color& light_color, float scale, const vector3& direction);

        static void normalize_irradiance(irradiance& radiance, bool normalize_rgb, float scale_rgb, bool normalize_alpha, float scale_alpha);

        static void transform_irradiance(irradiance& radiance, const matrix4x4& rotation);

    public:
        static void reset_light_context(dynamic_context& context);

        static bool setup_light_context(dynamic_context& context, const shaper_light_rigorous& shaper, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base);

        static bool clone_light_context(dynamic_context& dst, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base, const dynamic_context& src);

        static void rotate_light_context(dynamic_context& dst, const dynamic_context& src, const matrix4x4& inst_trs);

        static bool setup_envmap(dynamic_context& context, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center);

        static void setup_lights(dynamic_context& context, const shaper_light_rigorous& shaper, const vector3& inst_pos, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const view::base* base);

        static auto query_light_database(query& query, const vector3& position) -> std::uint32_t;

    private:
        static inline bool& light_query_ignore_distance = *reinterpret_cast<bool*>(0x00B42EE0);

        static inline bool& enable_echo_irradiance = *reinterpret_cast<bool*>(0x00B42EE4);

        static inline bool& normalize_rgb_irradiance = *reinterpret_cast<bool*>(0x00B42EC0);

        static inline bool& normalize_alpha_irradiance = *reinterpret_cast<bool*>(0x00A6B918);

        static inline float& scale_rgb_irradiance = *reinterpret_cast<float*>(0x00A6B914);

        static inline float& scale_alpha_irradiance = *reinterpret_cast<float*>(0x00A6B91C);

    public:
        static inline float& default_ingame_light_y = *reinterpret_cast<float*>(0x00A650E0);

        static inline float& default_ingame_light_w = *reinterpret_cast<float*>(0x007497CE);

        static inline vector4& ingame_light_params = *reinterpret_cast<vector4*>(0x00A6C204);

        static inline vector4& frontend_light_params = *reinterpret_cast<vector4*>(0x00A6C214);
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

    static_assert(sizeof(lighting::irradiance) == sizeof(lighting::dynamic_context::harmonics));
}
