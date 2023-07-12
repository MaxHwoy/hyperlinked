#pragma once

#include <hyperlib/assets/geometry.hpp>

namespace hyper
{
    enum class instance_flags : std::uint32_t
    {
        exclude_split_screen        = 1u << 0,  // 0x00000001
        exclude_main_view           = 1u << 1,  // 0x00000002
        exclude_racing              = 1u << 2,  // 0x00000004
        exclude_freeroam            = 1u << 3,  // 0x00000008
        exclude_disable_rendering   = 1u << 4,  // 0x00000010
        driveable                   = 1u << 5,  // 0x00000020
        include_rear_view           = 1u << 6,  // 0x00000040
        include_reflection          = 1u << 7,  // 0x00000080
        envmap_shadow               = 1u << 8,  // 0x00000100
        chopped_roadway             = 1u << 9,  // 0x00000200
        identity_matrix             = 1u << 10, // 0x00000400
        artwork_flipped             = 1u << 11, // 0x00000800
        reflection                  = 1u << 12, // 0x00001000
        environment_map             = 1u << 13, // 0x00002000
        swayable                    = 1u << 14, // 0x00004000
        enable_wind                 = 1u << 15, // 0x00008000
        always_facing               = 1u << 16, // 0x00010000
        dont_receive_shadows        = 1u << 17, // 0x00020000
        low_platform_only           = 1u << 18, // 0x00040000
        high_platform_only          = 1u << 19, // 0x00080000
        cast_shadow_volume          = 1u << 20, // 0x00100000
        cast_shadow_map             = 1u << 21, // 0x00200000
        inverted_matrix             = 1u << 22, // 0x00400000
        flip_on_backwards_track     = 1u << 23, // 0x00800000
        reflect_in_ocean            = 1u << 24, // 0x01000000
        visible_further             = 1u << 25, // 0x02000000
        cast_shadow_map_mesh        = 1u << 26, // 0x04000000
        include_reflection_ng       = 1u << 27, // 0x08000000
        aux_lighting                = 1u << 28, // 0x10000000
        pc_platform                 = 1u << 29, // 0x20000000
        collidable                  = 1u << 30, // 0x40000000
        lightmapped                 = 1u << 31, // 0x80000000
    };

    class scenery final
    {
    public:
        struct light_texture_collection
        {
            geometry::texture_entry light_map_entry[3];
            geometry::texture_entry directional_map_entry;
        };

        struct info
        {
            std::uint8_t debug_name[0x18];
            std::uint32_t solid_keys[4];
            geometry::model* models[4];
            float radius;
            std::uint32_t flags;
            std::uint32_t hierarchy_key;
            geometry::hierarchy* hierarchy;
        };

        struct instance
        {
            vector3 bbox_min;
            instance_flags flags;
            vector3 bbox_max;
            std::int16_t preculler_info_index;
            std::int16_t lighting_context_number;
            vector3 position;
            matrix3x3 rotation;
            std::uint32_t scenery_guid;
            std::int16_t scenery_info_number;
            std::int8_t lod_level;
            std::int8_t custom_flags;
            struct custom* custom;
            scenery::light_texture_collection* light_tex_collection;
        };

        struct tree_node
        {
            vector3 bbox_min;
            std::uint16_t child_count;
            std::uint16_t pad;
            vector3 bbox_max;
            std::int16_t children[10];
        };

        struct override_info_hookup
        {
            std::uint16_t override_info_number;
            std::uint16_t instance_number;
        };

        struct preculler_info
        {
            std::uint8_t visibility_bits[0x80];
        };

        struct __declspec(align(4)) scenery_group : linked_node<scenery_group>
        {
            std::uint32_t key;
            std::uint16_t group_number;
            std::uint16_t index_count;
            std::uint8_t barrier_flag;
            std::uint8_t drive_through_barrier_flag;
            std::uint16_t race_specific_section_number;
            std::uint16_t override_info_numbers[1];
        };

        struct override_info
        {
            std::uint16_t section_number;
            std::uint16_t instance_number;
            std::uint16_t instance_flags;
        };

        struct custom
        {
            char data[0x30];
            instance* instance;
        };

        struct section_box
        {
            vector4 centre;
            vector4 diag;
        };

        struct pack : linked_node<pack>
        {
            std::uint32_t chunks_loaded;
            std::uint16_t section_number;
            __declspec(align(0x04)) std::uint32_t polygon_in_memory_count;
            std::uint32_t polygon_in_world_count;
            scenery::info* infos;
            std::uint32_t info_count;
            scenery::instance* instances;
            std::uint32_t instance_count;
            section_box* ngbbs;
            tree_node* tree_nodes;
            std::uint32_t tree_node_count;
            preculler_info* preculler_infos;
            std::uint32_t preculler_info_count;
            std::uint32_t views_visible_this_frame;
            light_texture_collection* light_tex_list;
        };
    };

    CREATE_ENUM_FLAG_OPERATORS(instance_flags);

    ASSERT_SIZE(scenery::info, 0x48);
    ASSERT_SIZE(scenery::instance, 0x60);
    ASSERT_SIZE(scenery::tree_node, 0x30);
    ASSERT_SIZE(scenery::override_info_hookup, 0x04);
    ASSERT_SIZE(scenery::preculler_info, 0x80);
    ASSERT_SIZE(scenery::override_info, 0x06);
    ASSERT_SIZE(scenery::pack, 0x44);
}
