#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/streamer/sections.hpp>
#include <hyperlib/streamer/track_path.hpp>
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

    void world::enable_barrier_scenery_group(const char* name, bool flip_artwork)
    {
        if (visible_section::manager::get_group_info(name) != nullptr)
        {
            std::uint32_t key = hashing::bin(name);

            visible_section::manager::instance.enable_group(key);

            scenery::group::enable(key, flip_artwork);

            track_path::manager::instance.enable_barriers(name);
        }
    }

    void world::disable_all_scenery_groups()
    {
        for (const scenery::group* i = scenery::group::list.begin(); i != scenery::group::list.end(); i = i->next())
        {
            if (scenery::group::enabled_table[i->group_number])
            {
                i->disable_rendering();

                scenery::group::enabled_table[i->group_number] = 0;
            }
        }
    }

    void world::init_topology_and_scenery_groups()
    {
        for (const char* group : world::permanent_scenery_groups)
        {
            if (scenery::group::find(hashing::bin(group)) != nullptr)
            {
                world::enable_barrier_scenery_group(group, false);
            }
        }
    }

    void world::redo_topology_and_scenery_groups()
    {
        track_path::manager::instance.disable_all_barriers();

        visible_section::manager::instance.disable_all_groups();
        
        world::disable_all_scenery_groups();

        world::init_topology_and_scenery_groups();
    }
}
