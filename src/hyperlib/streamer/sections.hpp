#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/assets/lights.hpp>
#include <hyperlib/assets/flares.hpp>
#include <hyperlib/assets/triggers.hpp>

namespace hyper
{
    class game_provider final
    {
    public:
        inline static auto maximum_sections() -> std::uint16_t
        {
            return 27u * game_provider::section_factor_;
        }

        inline static auto get_drivable_from_lod_number(std::uint16_t section_number, std::uint32_t lod) -> std::uint16_t
        {
            return static_cast<std::uint16_t>(section_number - lod);
        }

        inline static auto get_lod_from_drivable_number(std::uint16_t section_number, std::uint32_t lod) -> std::uint16_t
        {
            return static_cast<std::uint16_t>(section_number + lod);
        }

        inline static bool is_scenery_section_drivable(std::uint16_t section_number, std::uint32_t lod)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            return c >= 1u && c <= 20u && i >= 1u && i < lod;
        }

        inline static bool is_lod_scenery_section_number(std::uint16_t section_number, std::uint32_t lod)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            return c >= 1u && c <= 20u && i >= lod && i < (lod << 1);
        }

        inline static bool is_panorama_section_number(std::uint16_t section_number, std::uint32_t lod)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            return c >= 1u && c <= 20u && i > (lod << 1);
        }

        inline static bool is_regular_section_number(std::uint16_t section_number)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;

            return c >= 1u && c <= 20u;
        }

        inline static bool is_raw_asset_section_number(std::uint16_t section_number)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            if (c < 21u || c > 26u)
            {
                return false;
            }

            if (c >= 24u)
            {
                return i != 0u;
            }

            return true;
        }

        inline static bool is_textures_asset_section(std::uint16_t section_number)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;

            return c == 25u || c == 23u;
        }

        inline static bool is_geometry_asset_section(std::uint16_t section_number)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;

            return c == 24u || c == 21u;
        }

        inline static bool is_island_section_number(std::uint16_t section_number)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            return c == 25u && i >= ((f * 10u - f) / 10u); // wowie
        }

        inline static bool is_shared_section_number(std::uint16_t section_number)
        {
            return section_number == game_provider::shared_solid_section() || 
                   section_number == game_provider::shared_texture_section() || 
                   section_number == game_provider::shared_scenery_section();
        }

        inline static void get_section_name_from_number(std::uint16_t section_number, char* buffer)
        {
            const std::uint16_t f = game_provider::section_factor_;
            const std::uint32_t c = section_number / f;
            const std::uint32_t i = section_number - f * c;

            ::sprintf(buffer, "%c%d", static_cast<char>(c) + 'A', i);
        }

        inline static bool are_sections_extended()
        {
            return !!(game_provider::section_factor_ - 100u);
        }

        inline static void set_extended_sections(bool extended)
        {
            game_provider::section_factor_ = extended ? 1000u : 100u;
        }

        inline static auto shared_solid_section() -> std::uint16_t
        {
            return 24u * game_provider::section_factor_;
        }

        inline static auto shared_texture_section() -> std::uint16_t
        {
            return 25u * game_provider::section_factor_;
        }

        inline static auto shared_scenery_section() -> std::uint16_t
        {
            return 26u * game_provider::section_factor_;
        }

    private:
        static std::uint16_t section_factor_;
    };

    class visible_section final
    {
    public:
        struct pack
        {
            std::uint16_t lod_offset;
            bool use_extended_sections;
            std::uint32_t section_count;
            std::uint16_t sections[1];
        };

        struct boundary : linked_node<boundary>
        {
            std::uint16_t section_number;
            std::uint8_t point_count;
            std::uint8_t panorama_boundary;
            std::uint16_t depth_sections[2];
            std::uint32_t depth_names[2];
            vector2 bbox_min;
            vector2 bbox_max;
            vector2 center;
            vector2 points[1];
        };

        struct drivable : linked_node<drivable>
        {
            boundary* boundary;
            std::uint16_t section_number;
            std::uint8_t most_visible_sections;
            std::uint8_t max_visible_sections;
            std::uint16_t visible_section_count;
            std::uint16_t visible_sections[1];
        };

        struct loading : linked_node<loading>
        {
            std::uint8_t name[0x0F];
            std::uint8_t DefaultFlag;
            std::uint16_t drivable_section_count;
            std::uint16_t drivable_sections[0x10];
            std::uint16_t extra_section_count;
            std::uint16_t extra_sections[0x08];
        };

        struct elev_poly
        {
            std::uint32_t depth_name;
            std::uint32_t padding[3];
            vector3pad points[3];
        };

        struct __declspec(align(0x04)) textures : linked_node<textures>
        {
            std::uint16_t section_number;
            std::uint16_t visible_from_section_count;
            std::uint16_t visible_from_sections[0x200];
            std::uint16_t super_section_count;
            std::uint16_t super_sections[0x20];
        };

        struct __declspec(align(0x04)) super : linked_node<super>
        {
            std::uint16_t section_number;
            __declspec(align(0x04)) std::uint16_t section_count;
            std::uint16_t sections[0x10];
        };

        struct override_object : linked_node<override_object>
        {
            std::uint16_t section_number;
            std::uint16_t touched;
            std::uint8_t object_name[0x40];
        };

        struct used_in_section_info : linked_node<used_in_section_info>
        {
            std::uint8_t name[0x40];
            bool is_texture;
            bool always_visible;
            std::uint32_t used_in_section_count;
            std::uint16_t used_in_sections[0x80];
            std::uint32_t visible_from_section_count;
            std::uint16_t visible_from_sections[0x100];
            std::uint32_t visible_from_texture_section_count;
            std::uint16_t visible_from_texture_sections[100];
            std::uint32_t placed_in_section_count;
            std::uint16_t placed_in_sections[0x100];
            float placement_score;
            float memory_score;
            float bandwidth_score;
            const char* placement_reason;
        };

        struct overlay : linked_node<overlay>
        {
            struct entry
            {
                char add_remove;
                char pad;
                std::uint16_t drivable_section_number;
                std::uint16_t section_number;
            };

            std::uint8_t name[40];
            std::uint32_t entry_count;
            overlay::entry entries[1];
        };

        struct bit_table
        {
            std::uint8_t bits[0x15E];
        };

        struct user_info
        {
            std::uint32_t reference_count;
            scenery::pack* scenery;
            flare::pack* flares;
            light::pack* lights;
            struct SmokeableSpawnerPack* spawners;
            struct ParkedCarPack* parked;
            trigger::pack* triggers;
        };

        struct unallocated_user_info : linked_node<unallocated_user_info>
        {
        };

        struct __declspec(align(0x04)) group_info
        {
            char* selection_set_name;
            bool used_for_topology;
        };

        struct manager
        {
        private:
            void loader_pack_header(chunk* block);

            void loader_boundaries(chunk* block);

            void loader_drivables(chunk* block);

            void loader_specifics(chunk* block);

            void loader_loadings(chunk* block);

            void loader_elev_polies(chunk* block);

        public:
            manager();

            auto get_drivable_section(const vector3& position) -> const drivable*;

            auto get_depth_name(const vector3& position) const -> std::uint32_t;

            auto get_sections_to_load(const loading* loading, std::uint16_t* sections_to_load, std::uint32_t max_sections) -> std::uint32_t;

            auto find_boundary(std::uint16_t section_number) -> const boundary*;

            auto find_drivable_section(std::uint16_t section_number) -> const drivable*;

            auto find_loading_section(std::uint16_t section_number) -> const loading*;

            auto find_closest_boundary(const vector2& position, float& distance) -> const boundary*;

            auto allocate_user_info(std::uint16_t section_number) -> user_info&;

            void unallocate_user_info(std::uint16_t section_number);

            void enable_group(std::uint32_t key);

            void disable_group(std::uint32_t key);

            void disable_all_groups();

            bool loader(chunk* block);

            bool unloader(chunk* block);

            inline auto get_lod_offset() -> std::uint32_t
            {
                return this->pack == nullptr ? 0u : this->pack->lod_offset;
            }

        public:
            static auto get_distance_outside(const boundary* bound, const vector2& position, float extra_width) -> float;

            static auto get_group_info(const char* group_name) -> const group_info*;

        public:
            linked_list<boundary> drivable_boundary_list;
            linked_list<boundary> non_drivable_boundary_list;
            linked_list<drivable> drivable_section_list;
            linked_list<textures> texture_section_list;
            linked_list<loading> loading_section_list;
            linked_list<super> super_section_list;
            linked_list<override_object> override_object_list;
            linked_list<used_in_section_info> geometry_used_in_section_info_list;
            linked_list<used_in_section_info> texture_used_in_section_info_list;
            linked_list<overlay> overlay_list;
            chunk* boundary_chunks;
            pack* pack;
            bool has_island_sections;
            std::uint32_t elev_poly_count;
            elev_poly* elev_polies;
            overlay* active_overlay;
            overlay* undo_overlay;
            user_info* user_infos[27000];
            std::uint32_t allocated_user_info_count;
            user_info user_info_storage_table[0x200];
            linked_list<unallocated_user_info> unallocated_user_info_list;
            bit_table* bit_tables;
            std::uint32_t enabled_groups[0x200];

        public:
            static manager instance;

            static inline std::uint32_t& section_lod_offset = *reinterpret_cast<std::uint32_t*>(0x00A72C2C);

            static inline std::uint32_t& current_zone_number = *reinterpret_cast<std::uint32_t*>(0x00A71C1C);

            static inline geometry::model*& zone_boundary_model = *reinterpret_cast<geometry::model**>(0x00B69BE8);

            static inline array<group_info, 5u> group_info_table = array<group_info, 5u>(0x00A72C30);
        };
    };

    ASSERT_SIZE(visible_section::pack, 0x0C);
    ASSERT_SIZE(visible_section::boundary, 0x38);
    ASSERT_SIZE(visible_section::drivable, 0x14);
    ASSERT_SIZE(visible_section::loading, 0x4C);
    ASSERT_SIZE(visible_section::elev_poly, 0x40);
    ASSERT_SIZE(visible_section::textures, 0x450);
    ASSERT_SIZE(visible_section::super, 0x30);
    ASSERT_SIZE(visible_section::override_object, 0x4C);
    ASSERT_SIZE(visible_section::used_in_section_info, 0x634);
    ASSERT_SIZE(visible_section::overlay, 0x3C);
    ASSERT_SIZE(visible_section::overlay::entry, 0x06);
    ASSERT_SIZE(visible_section::bit_table, 0x15E);
    ASSERT_SIZE(visible_section::user_info, 0x1C);
    ASSERT_SIZE(visible_section::unallocated_user_info, 0x08);
    ASSERT_SIZE(visible_section::group_info, 0x08);
}
