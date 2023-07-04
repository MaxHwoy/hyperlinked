#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    enum class view_id;

    enum render_target_id
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

    enum class camera_mover_types
    {
        none = 0x0,
        drive_cubic = 0x1,
        debug_world = 0x2,
        road_editor = 0x3,
        orbit_car = 0x4,
        rear_view_mirror = 0x5,
        track_car = 0x6,
        max = 0x7,
        select_car = 0x8,
        still = 0x9,
        race_start = 0xA,
        zone_freeze = 0xB,
        zone_preview = 0xC,
        auto_pilot = 0xD,
        ice = 0xE,
        animation_controller = 0xF,
        cop_view = 0x10,
        animation_entity = 0x11,
        showcase = 0x12,
        pip = 0x13,
        last_mover_type = 0x13,
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
        bool IsHoodRenderView;
        std::int32_t something_0xBC;
        matrix4x4 projection_matrix;
        matrix4x4 view_matrix;
        matrix4x4 view_projection_matrix;
        vector4 vector4_0x180;
        matrix4x4 non_jittered_projection_matrix;
        render_target* target;
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
        std::int32_t last_disparate_time;
        std::int32_t render_dash;
        float noise_intensity;
    };

    ASSERT_SIZE(render_target, 0x1C);
    ASSERT_SIZE(render_view, 0x1E0);
    ASSERT_SIZE(camera, 0x2F0);
}
