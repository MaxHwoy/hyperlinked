#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class grand_scenery_cull_info;

    class main_view_renderer final
    {
    public:
        static void render(const view::instance& main_view, const grand_scenery_cull_info& culler);
    };
}
