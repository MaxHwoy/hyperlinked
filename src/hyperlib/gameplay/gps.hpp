#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class gps final
    {
    public:
        static void render(const view::instance& view);
    };
}
