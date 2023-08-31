#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/fast_mem.hpp>
#include <hyperlib/streamer/sections.hpp>
#include <hyperlib/world/collision.hpp>

namespace hyper
{
    bool collision::manager::get_world_height_at_point_rigorous(const vector3& point, float& height, vector3* normal)
    {
        return call_function<bool(__thiscall*)(collision::manager*, const vector3*, float*, vector3*)>(0x00816DF0)(this, &point, &height, normal);
    }

    void collision::assets::ctor(assets& ref)
    {
        new (&ref) assets();
    }

    void collision::assets::dtor(assets& ref)
    {
        ref.~assets();
    }

    collision::assets::assets() :
        waiting_add_trigger_list(),
        static_instances(),
        static_objects(),
        pack_load_callback_count(0u),
        pack_load_callbacks{},
        static_triggers()
    {
#if defined(_DEBUG)
        collision::managed_instance_map* instance_map = reinterpret_cast<collision::managed_instance_map*>(memory::malloc_debug(sizeof(collision::managed_instance_map), 0x00u, __FILE__, __LINE__));
#else
        collision::managed_instance_map* instance_map = reinterpret_cast<collision::managed_instance_map*>(memory::malloc(sizeof(collision::managed_instance_map), 0x00u));
#endif
        if (instance_map != nullptr)
        {
            new (instance_map) collision::managed_instance_map();
        }

        this->managed_instances = instance_map;
        this->managed_instances_index = 0x8000;

#if defined(_DEBUG)
        collision::object_map* object_map = reinterpret_cast<collision::object_map*>(memory::malloc_debug(sizeof(collision::object_map), 0x00u, __FILE__, __LINE__));
#else
        collision::object_map* object_map = reinterpret_cast<collision::object_map*>(memory::malloc(sizeof(collision::object_map), 0x00u));
#endif
        if (object_map != nullptr)
        {
            new (object_map) collision::object_map();
        }

        this->managed_objects = object_map;
        this->managed_objects_index = 0x8000;
#if defined(_DEBUG)
        pack::packs = reinterpret_cast<pack**>(memory::malloc_debug(sizeof(pack*) * game_provider::maximum_sections(), 0x00u, __FILE__, __LINE__));
#else
        pack::packs = reinterpret_cast<pack**>(memory::malloc(sizeof(pack*) * game_provider::maximum_sections(), 0x00u));
#endif
        if (pack::packs != nullptr)
        {
            pack::pack_count = game_provider::maximum_sections();

            ::memset(pack::packs, 0, sizeof(pack*) * pack::pack_count);
        }
        else
        {
            pack::pack_count = 0u;
        }

        for (const uintptr_t address :
        {
            0x00812879u, // WCollisionMgr::GetInstanceListGuts
            0x00812B89u, // WCollisionMgr::GetInstanceListGuts
            0x00812E69u, // WCollisionMgr::GetObjectListGuts
        })
        {
            hook::set<std::uint16_t>(address, pack::pack_count);
        }
    }

    collision::assets::~assets()
    {
        pack** packs = pack::packs;

        if (packs != nullptr)
        {
            std::uint32_t pack_count = pack::pack_count;

            for (std::uint32_t i = 0u; i < pack_count; ++i)
            {
                pack* current = packs[i];

                if (current != nullptr)
                {
                    for (std::uint32_t k = 0u; k < current->instance_count; ++k)
                    {
                        current->instances[k].attached_article = nullptr;
                    }

                    current->section_number = 0u;
                    current->instances = nullptr;
                    current->instance_count = 0u;
                    current->objects = nullptr;
                    current->object_count = 0u;

                    fast_mem::instance.free(current, sizeof(pack), nullptr);

                    packs[i] = nullptr;
                }
            }

            memory::free(packs);
        }

        pack::packs = nullptr;
        pack::pack_count = 0u;

        for (const uintptr_t address :
        {
            0x00812879u, // WCollisionMgr::GetInstanceListGuts
            0x00812B89u, // WCollisionMgr::GetInstanceListGuts
            0x00812E69u, // WCollisionMgr::GetObjectListGuts
        })
        {
            hook::set<std::uint16_t>(address, 0u);
        }

        if (this->managed_instances != nullptr)
        {
            this->managed_instances->~managed_instance_map();

            memory::free(this->managed_instances);

            this->managed_instances = nullptr;
        }

        if (this->managed_objects != nullptr)
        {
            for (auto i = this->managed_objects->begin(); i != this->managed_objects->end(); ++i)
            {
                call_function<void(__cdecl*)(void*)>(0x006A1590)(i->second); // ::free
            }

            this->managed_objects->~object_map();

            memory::free(this->managed_objects);

            this->managed_objects = nullptr;
        }
    }
}
