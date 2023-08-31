#include <cassert>
#include <hyperlib/collections/eastl.hpp>

namespace hyper
{
    auto eastl::rbtree_increment(const rbtree_node_base* node) -> rbtree_node_base*
    {
        if (node->right)
        {
            node = node->right;

            while (node->left)
            {
                node = node->left;
            }
        }
        else
        {
            rbtree_node_base* temp = node->parent;

            while (node == temp->right)
            {
                node = temp;
                temp = temp->parent;
            }

            if (node->right != temp)
            {
                node = temp;
            }
        }

        return const_cast<rbtree_node_base*>(node);
    }

    auto eastl::rbtree_decrement(const rbtree_node_base* node) -> rbtree_node_base*
    {
        if ((node->parent->parent == node) && (node->color == rbtree_color::red))
        {
            return node->right;
        }
        
        if (node->left)
        {
            rbtree_node_base* temp = node->left;

            while (temp->right)
            {
                temp = temp->right;
            }

            return temp;
        }

        rbtree_node_base* temp = node->parent;

        while (node == temp->left)
        {
            node = temp;
            temp = temp->parent;
        }

        return const_cast<rbtree_node_base*>(temp);
    }

    auto eastl::rbtree_get_min_child(const rbtree_node_base* node) -> rbtree_node_base*
    {
        while (node->left)
        {
            node = node->left;
        }

        return const_cast<rbtree_node_base*>(node);
    }

    auto eastl::rbtree_get_max_child(const rbtree_node_base* node) -> rbtree_node_base*
    {
        while (node->right)
        {
            node = node->right;
        }

        return const_cast<rbtree_node_base*>(node);
    }

    auto eastl::rbtree_get_black_count(const rbtree_node_base* top, const rbtree_node_base* bottom) -> size_t
    {
        size_t count = 0u;

        for (/* empty */; bottom != nullptr; bottom = bottom->parent)
        {
            if (bottom->color == rbtree_color::black)
            {
                ++count;
            }

            if (bottom == top)
            {
                break;
            }
        }

        return count;
    }

    auto eastl::rbtree_rotate_left(rbtree_node_base* node, rbtree_node_base* root) -> rbtree_node_base*
    {
        rbtree_node_base* const temp = node->right;

        node->right = temp->left;

        if (temp->left)
        {
            temp->left->parent = node;
        }

        temp->parent = node->parent;

        if (node == root)
        {
            root = temp;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = temp;
        }
        else
        {
            node->parent->right = temp;
        }

        temp->left = node;
        node->parent = temp;

        return root;
    }

    auto eastl::rbtree_rotate_right(rbtree_node_base* node, rbtree_node_base* root) -> rbtree_node_base*
    {
        rbtree_node_base* const temp = node->left;

        node->left = temp->right;

        if (temp->right)
        {
            temp->right->parent = node;
        }

        temp->parent = node->parent;

        if (node == root)
        {
            root = temp;
        }
        else if (node == node->parent->right)
        {
            node->parent->right = temp;
        }
        else
        {
            node->parent->left = temp;
        }

        temp->right = node;
        node->parent = temp;

        return root;
    }

    void eastl::rbtree_insert(rbtree_node_base* node, rbtree_node_base* parent, rbtree_node_base* anchor, rbtree_side side)
    {
        rbtree_node_base*& root_ref = anchor->parent;

        // initialize fields in new node to insert.
        node->parent = parent;
        node->right = nullptr;
        node->left = nullptr;
        node->color = rbtree_color::red;

        // Insert the node.
        if (side == rbtree_side::left)
        {
            parent->left = node; // also makes (leftmost = node) when (parent == anchor)

            if (parent == anchor)
            {
                anchor->parent = node;
                anchor->right = node;
            }
            else if (parent == anchor->left)
            {
                anchor->left = node; // maintain leftmost pointing to min node
            }
        }
        else
        {
            parent->right = node;

            if (parent == anchor->right)
            {
                anchor->right = node; // maintain rightmost pointing to max node
            }
        }

        // rebalance the tree.
        while ((node != root_ref) && (node->parent->color == rbtree_color::red))
        {
            rbtree_node_base* const parent_squared = node->parent->parent;

            if (node->parent == parent_squared->left)
            {
                rbtree_node_base* const temp = parent_squared->right;

                if (temp != nullptr && (temp->color == rbtree_color::red))
                {
                    node->parent->color = rbtree_color::black;
                    temp->color = rbtree_color::black;
                    parent_squared->color = rbtree_color::red;
                    node = parent_squared;
                }
                else
                {
                    if (/* node->parent != nullptr && */ node == node->parent->right)
                    {
                        node = node->parent;
                        root_ref = eastl::rbtree_rotate_left(node, root_ref);
                    }

                    node->parent->color = rbtree_color::black;
                    parent_squared->color = rbtree_color::red;
                    root_ref = eastl::rbtree_rotate_right(parent_squared, root_ref);
                }
            }
            else
            {
                rbtree_node_base* const temp = parent_squared->left;

                if (temp && (temp->color == rbtree_color::red))
                {
                    node->parent->color = rbtree_color::black;
                    temp->color = rbtree_color::black;
                    parent_squared->color = rbtree_color::red;
                    node = parent_squared;
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        root_ref = eastl::rbtree_rotate_right(node, root_ref);
                    }

                    node->parent->color = rbtree_color::black;
                    parent_squared->color = rbtree_color::red;
                    root_ref = eastl::rbtree_rotate_left(parent_squared, root_ref);
                }
            }
        }

        root_ref->color = rbtree_color::black;
    }

    void eastl::rbtree_erase(rbtree_node_base* node, rbtree_node_base* anchor)
    {
        rbtree_node_base*& root_ref = anchor->parent;
        rbtree_node_base*& leftmost_ref = anchor->left;
        rbtree_node_base*& rightmost_ref = anchor->right;
        rbtree_node_base* successor = node;
        rbtree_node_base* child = nullptr;
        rbtree_node_base* child_parent = nullptr;

        if (successor->left == nullptr) // node has at most one non-null child.
        {
            child = successor->right; // child might be null.
        }
        else if (successor->right == nullptr) // node has exactly one non-null child.
        {
            child = successor->left; // child is not null.
        }
        else
        {
            // node has two non-null children. Set successor to node's successor. child might be null.
            successor = successor->right;

            while (successor->left)
            {
                successor = successor->left;
            }

            child = successor->right;
        }

        // here we remove node from the tree and fix up the node pointers appropriately around it.

        if (successor == node) // if node was a leaf node (had both null children)...
        {
            child_parent = successor->parent;  // assign replacement's parent.

            if (child != nullptr)
            {
                child->parent = successor->parent;
            }

            if (node == root_ref) // if the node being deleted is the root node...
            {
                root_ref = child; // set the new root node to be the replacement.
            }
            else
            {
                if (node == node->parent->left) // if node is a left node...
                {
                    node->parent->left = child; // make node's replacement node be on the same side.
                }
                else
                {
                    node->parent->right = child;
                }

                // now node is disconnected from the bottom of the tree (recall that in this pathway pNode was determined to be a leaf).
            }

            if (node == leftmost_ref) // if node is the tree begin() node...
            {
                // because node is the tree begin(), node->left must be null.
                // here we assign the new begin() (first node).

                if (node->right && child != nullptr)
                {
                    assert(child != nullptr); // logically child should always be valid.

                    leftmost_ref = eastl::rbtree_get_min_child(child);
                }
                else
                {
                    leftmost_ref = node->parent; // this makes (leftmost_ref == end()) if (node == root node)
                }
            }

            if (node == rightmost_ref) // if node is the tree last (rbegin()) node...
            {
                // because node is the tree rbegin(), node->right must be null.
                // here we assign the new rbegin() (last node)

                if (node->left && child != nullptr)
                {
                    assert(child != nullptr); // logically child should always be valid.

                    rightmost_ref = eastl::rbtree_get_max_child(child);
                }
                else // child == node->left
                {
                    rightmost_ref = node->parent; // makes rightmost_ref == &anchor if node == root_ref
                }
            }
        }
        else // else (successor != node)
        {
            // relink successor in place of node. successor is node's successor.
            // we specifically set successor to be on the right child side of node, so fix up the left child side.

            node->left->parent = successor;
            successor->left = node->left;

            if (successor == node->right) // if node's successor was at the bottom of the tree... (yes that's effectively what this statement means)
            {
                child_parent = successor; // assign replacement's parent.
            }
            else
            {
                child_parent = successor->parent;

                if (child != nullptr)
                {
                    child->parent = child_parent;
                }

                child_parent->left = child;

                successor->right = node->right;

                node->right->parent = successor;
            }

            if (node == root_ref)
            {
                root_ref = successor;
            }
            else if (node == node->parent->left)
            {
                node->parent->left = successor;
            }
            else
            {
                node->parent->right = successor;
            }

            // now node is disconnected from the tree.

            successor->parent = node->parent;

            std::swap(successor->color, node->color);
        }

        // here we do tree balancing as per the conventional red-black tree algorithm.

        if (node->color == rbtree_color::black)
        {
            while ((child != root_ref) && ((child == nullptr) || (child->color == rbtree_color::black)))
            {
                if (child == child_parent->left)
                {
                    rbtree_node_base* temp = child_parent->right;

                    if (temp->color == rbtree_color::red)
                    {
                        temp->color = rbtree_color::black;
                        child_parent->color = rbtree_color::red;
                        root_ref = eastl::rbtree_rotate_left(child_parent, root_ref);
                        temp = child_parent->right;
                    }

                    if (((temp->left == NULL) || (temp->left->color == rbtree_color::black)) &&
                        ((temp->right == NULL) || (temp->right->color == rbtree_color::black)))
                    {
                        temp->color = rbtree_color::red;
                        child = child_parent;
                        child_parent = child_parent->parent;
                    }
                    else
                    {
                        if ((temp->right == nullptr) || (temp->right->color == rbtree_color::black))
                        {
                            temp->left->color = rbtree_color::black;
                            temp->color = rbtree_color::red;
                            root_ref = eastl::rbtree_rotate_right(temp, root_ref);
                            temp = child_parent->right;
                        }

                        temp->color = child_parent->color;
                        child_parent->color = rbtree_color::black;

                        if (temp->right)
                        {
                            temp->right->color = rbtree_color::black;
                        }

                        root_ref = eastl::rbtree_rotate_left(child_parent, root_ref);

                        break;
                    }
                }
                else
                {
                    // the following is the same as above, with right <-> left.

                    rbtree_node_base* temp = child_parent->left;

                    if (temp->color == rbtree_color::red)
                    {
                        temp->color = rbtree_color::black;
                        child_parent->color = rbtree_color::red;

                        root_ref = eastl::rbtree_rotate_right(child_parent, root_ref);
                        temp = child_parent->left;
                    }

                    if (((temp->right == nullptr) || (temp->right->color == rbtree_color::black)) &&
                        ((temp->left == nullptr) || (temp->left->color == rbtree_color::black)))
                    {
                        temp->color = rbtree_color::red;
                        child = child_parent;
                        child_parent = child_parent->parent;
                    }
                    else
                    {
                        if ((temp->left == nullptr) || (temp->left->color == rbtree_color::black))
                        {
                            temp->right->color = rbtree_color::black;
                            temp->color = rbtree_color::red;

                            root_ref = eastl::rbtree_rotate_left(temp, root_ref);
                            temp = child_parent->left;
                        }

                        temp->color = child_parent->color;
                        child_parent->color = rbtree_color::black;

                        if (temp->left)
                        {
                            temp->left->color = rbtree_color::black;
                        }

                        root_ref = eastl::rbtree_rotate_right(child_parent, root_ref);

                        break;
                    }
                }
            }

            if (child != nullptr)
            {
                child->color = rbtree_color::black;
            }
        }
    }
}
