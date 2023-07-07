#include <hyperlib/streamer/sections.hpp>

namespace hyper
{
    auto visible_section::manager::find_closest_boundary(const vector2& position, float& distance) -> const boundary*
    {
        boundary* closest = nullptr;

        distance = std::numeric_limits<float>::infinity();

        for (boundary* i = this->drivable_boundary_list.begin(); i != this->drivable_boundary_list.end(); i = i->next())
        {
            float outside = this->get_distance_outside(i, position, distance);

            if (outside == 0.0f)
            {
                this->drivable_boundary_list.remove(i);

                this->drivable_boundary_list.add_before(i, this->drivable_boundary_list.begin());

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

        if (depth_name == closest->depth_names[0])
        {
            return this->get_drivable_section(closest->depth_sections[0]);
        }

        if (depth_name == closest->depth_names[1])
        {
            return this->get_drivable_section(closest->depth_sections[1]);
        }

        return this->get_drivable_section(closest->section_number);
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

    auto visible_section::manager::get_drivable_section(std::uint16_t section_number) -> const drivable*
    {
        for (drivable* i = this->drivable_section_list.begin(); i != this->drivable_section_list.end(); i = i->next())
        {
            if (i->section_number == section_number)
            {
                this->drivable_section_list.remove(i);

                this->drivable_section_list.add_before(i, this->drivable_section_list.begin());

                return i;
            }
        }

        return nullptr;
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

                if (position.y > elevation && elevation > maximum)
                {
                    maximum = elevation;

                    depth_name = poly.depth_name;
                }
            }
        }

        return depth_name;
    }
}
