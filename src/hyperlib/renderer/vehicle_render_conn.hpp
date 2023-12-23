#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class vehicle_render_conn
    {
    public:
        static void render_fe(const view::instance& view, bool reflection);

        static void render_world(const view::instance& view, bool reflection);

        static void render_flares(const view::instance& view, bool reflection, std::uint32_t reflection_flags);
    };
}
