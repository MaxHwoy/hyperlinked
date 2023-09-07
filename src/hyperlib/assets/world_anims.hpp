#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class world_anim final
    {
    public:
        enum class rt_flags : std::uint8_t
        {
            is_library_anim  = 1u << 0,
            use_library_anim = 1u << 1,
            use_parent_anim  = 1u << 2,
        };

        struct tree_info
        {
            std::uint32_t count;
        };

        struct end
        {
            std::uint32_t cool;
        };

        struct rt_node
        {
            matrix4x4 transform;
            std::uint32_t key;
            std::int8_t parent_index;
            std::uint8_t timescale;
            rt_flags flags;
            std::uint8_t pad;
            std::uint32_t solid_keys[0x03];
            std::uint32_t smackable_key;
            std::uint32_t scenery_guid;
            std::int16_t rotation_speed[3];
            std::uint16_t section_number;
            std::uint32_t key_frame_count;
            union {
                std::uint32_t frame_count  : 30;
                std::uint32_t is_ping_pong : 1;
                std::uint32_t is_looping   : 1;
            } bits;
            std::int16_t initial_angle;
            std::uint16_t unknown0x6E;
            bool use_parent_frames;
        };

        struct node
        {
            rt_node* anim_node;
            matrix4x4* key_frames;
        };

        struct tree
        {
            std::uint32_t node_count;
            rt_node* nodes[0x08];
            matrix4x4* frames[0x08];
        };

        struct header
        {
            std::uint32_t anim_bank_count;
            std::uint32_t instance_node_count;
            std::uint16_t section_number;
            std::uint32_t tree_count;
        };

        struct library
        {
            std::uint16_t tree_count;
            std::uint16_t tree_index;
            tree trees[0x01];
        };

        struct instance
        {
        public:
            void add_anim_node(tree& tree_to_add);

            void init();
        };

        struct tree_pool
        {
        public:
            auto alloc_tree(std::uint32_t node_count) -> instance*;

            void dealloc_tree(std::uint16_t section_number);

        public:
            instance* instances[0x100];
            bool alloc_map[0x100];
            std::uint32_t instance_count;

        public:
            static inline tree_pool& instance = *reinterpret_cast<tree_pool*>(0x00A843F8);
        };

    public:
        static auto find_library_tree(const world_anim::rt_node& node) -> world_anim::tree*;

        static bool loader(chunk* block);

        static bool unloader(chunk* block);

    private:
        static tree temp_tree_;

        static header temp_header_;

        static library* library_;

        static inline bool& disable_animations = *reinterpret_cast<bool*>(0x00A9E6A4);
    };

    CREATE_ENUM_FLAG_OPERATORS(world_anim::rt_flags);

    ASSERT_SIZE(world_anim::tree_info, 0x04);
    ASSERT_SIZE(world_anim::end, 0x04);
    ASSERT_SIZE(world_anim::rt_node, 0x74);
    ASSERT_SIZE(world_anim::node, 0x08);
    ASSERT_SIZE(world_anim::tree, 0x44);
    ASSERT_SIZE(world_anim::header, 0x10);
    ASSERT_SIZE(world_anim::tree_pool, 0x504);
}
