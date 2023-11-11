#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class track_path final
    {
    public:
        struct zone
        {
            enum class type : std::int32_t
            {
                none = -1,
                reset = 0,
                reset_to_point,
                guided_reset,
                tunnel,
                overpass,
                overpass_small,
                streamer_prediction,
                garage,
                hidden,
                traffic_pattern,
                dynamic,
                neighborhood,
                jump_camera,
                no_cop_spawn,
                pursuit_start,
                highway,
                canyon_drop,
                vertigo_camera,
                count,
            };

            type zone_type;
            vector2 position;
            vector2 direction;
            float elevation;
            char zone_source;
            char cached_index;
            std::int16_t visit_info;
            void* user_data;
            vector2 bbox_min;
            vector2 bbox_max;
            std::uint32_t data[0x04];
            std::uint16_t NumPoints;
            std::uint16_t byte_size;
            vector2 points[0x01];
        };

        struct barrier
        {
            vector2 points[2];
            bool enabled;
            bool pad;
            bool player_barrier;
            bool left_handed;
            std::uint32_t group_key;
        };

        struct zone_info
        {
            std::uint32_t zone_count;
            zone* first;
            zone* last;
            vector2 cached_bbox_min;
            vector2 cached_bbox_max;
            std::uint32_t cached_zone_count;
            std::uint32_t cache_hit_count;
            std::uint32_t cache_rebuild_count;
            std::uint32_t full_rebuild_count;
            zone* cached_zones[0x08];
        };

        struct manager
        {
        public:
            void disable_all_barriers();

            void enable_barriers(const char* barrier_name);

            auto find_zone(const vector2* position, zone::type type, const zone* prev) -> zone*;

        public:
            std::uint32_t zone_count;
            std::uint32_t zone_size;
            zone* zones;
            zone_info zone_infos[static_cast<std::uint32_t>(zone::type::count)];
            std::uint32_t most_cached_zones;
            std::uint32_t barrier_count;
            barrier* barriers;
            
        public:
            static inline manager& instance = *reinterpret_cast<manager*>(0x00B70EA0);
        };
    };

    ASSERT_SIZE(track_path::zone, 0x4C);
    ASSERT_SIZE(track_path::barrier, 0x18);
    ASSERT_SIZE(track_path::zone_info, 0x4C);
    ASSERT_SIZE(track_path::manager, 0x570);
}
