#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class particle_renderer final
    {
    public:
        static void render(const view::instance& view);
    };
}
