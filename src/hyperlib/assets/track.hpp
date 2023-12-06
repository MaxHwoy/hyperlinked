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
    public:
        char description[0x20];
        char directory[0x20];
        char region_name[0x08];
        char region_directory[0x20];

        std::int32_t location_number;
        char location_directory[0x10];

        std::int32_t location_name;
        std::int32_t drift_type;

        bool is_valid;
        bool is_point_to_point;
        bool has_reverse_version;

        std::uint8_t unused;

        bool is_performance_tuning;

        std::uint16_t track_number_01;
        std::uint16_t track_number_02;

        std::uint32_t sun_info_key;
        std::uint32_t gameplay_flags;
        std::uint32_t length;

        float time_to_beat_forward;
        float time_to_beat_reverse;

        std::int32_t score_to_beat_forward;
        std::int32_t score_to_beat_reverse;

        hyper::vector2 track_map_calibration_offset;
        float track_map_calibration_width;

        std::uint16_t track_map_calibration_rotation;
        std::uint16_t track_map_start_angle;
        std::uint16_t track_map_finish_angle;

        std::uint8_t data_00_02[0x02];

        float track_map_calibration_zoom_in;

        std::int32_t difficulty_forward;
        std::int32_t difficulty_reverse;

        std::int16_t seconds_before_shortcuts_allowed;
        std::int16_t drift_seconds_min;
        std::int16_t drift_seconds_max;

        std::uint8_t data_01_02[0x02];

        std::uint8_t traffic_vehicles_max[0x04][0x02];
        std::uint8_t traffic_allowed_near_start;
        std::uint8_t traffic_allowed_near_finish;

        std::int16_t vehicle_race_start_config;

        float traffic_min_initial_distance_from_start;
        float traffic_min_initial_distance_from_finish;
        float traffic_min_initial_distance_between_a;
        float traffic_min_initial_distance_between_b;
        float traffic_oncoming_fraction[0x04];

        hyper::vector2 menu_map_zoom_offset;
        float menu_map_zoom_width;

        std::int32_t menu_map_start_zoomed;

    public:
        static inline std::uint32_t& count = *reinterpret_cast<std::uint32_t*>(0x00B69B98);

        static inline hyper::track_info*& instances = *reinterpret_cast<hyper::track_info**>(0x00B69B94);

        static inline hyper::track_info*& current = *reinterpret_cast<hyper::track_info**>(0x00B69BA0);
    };

    ASSERT_SIZE(sun_info::layer, 0x24);
    ASSERT_SIZE(sun_info, 0x110);
}
