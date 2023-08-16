#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/geometry.hpp>

namespace hyper
{
    class world final
    {
    public:
        static void init();

        static void init_visible_zones(geometry::model*& boundary_model);
    };
}
