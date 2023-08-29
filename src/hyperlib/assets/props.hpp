#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    enum class instance_flags : std::uint32_t;

    class props final
    {
    public:
        struct scenery;

        struct anim_handle;

        struct spawner
        {
            vector4 rotation;
            vector3 position;
            std::uint32_t model;
            std::uint32_t collision;
            std::uint32_t smackable;
            std::uint32_t override_number;
            std::uint32_t unique_id;
            instance_flags exclude_flags;
            instance_flags spawn_flags;
            scenery* sim_model;
            anim_handle* handle;
        };

        struct spawner_pack : public linked_node<spawner_pack>
        {
        public:
            std::uint16_t section_number;
            std::uint16_t spawner_count;
            std::uint16_t endian_swapped;
            spawner spawners[0x01];

        public:
            static inline linked_list<spawner_pack>& list = *reinterpret_cast<linked_list<spawner_pack>*>(0x00B785BC);
        };

    public:
        static void sync_overrides();
    };

    ASSERT_SIZE(props::spawner, 0x40);
    ASSERT_SIZE(props::spawner_pack, 0x10 + sizeof(props::spawner));
}
