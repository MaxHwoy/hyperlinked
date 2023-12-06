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

        static void notify_sky_loader();

        static void enable_barrier_scenery_group(const char* name, bool flip_artwork);

        static void disable_all_scenery_groups();

        static void init_topology_and_scenery_groups();

        static void redo_topology_and_scenery_groups();

    private:
        static inline const char* permanent_scenery_groups[] =
        {
#if defined(NFSCO)
            "SCENERY_GROUP_CHRISTMAS_DISABLE",
#else
            "SCENERY_GROUP_CHRISTMAS",
#endif
            "SCENERY_GROUP_DOOR",
            "SCENERY_GROUP_HALLOWEEN_DISABLE",
        };
    };
}
