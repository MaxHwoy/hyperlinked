#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/udata.hpp>
#include <hyperlib/collections/bstl.hpp>
#include <hyperlib/collections/eastl.hpp>

namespace hyper
{
    class grid final
    {
    public:
        enum class elem_type : std::uint32_t
        {
            instance = 0x0,
            trigger = 0x1,
            object = 0x2,
            road_segment = 0x3,
            count = 0x4,
        };

        struct node_elem
        {
            std::uint32_t index;
            elem_type type;
        };

        struct node_elem_list : public eastl::list<node_elem, bstl::allocator>
        {
        };

        struct callback_func_list : public eastl::list<void(*)(void), bstl::allocator>
        {
        };

        struct node
        {
            node_elem_list* dynamic_elements;
            std::uint16_t node_index;
            std::uint16_t island;
            std::uint8_t elem_counts[0x04];
            std::uint16_t elem_offsets[0x04];
        };

        struct island
        {
            std::uint16_t section_number;
            std::uint16_t origin_row;
            std::uint16_t origin_col;
            std::uint16_t width;
            std::uint16_t height;
        };

        struct manager
        {
            vector4 minimum;
            float edge_size;
            float inverted_edge_size;
            unsigned int row_count;
            unsigned int col_count;
            node** nodes;
            bool allocated_nodes_locally;
            bool nodes_populated;
        };

        class maker
        {
        public:
            void set_start_position();

        public:
            manager* the_grid;
            ugroup* map_group;
            ugroup* island_group;
            std::uint16_t island_section_number;
            island* the_island;
            island* largest_island;
            bool start_position_set;
            vector3 startPos;
            vector4 full_grid_minimum;
            std::uint32_t full_grid_row_count;
            std::uint32_t full_grid_col_count;
            float full_grid_edge_size;
            island maximal_island;
            node** nodes;
            callback_func_list waiting_on_init_callbacks;

        public:
            static inline maker*& instance = *reinterpret_cast<maker**>(0x00B77EB8);
        };
    };

    ASSERT_SIZE(grid::node_elem, 0x08);
    ASSERT_SIZE(grid::node_elem_list, 0x10);
    ASSERT_SIZE(grid::callback_func_list, 0x10);
    ASSERT_SIZE(grid::node, 0x14);
    ASSERT_SIZE(grid::island, 0x0A);
    ASSERT_SIZE(grid::manager, 0x28);
    ASSERT_SIZE(grid::maker, 0x64);
}
