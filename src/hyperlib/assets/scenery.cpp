#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/assets/props.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/streamer/sections.hpp>

#pragma warning (disable : 6011)

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
            std::uint32_t override_size = game_provider::are_sections_extended() ? sizeof(std::uint32_t) : sizeof(std::uint16_t);

            uintptr_t current = reinterpret_cast<uintptr_t>(block->data());
            uintptr_t endaddr = reinterpret_cast<uintptr_t>(block->end());

            while (current < endaddr)
            {
                scenery::group* group = reinterpret_cast<scenery::group*>(current);

                scenery::group::list.add(group);

                current += sizeof(scenery::group) - sizeof(group->overrides);

                current += group->override_count * override_size;

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

        if (game_provider::are_sections_extended())
        {
            for (std::uint16_t i = 0u; i < this->override_count; ++i)
            {
                override_info::table[this->overrides.extended[i]].set_exclude_flags(mask, flag);
            }
        }
        else
        {
            for (std::uint16_t i = 0u; i < this->override_count; ++i)
            {
                override_info::table[this->overrides.vanilla[i]].set_exclude_flags(mask, flag);
            }
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

        if (game_provider::are_sections_extended())
        {
            for (std::uint16_t i = 0u; i < this->override_count; ++i)
            {
                override_info::table[this->overrides.extended[i]].set_exclude_flags(mask, flag);
            }
        }
        else
        {
            for (std::uint16_t i = 0u; i < this->override_count; ++i)
            {
                override_info::table[this->overrides.vanilla[i]].set_exclude_flags(mask, flag);
            }
        }

        props::sync_overrides();
    }

    bool scenery::loader(chunk* block)
    {
        switch (block->id())
        {
            case block_id::scenery_section:
                return scenery::loader_scenery_section(block);

            case block_id::scenery_override_infos:
                return scenery::loader_override_infos(block);

            case block_id::model_hierarchy_tree:
                return scenery::loader_model_hierarchy(block);

            default:
                return false;
        }
    }

    bool scenery::unloader(chunk* block)
    {
        switch (block->id())
        {
            case block_id::scenery_section:
                return scenery::unloader_scenery_section(block);
        
            case block_id::scenery_override_infos:
                return scenery::unloader_override_infos(block);
        
            case block_id::model_hierarchy_tree:
                return scenery::unloader_model_hierarchy(block);
        
            default:
                return false;
        }
    }

    bool scenery::loader_scenery_section(chunk* block)
    {
        scenery::pack* pack = nullptr;

        for (chunk* curr = reinterpret_cast<chunk*>(block->data()); curr != block->end(); curr = curr->end())
        {
            if (curr->id() == block_id::scenery_header)
            {
                pack = reinterpret_cast<scenery::pack*>(curr->aligned_data(0x10u));

                visible_section::manager::instance.allocate_user_info(pack->section_number).scenery = pack;

                if (pack->section_number == game_provider::shared_scenery_section())
                {
                    scenery::pack::list.add_before(pack, scenery::pack::list.begin());
                }
                else
                {
                    scenery::pack::list.add(pack);
                }

                continue;
            }

            if (curr->id() == block_id::scenery_infos)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Info array is being loaded without scenery section header!");

                pack->infos = span<scenery::info>(reinterpret_cast<scenery::info*>(curr->data()), curr->size() / sizeof(scenery::info));

                for (std::uint32_t i = 0u; i < pack->infos.length(); ++i)
                {
                    scenery::info& info = pack->infos[i];

                    if (info.hierarchy_key != 0u)
                    {
                        geometry::hierarchy** hierarchy = geometry::hierarchy::map::instance.at(info.hierarchy_key);

                        info.hierarchy = hierarchy == nullptr ? nullptr : *hierarchy;
                    }
                }

                continue;
            }

            if (curr->id() == block_id::scenery_instances)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Instance array is being loaded without scenery section header!");

                pack->instances = span<scenery::instance>(reinterpret_cast<scenery::instance*>(curr->aligned_data(0x10u)), curr->aligned_size(0x10u) / sizeof(scenery::instance));

                continue;
            }

            if (curr->id() == block_id::scenery_tree_nodes)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Tree Node array is being loaded without scenery section header!");

                pack->tree_nodes = span<scenery::tree_node>(reinterpret_cast<scenery::tree_node*>(curr->aligned_data(0x10u)), curr->aligned_size(0x10u) / sizeof(scenery::tree_node));

                continue;
            }

            if (curr->id() == block_id::scenery_override_hooks)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Override Hook array is being loaded without scenery section header!");

                scenery::override_info_hookup* table = reinterpret_cast<scenery::override_info_hookup*>(curr->data());

                std::uint32_t count = curr->size() / sizeof(scenery::override_info_hookup);

                if (game_provider::are_sections_extended())
                {
                    for (std::uint32_t i = 0u; i < count; ++i)
                    {
                        scenery::override_info::table[table[i].info.extended.override_number].assign_overrides(*pack);
                    }
                }
                else
                {
                    for (std::uint32_t i = 0u; i < count; ++i)
                    {
                        scenery::override_info::table[table[i].info.vanilla.override_number].assign_overrides(*pack);
                    }
                }

                continue;
            }

            if (curr->id() == block_id::scenery_preculler_infos)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Preculler Info array is being loaded without scenery section header!");

                pack->preculler_infos = span<scenery::preculler_info>(reinterpret_cast<scenery::preculler_info*>(curr->data()), curr->size() / sizeof(scenery::preculler_info));

                continue;
            }

            if (curr->id() == block_id::scenery_ngbbs)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery NGBB array is being loaded without scenery section header!");

                pack->ngbbs = reinterpret_cast<scenery::section_box*>(curr->aligned_data(0x10u));

                continue;
            }

            if (curr->id() == block_id::light_texture_collections)
            {
                ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery Light Texture array is being loaded without scenery section header!");

                pack->light_tex_list = reinterpret_cast<scenery::light_texture_collection*>(curr->data());

                std::uint32_t count = curr->size() / sizeof(scenery::light_texture_collection);

                ASSERT_WITH_MESSAGE(pack->instances.length() == count, "Light Texture array length should be the same as Scenery Instance array length!");

                for (std::uint32_t i = 0u; i < count; ++i)
                {
                    pack->instances[i].light_tex_collection = pack->light_tex_list + i;
                }

                continue;
            }

            PRINT_WARNING("Unknown chunk 0x%08X at scenery section %d", static_cast<std::uint32_t>(curr->id()), pack == nullptr ? 0u : pack->section_number);
        }

        ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery section does not have a header!");

        if (loader::chunk_movement_offset == 0u)
        {
            size_t info_count = pack->infos.length();

            for (size_t i = 0u; i < info_count; ++i)
            {
                scenery::info& info = pack->infos[i];

                for (size_t j = 0u; j < std::size(info.models); ++j)
                {
                    std::uint32_t key = info.solid_keys[j];

                    if (key != 0u && key != hashing::bin_const("SKYDOME") && key != hashing::bin_const("SKY_SPECULAR"))
                    {
                        geometry::model* model = nullptr;

                        for (size_t k = 0u; k < j; ++k)
                        {
                            geometry::model* compare = info.models[k];

                            if (compare != nullptr && key == compare->key)
                            {
                                model = compare;

                                break;
                            }
                        }

                        if (model == nullptr)
                        {
                            model = geometry::model::pool->construct(key);

                            if (model != nullptr)
                            {
                                model->lod_level = static_cast<std::int16_t>(j);

                                if (scenery::model_connection_callback != nullptr)
                                {
                                    scenery::model_connection_callback(*pack, info, *model);
                                }
                            }
                        }

                        info.models[j] = model;
                    }
                }

                if (info.models[static_cast<std::uint32_t>(model_lod::c)] == nullptr)
                {
                    info.models[static_cast<std::uint32_t>(model_lod::c)] = info.models[static_cast<std::uint32_t>(model_lod::b)];
                }

                if (info.models[static_cast<std::uint32_t>(model_lod::a)] == nullptr)
                {
                    info.models[static_cast<std::uint32_t>(model_lod::a)] = info.models[static_cast<std::uint32_t>(model_lod::b)];
                }
            }

            if (scenery::section_connection_callback != nullptr)
            {
                scenery::section_connection_callback(*pack);
            }
        }

        pack->chunks_loaded = true;

        return true;
    }

    bool scenery::loader_override_infos(chunk* block)
    {
        scenery::override_info* ptr = reinterpret_cast<scenery::override_info*>(block->data());

        size_t length = block->size() / sizeof(scenery::override_info);

        scenery::override_info::table = span<scenery::override_info>(ptr, length);

        return true;
    }

    bool scenery::loader_model_hierarchy(chunk* block)
    {
        for (chunk* curr = reinterpret_cast<chunk*>(block->data()); curr != block->end(); curr = curr->end())
        {
            if (curr->id() == block_id::model_hierarchy_instance)
            {
                geometry::hierarchy* hierarchy = reinterpret_cast<geometry::hierarchy*>(curr->data());

                hierarchy->flag |= geometry::hierarchy::flags::endian_swapped;

                geometry::hierarchy::map::instance[hierarchy->key] = hierarchy;

                for (std::uint32_t i = 0u; i < hierarchy->node_count; ++i)
                {
                    geometry::hierarchy::node& node = hierarchy->nodes[i];

                    geometry::model* model = nullptr;

                    if (node.solid_key != 0u)
                    {
                        model = geometry::model::pool->construct(node.solid_key);
                    }

                    node.model = model;
                }
            }
        }

        return true;
    }

    bool scenery::unloader_scenery_section(chunk* block)
    {
        scenery::pack* pack = nullptr;

        for (chunk* curr = reinterpret_cast<chunk*>(block->data()); curr != block->end(); curr = curr->end())
        {
            if (curr->id() == block_id::scenery_header)
            {
                pack = reinterpret_cast<scenery::pack*>(curr->aligned_data(0x10u));

                break;
            }
        }

        ASSERT_WITH_MESSAGE(pack != nullptr, "Scenery section does not have a header!");

        visible_section::manager::instance.unallocate_user_info(pack->section_number);

        scenery::pack::list.remove(pack);

        if (loader::chunk_movement_offset == 0u)
        {
            size_t info_count = pack->infos.length();

            for (size_t i = 0u; i < info_count; ++i)
            {
                scenery::info& info = pack->infos[i];

                for (size_t j = 0u; j < std::size(info.models); ++j)
                {
                    geometry::model* model = info.models[j];

                    if (model != nullptr)
                    {
                        for (size_t k = j + 1; k < std::size(info.models); ++k)
                        {
                            if (model == info.models[k])
                            {
                                info.models[k] = nullptr;
                            }
                        }

                        if (scenery::model_disconnection_callback != nullptr)
                        {
                            scenery::model_disconnection_callback(*pack, info, *model);
                        }

                        geometry::model::pool->destruct(model);

                        info.models[j] = nullptr;
                    }
                }
            }

            if (scenery::section_disconnection_callback != nullptr)
            {
                scenery::section_disconnection_callback(*pack);
            }
        }

        pack->chunks_loaded = false;

        return true;
    }

    bool scenery::unloader_override_infos(chunk* block)
    {
        scenery::override_info::table = span<scenery::override_info>();

        return true;
    }

    bool scenery::unloader_model_hierarchy(chunk* block)
    {
        for (chunk* curr = reinterpret_cast<chunk*>(block->data()); curr != block->end(); curr = curr->end())
        {
            if (curr->id() == block_id::model_hierarchy_instance)
            {
                geometry::hierarchy* hierarchy = reinterpret_cast<geometry::hierarchy*>(curr->data());

                for (std::uint32_t i = 0u; i < hierarchy->node_count; ++i)
                {
                    geometry::hierarchy::node& node = hierarchy->nodes[i];

                    if (node.model != nullptr)
                    {
                        geometry::model::pool->destruct(node.model);
                    }
                }

                geometry::hierarchy::map::instance.remove(hierarchy->key);
            }
        }

        return true;
    }
}
