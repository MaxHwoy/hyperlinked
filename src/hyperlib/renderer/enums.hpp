#pragma once

#include <cstdint>

namespace hyper
{
    enum class model_lod : std::uint32_t
    {
        a,
        b,
        c,
        d,
        count,
    };

    enum class visible_state : std::uint8_t
    {
        outside,
        partial,
        inside,
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
        first = 0u,
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
        first = 0u,
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

    enum class draw_flags : std::uint32_t
    {
        fully_visible            = 1u << 2,



        always_facing            = 1u << 9,
        render_ai_npc            = 1u << 10,
        render_ai_racer          = 1u << 11,



        cast_shadows             = 1u << 16,
        sky_shade                = 1u << 17,
        inverted_culling         = 1u << 18,
        dynamic_placement        = 1u << 19,

        high_quality             = 1u << 22,
        dont_receive_shadows     = 1u << 23,

        use_ghost_shader         = 1u << 25,

        dont_fix_replacement_tex = 1u << 31,

        use_low_lod              = render_ai_npc | render_ai_racer,
    };

    CREATE_ENUM_EXPR_OPERATORS(model_lod);
    CREATE_ENUM_EXPR_OPERATORS(view_id);
    CREATE_ENUM_EXPR_OPERATORS(render_target_id);
    CREATE_ENUM_FLAG_OPERATORS(draw_flags);
}
