#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    enum class instance_flags : std::uint32_t;

    class model_renderer final
    {
    public:
        static void render(const view::instance& view, instance_flags flags);
    };
}
