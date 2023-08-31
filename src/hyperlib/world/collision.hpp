#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/udata.hpp>
#include <hyperlib/collections/eastl.hpp>

namespace hyper
{
    struct collision
    {
        enum class vertex_flags : std::uint8_t
        {
            overlapping = 1u << 2,
            two_sided   = 1u << 4,
            facing_down = 1u << 7,
        };

        enum class instance_flags : std::uint8_t
        {
            y_vector_not_up = 1u << 0,
            dynamic         = 1u << 1,
            disabled        = 1u << 2,
            no_traffic      = 1u << 6,
            no_cop          = 1u << 7,
            exclusion       = no_traffic | no_cop,
        };

        enum class object_flags : std::uint16_t
        {
            dynamic           = 1u << 0,
            vehicle           = 1u << 1,
            character         = 1u << 2,
            player_controlled = 1u << 3,
            hench_controlled  = 1u << 4,
            disabled          = 1u << 5,
            unrenderable      = 1u << 6,
        };

        enum class trigger_flags : std::uint32_t
        {
            enabled               = 1u << 0,
            one_shot              = 1u << 1,
            player_activated      = 1u << 2,
            ai_activated          = 1u << 3,
            explosion_activated   = 1u << 4,
            vehicle_activated     = 1u << 5,
            body_activated        = 1u << 6,
            time_activated        = 1u << 7,
            unmanaged             = 1u << 8,
            path_anim_activated   = 1u << 9,
            silencable            = 1u << 10,
            use_entry             = 1u << 11,
            non_up_y_vec          = 1u << 12,
            player_char_activated = 1u << 13,
            debug_only            = 1u << 14,
            fire_on_exit          = 1u << 15,
            human_activated       = 1u << 16,
            cop_activated         = 1u << 17,
            fire_on_entry         = 1u << 18,
        };

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

        struct article
        {
            std::uint16_t strip_count;
            std::uint16_t strip_ize;
            std::uint16_t edge_count;
            std::uint16_t edge_size;
            bool resolved;
            std::uint8_t surface_count;
            std::uint16_t surface_size;
            std::uint16_t intermediate_index;
            std::uint16_t flags;
        };

        struct strip_sphere
        {
        public:
            inline auto true_radius() -> float
            {
                return this->radius * 0.0625f;
            }

        public:
            vector3 position;
            std::uint16_t radius;
            std::uint16_t offset;
        };

        struct vertex
        {
        public:
            inline auto position() -> vector3
            {
                return { this->x * 0.0078125f, this->y * 0.0078125f, this->z * 0.0078125f };
            }

        public:
            std::int16_t x;
            std::int16_t y;
            std::int16_t z;
            std::uint8_t surface;
            vertex_flags flags;
        };

        struct surface_ref
        {
            union
            {
                std::uint32_t key;
                void* ptr;
            } ref;
        };

        struct edge
        {
            vector3 min;
            std::uint8_t surface;
            instance_flags flags;
            std::uint16_t unknown;
            vector3 max;
            float normalizer;
        };

        struct instance
        {
            vector3 inverted_row1;
            float extent_x;
            std::uint16_t iteration_stamp;
            instance_flags flags;
            float extent_y;
            std::uint16_t group_number; // scenery group number
            std::uint16_t article_index;
            article* attached_article;
            vector3 inverted_row2;
            float extent_z;
            vector3 inverted_position;
            float radius; // x z radius
        };

        struct managed_instance
        {
            instance* the_instance;
            std::uint32_t trigger_index;
        };

        struct managed_instance_map : public eastl::map<std::uint32_t, managed_instance*>
        {
        };

        struct object
        {
            vector3 position;
            float radius;
            vector4 dimensions;
            char type;
            char shape;
            object_flags flags;
            std::uint16_t instance_index;
            std::uint32_t grid_element;
            void* the_surface;
            matrix4x4 transform;
        };

        struct object_map : public eastl::map<std::uint32_t, object*>
        {
        };

        struct trigger
        {
            vector3 matrix_row1;
            float extent_x;
            std::uint32_t flags : 24;
            std::uint32_t shape : 4;
            std::uint32_t type  : 4;
            float extent_y;
            void* event; // EventSys::StaticData*
            std::uint16_t iteration_stamp;
            std::uint16_t fingerprint;
            vector3 matrix_row3;
            float extent_z;
            vector3 position;
            float radius;
        };

        struct trigger_set : public eastl::set<trigger*>
        {
        };

        struct pack
        {
        public:
            std::uint16_t section_number;
            std::uint32_t instance_count;
            instance* instances;
            std::uint32_t object_count;
            object* objects;
            uheader* header;

        public:
            static inline pack**& packs = *reinterpret_cast<pack***>(0x00B77EB0);

            static inline std::uint16_t pack_count = 0u;
        };

        struct assets
        {
        public:
            static void ctor(assets& ref);
            static void dtor(assets& ref);

        public:
            assets();
            ~assets();

        public:
            span<instance> static_instances;
            managed_instance_map* managed_instances;
            std::uint32_t managed_instances_index;
            span<object> static_objects;
            object_map* managed_objects;
            std::uint32_t managed_objects_index;
            std::uint32_t pack_load_callback_count;
            void(*pack_load_callbacks[4])(std::uint16_t section_number, bool loaded);
            span<trigger> static_triggers;
            trigger_set waiting_add_trigger_list;

        public:
            static inline assets*& instance = *reinterpret_cast<assets**>(0x00B77EA0);
        };
    };

    CREATE_ENUM_FLAG_OPERATORS(collision::vertex_flags);
    CREATE_ENUM_FLAG_OPERATORS(collision::instance_flags);
    CREATE_ENUM_FLAG_OPERATORS(collision::object_flags);
    CREATE_ENUM_FLAG_OPERATORS(collision::trigger_flags);

    ASSERT_SIZE(collision::surface, 0x02);
    ASSERT_SIZE(collision::manager, 0x08);
    ASSERT_SIZE(collision::triangle, 0x30);
    ASSERT_SIZE(collision::world_pos, 0x3C);
    ASSERT_SIZE(collision::collider, 0x98);
    ASSERT_SIZE(collision::article, 0x10);
    ASSERT_SIZE(collision::strip_sphere, 0x10);
    ASSERT_SIZE(collision::vertex, 0x08);
    ASSERT_SIZE(collision::edge, 0x20);
    ASSERT_SIZE(collision::instance, 0x40);
    ASSERT_SIZE(collision::managed_instance, 0x08);
    ASSERT_SIZE(collision::managed_instance_map, 0x1C);
    ASSERT_SIZE(collision::object, 0x70);
    ASSERT_SIZE(collision::object_map, 0x1C);
    ASSERT_SIZE(collision::trigger, 0x40);
    ASSERT_SIZE(collision::trigger_set, 0x1C);
    ASSERT_SIZE(collision::pack, 0x18);
    ASSERT_SIZE(collision::assets, 0x58);
}
