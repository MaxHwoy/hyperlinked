#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    enum class view_mode : std::uint32_t;

    class render_target;

    class renderer final
    {
    private:
        static bool reinit_window_params();

        static void init_render_targets();

        static void open_render_targets();

        static void close_render_targets();

        static void set_render_target(render_target& target, bool clear, ::D3DCOLOR clear_color);

        static void update_render_views();

    public:
        static bool reset();

        static void render();

    public:
        static inline view_mode& mode = *reinterpret_cast<view_mode*>(0x00AB0A38);

        static inline bool& cull_backfaces = *reinterpret_cast<bool*>(0x00A650E4);

        static inline ::D3DCULL& current_cull_mode = *reinterpret_cast<::D3DCULL*>(0x00A6523C);

        static inline bool& envmap_calibration = *reinterpret_cast<bool*>(0x00AB0AA8);
    };
}
