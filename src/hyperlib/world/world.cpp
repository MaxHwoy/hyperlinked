#include <hyperlib/world/world.hpp>

namespace hyper
{
    void world::init()
    {
        call_function<void(__cdecl*)()>(0x007BBE70)();
    }

    void world::init_visible_zones(geometry::model*& boundary_model)
    {
        if (boundary_model == nullptr && geometry::model::pool != nullptr)
        {
            boundary_model = geometry::model::pool->construct(hashing::bin_const("MARKER_BOUNDARY"));
        }
    }

    void world::notify_sky_loader()
    {
        call_function<void(__cdecl*)()>(0x007AF8F0)();
    }
}
