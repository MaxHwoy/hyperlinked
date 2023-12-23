#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class skid_renderer final
    {
    public:
        static void render(const view::instance& view);
    };
}
