#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct aabb_node
    {
    public:
        bool contains(const vector3& point) const;

    public:
        vector3 center;
        std::int16_t parent_index;
        std::int16_t child_count;
        vector3 extent;
        std::int16_t children[10];
    };

    struct aabb_tree
    {
    private:
        auto query_leaf_internal(const aabb_node& node, const vector3& position) -> const aabb_node*;

    public:
        auto query_leaf(const vector3& position) -> const aabb_node*;

    public:
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
