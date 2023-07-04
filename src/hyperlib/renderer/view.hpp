#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    enum class clipping_plane_type : std::uint32_t
    {
        left,
        right,
        bottom,
        top,
        near,
        far,
        count,
    };

    enum class view_id : std::uint32_t
    {
        flailer = 0x0,
        player1 = 0x1,
        player2 = 0x2,
        player1_rvm = 0x3,
        player1_reflection = 0x4,
        player2_reflection = 0x5,
        player1_glow = 0x6,
        player2_glow = 0x7,
        player1_pip = 0x8,
        player1_headlight = 0x9,
        player2_headlight = 0xA,
        quadrant_top_left = 0xB,
        quadrant_top_right = 0xC,
        quadrant_bottom_left = 0xD,
        quadrant_bottom_right = 0xE,
        hdr_scene = 0xF,
        player1_shadowmap = 0x10,
        player2_shadowmap = 0x11,
        player1_shadowproj = 0x12,
        player2_shadowproj = 0x13,
        lightstreaks = 0x14,
        shadowmatte = 0x15,
        env_z_pos = 0x16,
        env_x_pos = 0x17,
        env_z_neg = 0x18,
        env_x_neg = 0x19,
        env_y_pos = 0x1A,
        env_y_neg = 0x1B,
        count,
    };

    enum class view_mode : std::uint32_t
    {
        none = 0x0,
        one = 0x1,
        one_rvm = 0x2,
        two_h = 0x3,
        two_v = 0x4,
        quad = 0x5,
    };

    struct clipping_planes
    {
        vector4 planes[6];
    };

    struct view_platform_info
    {
        matrix4x4 view_matrix;
        matrix4x4 projection_matrix;
        matrix4x4 view_projection_matrix;
        matrix4x4 non_jittered_projection_matrix;
        matrix4x4 non_jittered_view_projection_matrix;
        clipping_planes clipping_planes;
    };

    struct view_platform_interface
    {
        view_platform_info* info;
    };

    struct view : public view_platform_interface
    {
        const char* name;
        view_id id;
        bool active;
        std::int8_t letter_box;
        std::int8_t pad0;
        std::int8_t pad1;
        float pixelation;
        float near_z;
        float far_z;
        float fov_bias;
        float fov_degrees;
        std::uint16_t fov_angle;
        std::int32_t black_and_white_mode;
        std::int32_t pixel_min_size;
        vector3 view_direction;
        __declspec(align(0x10)) struct camera* camera;
        linked_list<struct camera_mover> camera_mover_list;
        struct dynamic_light_context* world_light_context;
        render_target* render_target_table;
        char rest[12];
    };

    ASSERT_SIZE(clipping_planes, 0x60);
    ASSERT_SIZE(view_platform_info, 0x1A0);
    ASSERT_SIZE(view, 0x60);
}
