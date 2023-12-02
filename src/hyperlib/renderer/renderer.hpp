#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class render_target;

    class renderer final
    {
    private:
        static bool reinit_window_params();

        static void init_render_targets();

        static void open_render_targets();

        static void close_render_targets();

        static void set_render_target(render_target& target, bool clear, ::D3DCOLOR clear_color);

        static void setup_car_env_map();

        static void update_view_instances();

        static void update_render_views();

        static void update_headlight_views();

        static void update_world_lights();

        static void update_animations();

        static void reset_renderer_state();

        static bool wants_renderer_reset();

        static void begin_scene();

        static void end_scene();

        static void render_world();

        static void render_fe();

        static void wait_render();

        static void make_screenshot(const char* directory);

        static void handle_cursor();

    public:
        static bool reset();

        static void render();

    public:
        static inline bool& cull_backfaces = *reinterpret_cast<bool*>(0x00A650E4);

        static inline ::D3DCULL& current_cull_mode = *reinterpret_cast<::D3DCULL*>(0x00A6523C);

        static inline bool& envmap_calibration = *reinterpret_cast<bool*>(0x00AB0AA8);

        static inline bool& reinit_renderer = *reinterpret_cast<bool*>(0x00AB0B25);

        static inline bool& time_of_day_not_inited = *reinterpret_cast<bool*>(0x00A65578);

        static inline bool& take_screenshot = *reinterpret_cast<bool*>(0x00AB0AE0);

        static inline bool& shadow_map_cull = *reinterpret_cast<bool*>(0x00AB0A34);

        static inline matrix4x4*& shadow_map_trs = *reinterpret_cast<matrix4x4**>(0x00AB0C18);
    };
}
