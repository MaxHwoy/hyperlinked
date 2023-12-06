#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class grand_scenery_cull_info;

    class rvm_renderer final
    {
    public:
        static void render(const view::instance& rvm_view, const grand_scenery_cull_info& culler);
    };
}
