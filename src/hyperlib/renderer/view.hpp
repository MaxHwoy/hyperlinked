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

    struct clipping_planes
    {
        plane planes[static_cast<std::uint32_t>(clipping_plane_type::count)];
    };

    struct view_platform_info
    {
        matrix4x4 view_matrix;
        matrix4x4 projection_matrix;
        matrix4x4 view_projection_matrix;
        matrix4x4 non_jittered_projection_matrix;
        matrix4x4 non_jittered_view_projection_matrix;
        clipping_planes clipping_planes;

        static inline auto infos = array<view_platform_info, static_cast<size_t>(view_id::count)>(0x00B1A780);
    };

    struct view_platform_interface
    {
        view_platform_info* info;

        auto get_visible_state_sb(const vector3& point, const matrix4x4* trs) const -> visible_state;

        auto get_visible_state_sb(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> visible_state;
    };

    struct __declspec(align(0x10)) view_base : public view_platform_interface
    {
        const char* name;
        view_id id;
        bool active;
        std::int8_t letter_box;
        float pixelation;
        float near_z;
        float far_z;
        float fov_bias;
        float fov_degrees;
        std::uint16_t fov_angle;
        std::int32_t black_and_white_mode;
        std::int32_t pixel_min_size;
        vector3 view_direction;
        __declspec(align(0x10)) camera* camera;
        linked_list<struct camera_mover> camera_mover_list;
        struct dynamic_light_context* world_light_context;
        render_target* attached_target;
        char pad[0x0C];
    };

    struct __declspec(align(0x10)) view : public view_base
    {
        std::uint32_t num_cops_in_view;
        std::uint32_t num_cops_total;
        std::uint32_t num_cops_cherry;
        rain* rain;
        screen_effect_db* screen_effect;
        face_pixelation* face_pixelation;

        static inline auto views = array<view, static_cast<size_t>(view_id::count)>(0x00B4AF10);
    };

    ASSERT_SIZE(clipping_planes, 0x60);
    ASSERT_SIZE(view_platform_info, 0x1A0);
    ASSERT_SIZE(view_base, 0x60);
    ASSERT_SIZE(view, 0x80);
}
