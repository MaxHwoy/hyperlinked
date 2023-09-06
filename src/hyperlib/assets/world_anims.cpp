#include <hyperlib/assets/world_anims.hpp>
#include <hyperlib/memory/memory.hpp>
#include <hyperlib/streamer/sections.hpp>

namespace hyper
{
    world_anim::tree world_anim::temp_tree_{};

    world_anim::header world_anim::temp_header_{};

    world_anim::library* world_anim::library_{};

    void world_anim::instance::add_anim_node(world_anim::tree& tree_to_add)
    {
        call_function<void(__thiscall*)(world_anim::instance*, world_anim::tree&)>(0x00451040)(this, tree_to_add);
    }

    void world_anim::instance::init()
    {
        call_function<void(__thiscall*)(world_anim::instance*)>(0x00459BF0)(this);
    }

    auto world_anim::tree_pool::alloc_tree(std::uint32_t node_count) -> world_anim::instance*
    {
        return call_function<world_anim::instance*(__thiscall*)(world_anim::tree_pool*, std::uint32_t)>(0x0045C740)(this, node_count);
    }

    void world_anim::tree_pool::dealloc_tree(std::uint16_t section_number)
    {
        call_function<void(__thiscall*)(world_anim::tree_pool*, std::uint16_t)>(0x0045DC80)(this, section_number);
    }

    auto world_anim::find_library_tree(const world_anim::rt_node& node) -> world_anim::tree*
    {
        world_anim::library* library = world_anim::library_;

        if (library != nullptr)
        {
            std::uint32_t count = library->tree_count;

            for (std::uint32_t i = 0u; i < count; ++i)
            {
                world_anim::tree* lib_tree = library->trees + i;

                if (lib_tree->node_count != 0u)
                {
                    std::uint32_t key = lib_tree->nodes[0]->key;

                    if (key == node.solid_keys[0] || key == node.solid_keys[1])
                    {
                        return lib_tree;
                    }
                }
            }
        }

        return nullptr;
    }

    bool world_anim::loader(chunk* block)
    {
        if (!world_anim::disable_animations)
        {
            if (block->id() == block_id::world_anim_header)
            {
                world_anim::header& header = world_anim::temp_header_ = *reinterpret_cast<world_anim::header*>(block->aligned_data(0x10u));

                if (header.section_number == game_provider::shared_solid_section() && world_anim::library_ == nullptr)
                {
                    alloc_size_t total_size = static_cast<alloc_size_t>((sizeof(world_anim::library) - sizeof(world_anim::tree)) + header.tree_count * sizeof(world_anim::tree));
#if defined(_DEBUG)
                    world_anim::library_ = reinterpret_cast<world_anim::library*>(memory::malloc_debug(total_size, 0u, __FILE__, __LINE__));
#else
                    world_anim::library_ = reinterpret_cast<world_anim::library*>(memory::malloc(total_size, 0u));
#endif
                    ::memset(world_anim::library_, 0, total_size);

                    world_anim::library_->tree_count = static_cast<std::uint16_t>(header.tree_count);
                }
            }

            if (block->id() == block_id::world_anim_counts)
            {
                std::uint32_t node_count = reinterpret_cast<tree_info*>(block->aligned_data(0x10u))->count;

                world_anim::tree* tree = nullptr;

                if (world_anim::temp_header_.section_number == game_provider::shared_solid_section())
                {
                    tree = &world_anim::library_->trees[world_anim::library_->tree_index++];
                }
                else
                {
                    tree = &world_anim::temp_tree_;
                }

                tree->node_count = node_count;

                for (std::uint32_t i = 0u; i < node_count; ++i)
                {
                    block = block->end();

                    if (block->id() == block_id::world_anim_rtnode)
                    {
                        world_anim::rt_node* node = reinterpret_cast<world_anim::rt_node*>(block->aligned_data(0x10u));

                        tree->nodes[i] = node;

                        if ((node->flags & (rt_flags::use_library_anim | rt_flags::use_parent_anim)) != 0 || node->key_frame_count == 0u)
                        {
                            tree->frames[i] = nullptr;
                        }
                        else
                        {
                            block = block->end();

                            if (block->id() == block_id::world_anim_frames)
                            {
                                tree->frames[i] = reinterpret_cast<matrix4x4*>(block->aligned_data(0x10u));
                            }
                        }
                    }
                }

                if (world_anim::temp_header_.section_number != game_provider::shared_solid_section())
                {
                    for (std::uint32_t i = 0u; i < node_count; ++i)
                    {
                        world_anim::rt_node& node = *tree->nodes[i];

                        if ((node.flags & rt_flags::use_library_anim) != 0)
                        {
                            if (world_anim::tree* lib_tree = world_anim::find_library_tree(node))
                            {
                                world_anim::rt_node* parent_node = lib_tree->nodes[0];

                                node.rotation_speed[0] = parent_node->rotation_speed[0];
                                node.rotation_speed[1] = parent_node->rotation_speed[1];
                                node.rotation_speed[2] = parent_node->rotation_speed[2];
                                node.initial_angle = parent_node->initial_angle;

                                if (lib_tree->node_count > 1u)
                                {
                                    for (std::uint32_t j = 1u; j < lib_tree->node_count; ++j)
                                    {
                                        tree->nodes[tree->node_count + j] = lib_tree->nodes[j];
                                        tree->frames[tree->node_count + j] = lib_tree->frames[j];
                                    }

                                    tree->node_count += lib_tree->node_count - 1u;
                                }
                            }
                        }
                        else if ((node.flags & rt_flags::use_parent_anim) != 0 && node.use_parent_frames)
                        {
                            ASSERT(node.parent_index >= 0);

                            world_anim::rt_node* parent_node = tree->nodes[node.parent_index];
                            
                            node.rotation_speed[0] = parent_node->rotation_speed[0];
                            node.rotation_speed[1] = parent_node->rotation_speed[1];
                            node.rotation_speed[2] = parent_node->rotation_speed[2];
                            node.initial_angle = parent_node->initial_angle;
                            
                            tree->frames[i] = tree->frames[node.parent_index];
                        }
                    }

                    world_anim::instance* instance = world_anim::tree_pool::instance.alloc_tree(tree->node_count);

                    instance->add_anim_node(*tree);

                    instance->init();
                }
            }
        }

        return true;
    }

    bool world_anim::unloader(chunk* block)
    {
        if (!world_anim::disable_animations)
        {
            if (block->id() == block_id::world_anim_header)
            {
                world_anim::header* header = reinterpret_cast<world_anim::header*>(block->aligned_data(0x10u));

                if (header->section_number == game_provider::shared_solid_section())
                {
                    if (world_anim::library_ != nullptr)
                    {
                        memory::free(world_anim::library_);

                        world_anim::library_ = nullptr;
                    }
                }
                else
                {
                    world_anim::tree_pool::instance.dealloc_tree(header->section_number);
                }
            }
        }

        return true;
    }
}
