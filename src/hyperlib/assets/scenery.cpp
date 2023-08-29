#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/assets/props.hpp>
#include <hyperlib/streamer/sections.hpp>

namespace hyper
{
    const scenery::group* scenery::group::scenery_group_door_(nullptr);

    void scenery::override_info::set_exclude_flags(std::uint16_t mask, std::uint16_t flag)
    {
        this->instance_flags = flag | (this->instance_flags & mask);

        visible_section::user_info* info = visible_section::manager::instance.user_infos[this->section_number];

        if (info != nullptr && info->scenery != nullptr)
        {
            this->assign_overrides(*info->scenery);
        }
    }

    void scenery::override_info::assign_overrides(scenery::pack& pack) const
    {
        constexpr hyper::instance_flags mask = ~static_cast<hyper::instance_flags>(std::numeric_limits<std::uint16_t>::max());

        scenery::instance& instance = pack.instances[this->instance_number];

        hyper::instance_flags old_flags = instance.flags;
        hyper::instance_flags new_flags = static_cast<hyper::instance_flags>(this->instance_flags);

        if (override_info::can_flip(old_flags) && override_info::should_flip(old_flags, new_flags))
        {
            math::flip_sign(instance.rotation.m11);
            math::flip_sign(instance.rotation.m12);
            math::flip_sign(instance.rotation.m13);
        }

        instance.flags = (old_flags & mask) | new_flags;
    }

    void scenery::group::enable(std::uint32_t key, bool flip_artwork)
    {
        const scenery::group* group = scenery::group::find(key);

        if (group != nullptr)
        {
            group->enable_rendering(flip_artwork);

            std::uint8_t mask = 1u;

            if (flip_artwork)
            {
                mask |= 2u;
            }

            if (group->drive_through_barrier_flag)
            {
                mask |= 4u;
            }

            scenery::group::enabled_table[group->group_number] = mask;
        }
    }

    void scenery::group::disable(std::uint32_t key)
    {
        const scenery::group* group = scenery::group::find(key);

        if (group != nullptr)
        {
            group->disable_rendering();

            scenery::group::enabled_table[group->group_number] = 0u;
        }
    }

    auto scenery::group::find(std::uint32_t key) -> const scenery::group*
    {
        if (key == hashing::bin_const("SCENERY_GROUP_DOOR"))
        {
            return scenery::group::scenery_group_door_;
        }

        for (const scenery::group* i = scenery::group::list.begin(); i != scenery::group::list.end(); i = i->next())
        {
            if (i->key == key)
            {
                return i;
            }
        }

        return nullptr;
    }

    bool scenery::group::loader(chunk* block)
    {
        if (block->id() == block_id::scenery_groups)
        {
            uintptr_t current = reinterpret_cast<uintptr_t>(block->data());
            uintptr_t endaddr = reinterpret_cast<uintptr_t>(block->end());

            while (current < endaddr)
            {
                scenery::group* group = reinterpret_cast<scenery::group*>(current);

                scenery::group::list.add(group);

                current += sizeof(scenery::group) - sizeof(group->overrides);

                current += group->override_count * sizeof(group->overrides[0]);

                current += sizeof(void*) - (current & 0x03u);

                if (group->key == hashing::bin_const("SCENERY_GROUP_DOOR"))
                {
                    scenery::group::scenery_group_door_ = group;
                }
            }

            return true;
        }

        return false;
    }

    bool scenery::group::unloader(chunk* block)
    {
        if (block->id() == block_id::scenery_groups)
        {
            scenery::group::list.clear();

            scenery::group::enabled_table.clear();

            scenery::group::enabled_table[0] = 1u;

            scenery::group::scenery_group_door_ = nullptr;

            return true;
        }

        return false;
    }

    void scenery::group::enable_rendering(bool flip_artwork) const
    {
        constexpr std::uint16_t mask = ~static_cast<std::uint16_t>(
            instance_flags::exclude_disable_rendering |
            instance_flags::exclude_group_disable |
            instance_flags::artwork_flipped
        );

        std::uint16_t flag = flip_artwork
            ? static_cast<std::uint16_t>(instance_flags::artwork_flipped)
            : static_cast<std::uint16_t>(0u);

        for (std::uint16_t i = 0u; i < this->override_count; ++i)
        {
            override_info::table[this->overrides[i]].set_exclude_flags(mask, flag);
        }

        props::sync_overrides();
    }

    void scenery::group::disable_rendering() const
    {
        constexpr std::uint16_t mask = std::numeric_limits<std::uint16_t>::max();

        constexpr std::uint16_t flag = static_cast<std::uint16_t>(
            instance_flags::exclude_disable_rendering |
            instance_flags::exclude_group_disable
        );

        for (std::uint16_t i = 0u; i < this->override_count; ++i)
        {
            override_info::table[this->overrides[i]].set_exclude_flags(mask, flag);
        }

        props::sync_overrides();
    }
}
