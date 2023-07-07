#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>

namespace hyper
{
    enum class visible_state : std::uint8_t
    {
        outside = 0x0,
        partial = 0x1,
        inside = 0x2,
    };

    enum class poly_flags : std::uint8_t
    {
        apply_aspect           = 1u << 0,
        apply_z_sort           = 1u << 1,
        multi_text_mas         = 1u << 2,
        no_tex_fix             = 1u << 3,
        use_native_screen_size = 1u << 4,
    };

    enum class view_mode : std::uint32_t
    {
        none,
        one,
        one_rvm,
        two_h,
        two_v,
        quad,
    };

    enum class render_target_id : std::uint32_t
    {
        visual_treatment,
        player,
        reflection,
        flailer,
        rvm,
        shadowmap,
        pip,
        motion_blur,
        env_x_pos,
        env_x_neg,
        env_y_pos,
        env_y_neg,
        env_z_pos,
        env_z_neg,
        count,
    };

    enum class view_id : std::uint32_t
    {
        flailer,
        player1,
        player2,
        player1_rvm,
        player1_reflection,
        player2_reflection,
        player1_glow,
        player2_glow,
        player1_pip,
        player1_headlight,
        player2_headlight,
        quadrant_top_left,
        quadrant_top_right,
        quadrant_bottom_left,
        quadrant_bottom_right,
        hdr_scene,
        player1_shadowmap,
        player2_shadowmap,
        player1_shadowproj,
        player2_shadowproj,
        lightstreaks,
        shadowmatte,
        env_z_pos,
        env_x_pos,
        env_z_neg,
        env_x_neg,
        env_y_pos,
        env_y_neg,
        count,
    };

    enum class camera_mover_types : std::uint32_t
    {
        none,
        drive_cubic,
        debug_world,
        road_editor,
        orbit_car,
        rear_view_mirror,
        track_car,
        max,
        select_car,
        still,
        race_start,
        zone_freeze,
        zone_preview,
        auto_pilot,
        ice,
        animation_controller,
        cop_view,
        animation_entity,
        showcase,
        pip,
        count,
    };

    enum class screen_effect_control : std::uint32_t
    {
        frame,
        boolean,
        function,
    };

    enum class screen_effect_type : std::uint32_t
    {
        tint,
        motion_blur,
        visual_sig,
        glare,
        fe_blur,
        count,
    };

    enum class curtain_status : std::uint32_t
    {
        inactive,
        active,
        turn_on,
        override,
    };

    enum class rain_wind_type : std::uint32_t
    {
        point,
        vector,
        count,
    };

    struct render_target
    {
        render_target_id target_id;
        view_id view_id;
        IDirect3DSurface9* d3d_target;
        IDirect3DSurface9* d3d_depth_stencil;
        bool active;
        __declspec(align(0x04)) std::uint32_t resolution_x;
        __declspec(align(0x04)) std::uint32_t resolution_y;

        static inline auto targets = array<render_target, static_cast<size_t>(render_target_id::count)>(0x00AB04D0);
    };

    struct __declspec(align(0x10)) render_view
    {
        view_id id;
        bool has_camera;
        bool is_shadow_projection_view;
        IDirect3DSurface9* d3d_render_target;
        IDirect3DSurface9* d3d_depth_stencil_surface;
        __declspec(align(0x10)) vector3 camera_direction;
        __declspec(align(0x10)) vector3 camera_up_vector;
        __declspec(align(0x10)) vector3 camera_previous_direction;
        __declspec(align(0x10)) vector3 camera_position;
        __declspec(align(0x10)) vector3 camera_velocity;
        __declspec(align(0x10)) matrix4x4 camera_matrix;
        float camera_focal_distance;
        float camera_depth_of_field;
        float camera_dof_falloff;
        float camera_dof_max_intensity;
        bool has_outside_view;
        camera_mover_types camera_mover_type;
        bool is_hood_render_view;
        std::int32_t something_0xBC;
        matrix4x4 projection_matrix;
        matrix4x4 view_matrix;
        matrix4x4 view_projection_matrix;
        vector4 vector4_0x180;
        matrix4x4 non_jittered_projection_matrix;
        render_target* target;

        static inline auto views = array<render_view, static_cast<size_t>(view_id::count)>(0x00B47A90);
    };

    struct __declspec(align(0x10)) poly
    {
        vector3pad vertices[4];
        float uvs[4][2];
        float uvs_mask[4][2];
        color32 colors[4];
        poly_flags flags;
        char flailer;
    };

    struct __declspec(align(0x10)) camera_params
    {
        __declspec(align(0x10)) matrix4x4 matrix;
        __declspec(align(0x10)) vector3 position;
        __declspec(align(0x10)) vector3 direction;
        __declspec(align(0x10)) vector3 target;
        __declspec(align(0x10)) vector4 noise_frequency_1;
        __declspec(align(0x10)) vector4 noise_amplitude_1;
        __declspec(align(0x10)) vector4 noise_frequency_2;
        __declspec(align(0x10)) vector4 noise_amplitude_2;
        __declspec(align(0x10)) vector4 fade_color;
        float target_distance;
        float focal_distance;
        float depth_of_field;
        float dof_falloff;
        float dof_max_intensity;
        float near_clip;
        float far_clip;
        float lb_height;
        float sim_time_multiplier;
        std::uint16_t horizontal_fov;
    };

    struct __declspec(align(0x10)) camera
    {
        camera_params current_key;
        camera_params previous_key;
        camera_params velocity_key;
        bool clear_velocity;
        float elapsed_time;
        float last_update_time;
        float last_disparate_time;
        bool render_dash;
        float noise_intensity;
    };

    struct __declspec(align(0x04)) screen_effect_inf
    {
        screen_effect_control controller;
        std::uint32_t frame_num;
        bool active;
    };

    struct screen_effect_def
    {
        color color;
        float intensity;
        float data[14];
        void(__cdecl* update)(screen_effect_type, struct screen_effect_db*);
    };

    struct screen_effect_db
    {
        struct view_base* view;
        screen_effect_inf se_inf[5];
        screen_effect_def se_data[5];
        std::uint32_t num_type[5];
        float se_time;
    };

    struct face_pixelation
    {
        struct view_base* view;
        vector2 screen;
    };

    struct on_screen_rain_entry
    {
        float data[6];
        std::uint32_t index;
    };

    struct on_screen_rain
    {
        std::uint32_t entry_count;
        on_screen_rain_entry entries[20];
    };

    struct rain
    {
        on_screen_rain screen_rain;
        __declspec(align(0x04)) bool no_rain;
        __declspec(align(0x04)) bool no_rain_ahead;
        __declspec(align(0x04)) bool in_tunnel;
        __declspec(align(0x04)) bool in_overpass;
        __declspec(align(0x04)) struct track_path_zone* zone;
        char padding_0x0244[8];
        vector3pad out_vex;
        vector2 position;
        char padding_0x0268[8];
        vector3pad local_cam_velocity;
        curtain_status rain_curtain;
        std::uint32_t render_count;
        struct view* view;
        float rain_intensity;
        float cloud_intensity;
        std::uint32_t desired_active;
        std::uint32_t raindrop_count[2];
        vector3pad raindrop_positions[350][2];
        char old_swap_buffer[350][4];
        std::uint32_t rain_point_count;
        std::uint32_t num_of_type[2];
        std::uint32_t desired_num_of_type[2];
        float percentages[2];
        float precip_wind_effect[2][2];
        texture_info* textures[2];
        char padding_0x340C[4];
        vector3pad old_car_direction;
        vector3pad precip_radius[2];
        vector3pad precip_speed_range[2];
        float precip_z_constant[2];
        rain_wind_type wind_type[2];
        float camera_speed;
        char padding_0x3474[12];
        vector3pad wind_speed;
        vector3pad desired_wind_speed;
        float desired_wind_time;
        float wind_time;
        std::uint32_t fog_red;
        std::uint32_t fog_green;
        std::uint32_t fog_blue;
        vector2 bbox_max;
        vector2 bbox_min;
        char padding_0x34C4[12];
        poly precip_poly[2];
        matrix4x4 local_to_world;
        matrix4x4 world_to_local;
        float len_modifier;
        float desired_intensity;
        float desired_cloudyness;
        float desired_road_dampness;
        float road_dampness;
        float precip_percent[2];
        char padding_0x36AC[4];
        vector3pad prevail_wind_speed;
        float weather_time;
        float desired_weather_time;
        vector3pad velocities[2][10];
        vector2 ent0;
        vector2 ent1;
        vector2 ext0;
        vector2 ext1;
        char data_0x3828[6488];
    };

    class renderer final
    {
    public:
        static inline bool& draw_world = *reinterpret_cast<bool*>(0x00A63E0C);

        static inline std::uint32_t& shadow_detail = *reinterpret_cast<std::uint32_t*>(0x00A65398);

        static inline bool& road_reflection_enabled = *reinterpret_cast<bool*>(0x00A65358);

        static inline view_mode& mode = *reinterpret_cast<view_mode*>(0x00AB0A38);
    };

    DEFINE_ENUM_FLAG_OPERATORS(poly_flags);

    ASSERT_SIZE(render_target, 0x1C);
    ASSERT_SIZE(render_view, 0x1E0);
    ASSERT_SIZE(poly, 0xA0);
    ASSERT_SIZE(camera_params, 0xF0);
    ASSERT_SIZE(camera, 0x2F0);
    ASSERT_SIZE(screen_effect_inf, 0x0C);
    ASSERT_SIZE(screen_effect_def, 0x50);
    ASSERT_SIZE(screen_effect_db, 0x1E8);
    ASSERT_SIZE(on_screen_rain_entry, 0x1C);
    ASSERT_SIZE(on_screen_rain, 0x234);
    ASSERT_SIZE(rain, 0x5180);
}
