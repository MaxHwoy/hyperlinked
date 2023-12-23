#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct quick_spline
    {
        enum class basis_type : std::uint32_t
        {
            overhauser,
        };

        enum class end_point_type : std::uint32_t
        {
            loop,
            line,
            extrapolated,
        };

        std::uint32_t key;
        end_point_type end_point;
        basis_type basis;
        float max_param;
        float min_param;
        float length;
        std::uint8_t control_points_dirty;
        std::uint8_t buffer_was_allocated;
        std::uint8_t min_control_points;
        std::uint16_t max_control_points;
        std::uint16_t num_control_points;
        vector4* control_point_buffer;
        vector4* control_points;
        matrix4x4* basis_matricies;
    };

    ASSERT_SIZE(quick_spline, 0x2C);
}
