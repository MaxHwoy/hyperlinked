#include <hyperlib/world/world.hpp>

namespace hyper
{
    void world::init()
    {

    }

    void world::init_visible_zones(geometry::model*& boundary_model)
    {
        if (boundary_model == nullptr && geometry::model::pool != nullptr)
        {
            boundary_model = geometry::model::pool->allocate();

            if (boundary_model != nullptr)
            {
                boundary_model->init(hashing::bin("MARKER_BOUNDARY"));
            }
        }
    }
}
