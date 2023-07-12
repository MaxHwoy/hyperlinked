#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct collision
    {
        struct surface // inherit from carp surface
        {
            char data[0x02];
        };

        struct manager
        {
            std::uint32_t surface_exclusion_mask;
            std::uint32_t primitive_mask;

            inline manager(std::uint32_t surface_mask, std::uint32_t prim_mask) : surface_exclusion_mask(surface_mask), primitive_mask(prim_mask)
            {
            }

            inline manager() = default;

            inline manager(const manager& other) = default;

            inline manager(manager&& other) noexcept = default;

            inline manager& operator=(const manager& other) = default;

            inline manager& operator=(manager&& other) noexcept = default;

            bool get_world_height_at_point_rigorous(const vector3& point, float& height, vector3* normal);
        };

        struct triangle
        {
            vector3 p0;
            void* surface_ref; // Attrib::Collection*
            vector3 p1;
            std::uint32_t flags;
            vector3 p2;
            surface surface;
        };

        struct world_pos
        {
            triangle face;
            std::uint32_t flags;
            float height_offset;
            void* surface; // Attrib::Collection*
        };

        struct instance_cache_list
        {
            char data[0x10];
        };

        struct barrier_list
        {
            char data[0x10];
        };

        struct triangle_list
        {
            char data[0x14];
        };

        struct object_list
        {
            char data[0x10];
        };

        struct collider
        {
            enum class shape : std::uint32_t
            {
                sphere,
                cylinder,
            };

            vector3 requested_position;
            float requestedradius;
            vector3 last_requested_position;
            float last_requested_radius;
            vector3 position;
            float radius;
            vector3 last_refreshed_position;
            instance_cache_list instance_cache_list;
            barrier_list barrier_list;
            triangle_list tri_list;
            object_list obb_list;
            bool region_initialized;
            shape collider_shape;
            std::uint32_t type_mask;
            std::uint32_t ref_count;
            std::uint32_t world_id;
            std::uint32_t exclusion_flags;
        };
    };

    ASSERT_SIZE(collision::surface, 0x02);
    ASSERT_SIZE(collision::manager, 0x08);
    ASSERT_SIZE(collision::triangle, 0x30);
    ASSERT_SIZE(collision::world_pos, 0x3C);
    ASSERT_SIZE(collision::collider, 0x98);
}
