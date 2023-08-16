#include <hyperlib/streamer/sections.hpp>
#include <hyperlib/streamer/streamer.hpp>
#include <hyperlib/world/world.hpp>

namespace hyper
{
    visible_section::manager::manager() :
        drivable_boundary_list(),
        non_drivable_boundary_list(),
        drivable_section_list(),
        texture_section_list(),
        loading_section_list(),
        super_section_list(),
        override_object_list(),
        geometry_used_in_section_info_list(),
        texture_used_in_section_info_list(),
        overlay_list(),
        boundary_chunks(nullptr),
        pack(nullptr),
        has_island_sections(false),
        elev_poly_count(0u),
        elev_polies(nullptr),
        active_overlay(nullptr),
        undo_overlay(nullptr),
        user_infos{},
        allocated_user_info_count(0u),
        user_info_storage_table{},
        unallocated_user_info_list(),
        bit_tables(nullptr),
        enabled_groups{}
    {
        for (std::uint32_t i = 0u; i < std::size(this->user_info_storage_table); ++i)
        {
            this->unallocated_user_info_list.add(reinterpret_cast<unallocated_user_info*>(this->user_info_storage_table + i));
        }
    }

    auto visible_section::manager::get_drivable_section(std::uint16_t section_number) -> const drivable*
    {
        for (drivable* i = this->drivable_section_list.begin(); i != this->drivable_section_list.end(); i = i->next())
        {
            if (i->section_number == section_number)
            {
                this->drivable_section_list.move_front(i);

                return i;
            }
        }

        return nullptr;
    }

    auto visible_section::manager::find_boundary(std::uint16_t section_number) -> const boundary*
    {
        if (game_provider::is_scenery_section_drivable(section_number, this->pack->lod_offset))
        {
            for (const boundary* i = this->drivable_boundary_list.begin(); i != this->drivable_boundary_list.end(); ++i)
            {
                if (i->section_number == section_number)
                {
                    return i;
                }
            }
        }
        else
        {
            for (const boundary* i = this->non_drivable_boundary_list.begin(); i != this->non_drivable_boundary_list.end(); ++i)
            {
                if (i->section_number == section_number)
                {
                    return i;
                }
            }
        }

        return nullptr;
    }

    auto visible_section::manager::find_drivable_section(const vector3& position) -> const drivable*
    {
        float distance;

        const boundary* closest = this->find_closest_boundary(position.as_vector2(), distance);

        if (distance > 0.1f)
        {
            return nullptr;
        }

        if (closest->depth_sections[0] == 0u)
        {
            return this->get_drivable_section(closest->section_number);
        }

        std::uint32_t depth_name = this->get_depth_name(position);

        if (depth_name != 0u)
        {
            if (depth_name == closest->depth_names[0])
            {
                return this->get_drivable_section(closest->depth_sections[0]);
            }

            if (depth_name == closest->depth_names[1])
            {
                return this->get_drivable_section(closest->depth_sections[1]);
            }
        }

        return this->get_drivable_section(closest->section_number);
    }

    auto visible_section::manager::find_closest_boundary(const vector2& position, float& distance) -> const boundary*
    {
        boundary* closest = nullptr;

        distance = std::numeric_limits<float>::infinity();

        for (boundary* i = this->drivable_boundary_list.begin(); i != this->drivable_boundary_list.end(); i = i->next())
        {
            float outside = this->get_distance_outside(i, position, distance);

            if (outside == 0.0f)
            {
                this->drivable_boundary_list.move_front(i);

                distance = 0.0f;

                return i;
            }

            if (outside < distance)
            {
                distance = outside;

                closest = i;
            }
        }

        return closest;
    }

    auto visible_section::manager::allocate_user_info(std::uint16_t section_number) -> user_info&
    {
        user_info*& info = this->user_infos[section_number];

        if (info == nullptr)
        {
            info = reinterpret_cast<user_info*>(this->unallocated_user_info_list.remove_first());

            ::memset(info, 0, sizeof(user_info));

            ++this->allocated_user_info_count;
        }

        ++info->reference_count;

        return *info;
    }

    void visible_section::manager::unallocate_user_info(std::uint16_t section_number)
    {
        user_info*& info = this->user_infos[section_number];

        if (info != nullptr && --info->reference_count == 0u)
        {
            this->unallocated_user_info_list.add(reinterpret_cast<unallocated_user_info*>(info));

            this->allocated_user_info_count--;

            info = nullptr;
        }
    }

    void visible_section::manager::enable_group(std::uint32_t key)
    {
        for (std::uint32_t i = 0u; i < std::size(this->enabled_groups); ++i)
        {
            if (!this->enabled_groups[i])
            {
                this->enabled_groups[i] = key;

                return;
            }
        }
    }

    void visible_section::manager::disable_group(std::uint32_t key)
    {
        for (std::uint32_t i = 0u; i < std::size(this->enabled_groups); ++i)
        {
            if (this->enabled_groups[i] == key)
            {
                this->enabled_groups[i] = 0u;
            }
        }
    }

    bool visible_section::manager::loader(chunk* block)
    {
        if (block->id() == block_id::visible_section_manager)
        {
            this->boundary_chunks = block;

            for (chunk* i = reinterpret_cast<chunk*>(block->data()); i < block->end(); i = i->end())
            {
                switch (i->id())
                {
                    case block_id::visible_section_pack_header:
                        this->loader_pack_header(i);
                        break;

                    case block_id::visible_section_boundaries:
                        this->loader_boundaries(i);
                        break;

                    case block_id::visible_section_drivables:
                        this->loader_drivables(i);
                        break;

                    case block_id::visible_section_specifics:
                        this->loader_specifics(i);
                        break;

                    case block_id::visible_section_loadings:
                        this->loader_loadings(i);
                        break;

                    case block_id::visible_section_elev_polies:
                        this->loader_elev_polies(i);
                        break;
                }
            }

            world::init_visible_zones(visible_section::manager::zone_boundary_model);

            streamer::instance.refresh_loading();

            return true;
        }

        if (block->id() == block_id::visible_section_overlay)
        {
            this->overlay_list.add(reinterpret_cast<overlay*>(block->data()));

            return true;
        }

        return false;
    }

    bool visible_section::manager::unloader(chunk* block)
    {
        if (block->id() == block_id::visible_section_manager)
        {
            this->drivable_boundary_list.clear();
            this->non_drivable_boundary_list.clear();
            this->loading_section_list.clear();
            this->drivable_section_list.clear();
            this->elev_polies = nullptr;
            this->elev_poly_count = 0u;
            this->pack = nullptr;
            this->boundary_chunks = nullptr;

            return true;
        }

        if (block->id() == block_id::visible_section_overlay)
        {
            this->overlay_list.remove(reinterpret_cast<overlay*>(block->data()));

            return true;
        }

        return false;
    }

    void visible_section::manager::loader_pack_header(chunk* block)
    {
        this->pack = reinterpret_cast<visible_section::pack*>(block->data());
        
        visible_section::manager::lod_offset = this->pack->lod_offset;
    }

    void visible_section::manager::loader_boundaries(chunk* block)
    {
        uintptr_t current = reinterpret_cast<uintptr_t>(block->data());
        uintptr_t endaddr = reinterpret_cast<uintptr_t>(block->end());

        while (current < endaddr)
        {
            boundary* bound = reinterpret_cast<boundary*>(current);

            if (game_provider::is_scenery_section_drivable(bound->section_number, this->pack->lod_offset))
            {
                this->drivable_boundary_list.add(bound);
            }
            else
            {
                this->non_drivable_boundary_list.add(bound);
            }

            current += (sizeof(boundary) - sizeof(boundary::points)) + sizeof(vector2) * bound->point_count;
        }
    }

    void visible_section::manager::loader_drivables(chunk* block)
    {
        uintptr_t current = reinterpret_cast<uintptr_t>(block->data());
        uintptr_t endaddr = reinterpret_cast<uintptr_t>(block->end());

        while (current < endaddr)
        {
            drivable* section = reinterpret_cast<drivable*>(current);

            this->drivable_section_list.add(section);

            current += sizeof(drivable) + sizeof(std::uint16_t) * section->max_visible_sections;
        }
    }

    void visible_section::manager::loader_specifics(chunk* block)
    {
    }

    void visible_section::manager::loader_loadings(chunk* block)
    {
        std::uint32_t count = block->size() / sizeof(loading_section);

        for (std::uint32_t i = 0u; i < count; ++i)
        {
            this->loading_section_list.add(reinterpret_cast<loading_section*>(block->data()) + i);
        }
    }

    void visible_section::manager::loader_elev_polies(chunk* block)
    {
        const size_t alignment = 0x10;

        this->elev_polies = reinterpret_cast<elev_poly*>(block->aligned_data(alignment));
        this->elev_poly_count = block->aligned_size(alignment);
    }

    auto visible_section::manager::get_distance_outside(const boundary* bound, const vector2& position, float extra_width) const -> float
    {
        if (math::is_in_bounding_box(position, bound->bbox_min, bound->bbox_max, extra_width))
        {
            std::uint32_t count = static_cast<std::uint32_t>(bound->point_count);

            if (math::is_in_bounding_box(position, bound->bbox_min, bound->bbox_max, 0.0f) &&
                math::is_point_in_polygon(position, bound->points, count))
            {
                return 0.0f;
            }

            for (std::uint32_t i = 1u; i < count; ++i)
            {
                extra_width = math::min(extra_width, math::distance_to_line(position, bound->points[i - 1u], bound->points[i]));
            }

            if (count > 1u)
            {
                extra_width = math::min(extra_width, math::distance_to_line(position, bound->points[count - 1u], bound->points[0u]));
            }
        }

        return extra_width;
    }

    auto visible_section::manager::get_depth_name(const vector3& position) const -> std::uint32_t
    {
        float maximum = -std::numeric_limits<float>::infinity();

        std::uint32_t depth_name = 0u;

        for (std::uint32_t i = 0u; i < this->elev_poly_count; ++i)
        {
            const elev_poly& poly = this->elev_polies[i];

            const vector3& p1 = poly.points[0].as_vector3();
            const vector3& p2 = poly.points[1].as_vector3();
            const vector3& p3 = poly.points[2].as_vector3();

            if (math::is_point_in_triangle(position, p1, p2, p3))
            {
                float elevation = math::get_triangle_height(position.as_vector2(), p1, p2, p3);

                if (position.z > elevation && elevation > maximum)
                {
                    maximum = elevation;

                    depth_name = poly.depth_name;
                }
            }
        }

        return depth_name;
    }
}
