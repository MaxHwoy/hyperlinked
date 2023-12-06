#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/lights.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class light_renderer final
    {
    public:
        struct query
        {
            const light::instance* lights[0x18];
            float influences[0x18];
            color ambience;
        };

    private:
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
        static void make_world_space_light_direction(vector3& direction, const light::shaper& light);

        static void make_camera_space_light_direction(vector3& direction, const light::shaper& light, const matrix4x4& rotation);

        static void make_sun_direction_light_direction(vector3& direction, const light::shaper& light);

        static void make_inverse_sun_direction_light_direction(vector3& direction, const light::shaper& light);

        static void make_world_position_light_direction(vector3& direction, const light::shaper_rigorous& shaper, const vector3& inst_pos);

        static void make_light_direction_from_angles(vector3& direction, float theta, float phi, float scale);

        static void make_light_direction_from_sun(vector3& direction, const vector3& sun);

        static void query_light_internal(query& query, query_helper& helper, const light::instance& light, const vector3& position);

        static void update_irradiance(irradiance& radiance, const vector3& direction, const color& light_color);

        static void update_irradiance(irradiance& radiance, const color& light_color, float scale, const vector3& direction);

        static void normalize_irradiance(irradiance& radiance, bool normalize_rgb, float scale_rgb, bool normalize_alpha, float scale_alpha);

        static void transform_irradiance(irradiance& radiance, const matrix4x4& rotation);

    public:
        static void reset_light_context(light::context::dynamic& context);

        static bool setup_light_context(light::context::dynamic& context, const light::shaper_rigorous& shaper, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base);

        static bool clone_light_context(light::context::dynamic& dst, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base, const light::context::dynamic& src);

        static void rotate_light_context(light::context::dynamic& dst, const light::context::dynamic& src, const matrix4x4& inst_trs);

        static bool setup_envmap(light::context::dynamic& context, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center);

        static void setup_lights(light::context::dynamic& context, const light::shaper_rigorous& shaper, const vector3& inst_pos, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const view::base* base);

        static auto query_light_database(query& query, const vector3& position) -> std::uint32_t;

    private:
        static inline bool& light_query_ignore_distance = *reinterpret_cast<bool*>(0x00B42EE0);

        static inline bool& enable_echo_irradiance = *reinterpret_cast<bool*>(0x00B42EE4);

        static inline bool& normalize_rgb_irradiance = *reinterpret_cast<bool*>(0x00B42EC0);

        static inline bool& normalize_alpha_irradiance = *reinterpret_cast<bool*>(0x00A6B918);

        static inline float& scale_rgb_irradiance = *reinterpret_cast<float*>(0x00A6B914);

        static inline float& scale_alpha_irradiance = *reinterpret_cast<float*>(0x00A6B91C);

    public:
        static inline float& default_spec_power = *reinterpret_cast<float*>(0x00A6C224);

        static inline float& default_ingame_light_y = *reinterpret_cast<float*>(0x00A650E0);

        static inline float& default_ingame_light_w = *reinterpret_cast<float*>(0x007497CE);

        static inline float& ingame_envmap_pull_amount = *reinterpret_cast<float*>(0x00A6C228);

        static inline float& frontend_envmap_pull_amount = *reinterpret_cast<float*>(0x00B43030);

        static inline vector4& ingame_light_params = *reinterpret_cast<vector4*>(0x00A6C204);

        static inline vector4& frontend_light_params = *reinterpret_cast<vector4*>(0x00A6C214);

        ASSERT_SIZE(light_renderer::query, 0xD0);
        ASSERT_SIZE(light_renderer::query_helper, 0xD0);
        ASSERT_SIZE(light_renderer::irradiance, 0xA0);
        static_assert(sizeof(light_renderer::irradiance) == sizeof(light::context::dynamic::harmonics));
    };
}
