#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct sun_info final
    {
    public:
        enum class sun_texture : std::uint32_t
        {
            center,
            halo,
            major_rays,
            minor_rays,
            ring,
            count,
            first = 0u,
        };

        enum class sun_alpha : std::uint32_t
        {
            blend,
            add,
            count,
            first = 0u,
        };

        struct layer
        {
            sun_texture texture;
            sun_alpha alpha;
            float intensity_scale;
            float size;
            float offset_x;
            float offset_y;
            color32 color;
            std::int32_t angle;
            float sweep_angle_amount;
        };

        std::uint32_t version;
        std::uint32_t key;
        std::uint8_t name[0x18];
        vector3 sun_position;
        vector3 car_shadow_position;
        layer layers[6];

    public:
        static inline span<sun_info>& instances = *reinterpret_cast<span<sun_info>*>(0x00A8FFE4);

        static inline sun_info*& current = *reinterpret_cast<sun_info**>(0x00A8FFEC);

        static inline vector3& sun_origin = *reinterpret_cast<vector3*>(0x00A90190);

        static inline vector3& sun_origin_normalized = *reinterpret_cast<vector3*>(0x00A6B950);
    };

    class track_info final
    {
        // #TODO
    };

    ASSERT_SIZE(sun_info::layer, 0x24);
    ASSERT_SIZE(sun_info, 0x110);
}
