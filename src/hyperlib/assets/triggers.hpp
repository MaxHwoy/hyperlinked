#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/aabb.hpp>

namespace hyper
{
    class trigger final
    {
    public:
        enum class event_id : std::uint32_t
        {
            car_on_fern = 0x10003,
            view_driving_line = 0x10005,
            activate_train = 0x10006,
            sound = 0x10007,
            guide_arrow = 0x10008,
            activate_plane = 0x10009,
            initiate_pursuit = 0x20000,
            call_for_backup = 0x20001,
            call_for_roadblock = 0x20002,
            strategy_initiate = 0x20003,
            collision = 0x20004,
            announce_arrest = 0x20005,
            strategy_outcome = 0x20006,
            roadblock_update = 0x20007,
            cancel_pursuit = 0x20008,
            start_siren = 0x40000,
            stop_siren = 0x40001,
        };

        enum class parameter_type : std::uint32_t
        {
            TREE = 0x001D5D4F,
            TUNNEL_ENTRY = 0x2C15BD86,
            INTERSECTION = 0x495F75B6,
            PILLAR = 0x72F66B23,
            FOUNTAIN = 0xB2B5A6E3,
            TRAFFIC_LIGHT = 0xB6BD9C95,
            FREEWAY_SIGN = 0xF3ABE1C2,
            LAMPPOST = 0xF40A48EF,
        };

        struct instance
        {
            std::uint32_t key;
            event_id id;
            parameter_type Parameter;
            std::int32_t track_direction_mask;
            vector3 position;
            float radius;
        };

        struct pack : linked_node<pack>
        {
            std::uint32_t version;
            std::uint16_t section_number;
            std::uint32_t trigger_count;
            bool endian_swapped;
            __declspec(align(0x04)) aabb_tree* tree;
            instance* triggers;
        };
    };

    ASSERT_SIZE(trigger::instance, 0x20);
    ASSERT_SIZE(trigger::pack, 0x20);
}
