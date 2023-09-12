#include <hyperlib/assets/aabb.hpp>

namespace hyper
{
    bool aabb_node::contains(const vector3& point) const
    {
        return (::fabs(point.x - this->center.x) < this->extent.x) &&
               (::fabs(point.y - this->center.y) < this->extent.y) &&
               (::fabs(point.z - this->center.z) < this->extent.z);
    }

    auto aabb_tree::query_leaf_internal(const aabb_node& node, const vector3& position) -> const aabb_node*
    {
        for (std::int16_t i = 0; i < node.child_count; ++i)
        {
            const aabb_node& child = this->nodes[node.children[i]];

            if (child.contains(position))
            {
                if (child.child_count < 0)
                {
                    return &child;
                }

                if (const aabb_node* result = this->query_leaf_internal(child, position))
                {
                    return result;
                }
            }
        }

        return nullptr;
    }

    auto aabb_tree::query_leaf(const vector3& position) -> const aabb_node*
    {
        BENCHMARK();

        if (this->nodes != nullptr)
        {
            const aabb_node* stack[0x100];

            stack[0] = this->nodes;

            std::uint32_t top = 1u;

            while (top > 0u)
            {
                const aabb_node* current = stack[--top];

                if (current->contains(position))
                {
                    if (current->child_count < 0)
                    {
                        return current;
                    }
                    else
                    {
                        for (std::int16_t i = current->child_count - 1; i >= 0; --i)
                        {
                            stack[top++] = this->nodes + current->children[i];
                        }
                    }
                }
            }
        }

        return nullptr;
    }
}
