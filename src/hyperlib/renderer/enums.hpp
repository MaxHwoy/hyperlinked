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
        outside = 0x0,
        partial = 0x1,
        inside = 0x2,
    };

    enum class poly_flags : std::uint8_t
    {
        apply_aspect = 1u << 0,
        apply_z_sort = 1u << 1,
        multi_text_mas = 1u << 2,
        no_tex_fix = 1u << 3,
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
}
