#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct aabb_node
    {
        vector3 position;
        std::uint16_t parent_index;
        std::uint16_t child_count;
        vector3 extent;
        std::int16_t children[10];
    };

    struct aabb_tree
    {
        aabb_node* nodes;
        std::uint16_t leaf_node_count;
        std::uint16_t parent_node_count;
        std::uint16_t total_node_count;
        std::uint16_t max_depth;
        std::uint32_t padding;
    };

    ASSERT_SIZE(aabb_node, 0x30);
    ASSERT_SIZE(aabb_tree, 0x10);
}
