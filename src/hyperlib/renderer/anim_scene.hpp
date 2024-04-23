#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class anim_scene final
    {
    public:
        static void render(const view::instance& view, std::uint32_t flags);
    };
}
