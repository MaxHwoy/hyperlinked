#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/streamer/sections.hpp>

namespace hyper
{
    class streamer final
    {
    public:
        struct section;

        enum class position_type : std::uint32_t
        {
            player1,
            player2,
        };

        enum class loading_phase : std::uint32_t
        {
            idle = 0x0,
            allocating_texture_sections = 0x1,
            loading_texture_sections = 0x2,
            allocating_geometry_sections = 0x3,
            loading_geometry_sections = 0x4,
            allocating_regular_sections = 0x5,
            loading_regular_sections = 0x6,
            count = 0x7,
        };

        enum class file_type
        {
            permanent = 0x0,
            region = 0x1,
            count = 0x2,
        };

        struct barrier
        {
            vector2 points[2];
        };

        struct info
        {
            std::uint32_t file_size[2];
        };

        struct disc_bundle
        {
            struct member
            {
                std::uint16_t section_number;
                std::uint16_t file_offset;
                section* section;
            };

            size_t file_offset;
            size_t file_size;
            std::uint8_t section_name[11];
            std::uint8_t member_count;
            member members[0x01];
        };

        struct section
        {
            enum class status_type : std::uint32_t
            {
                unloaded = 0x0,
                allocated = 0x1,
                loading = 0x2,
                loaded = 0x3,
                activated = 0x4,
            };

            std::uint8_t name[8];
            std::uint16_t number;
            bool was_rendered;
            std::uint8_t currently_visible;
            status_type status;
            file_type type;
            size_t file_offset;
            size_t size;
            size_t compressed_size;
            size_t perm_size;
            std::int32_t section_priority;
            vector2 center;
            float radius;
            std::uint32_t checksum;
            std::int32_t last_needed_timestamp;
            std::uint32_t unactivated_frame_count;
            std::int32_t loaded_time;
            std::int32_t base_loading_priority;
            std::int32_t loading_priority;
            void* memory;
            disc_bundle* disc;
            size_t loaded_size;
            visible_section::boundary* boundary;
        };

        struct position_entry
        {
            vector3 position;
            std::uint32_t pad0;
            vector3 velocity;
            std::uint32_t pad1;
            vector3 direction;
            std::uint32_t pad2;
            vector2 position2D;
            vector2 velocity2D;
            vector2 direction2D;
            bool position_set;
            bool following_car;
            std::uint16_t current_zone;
            std::uint16_t predicted_zone;
            std::int32_t predicted_zone_valid_time;
            vector2 begin_loading_position;
            float begin_loading_time;
            std::uint32_t sections_to_load_count;
            std::uint32_t loaded_section_count;
            std::uint32_t amount_to_load;
            std::uint32_t amount_loaded;
            bool audio_reading;
            float audio_reading_time;
            vector2 audio_reading_position;
            bool audio_blocking;
            float audio_blocking_time;
            vector2 audio_blocking_position;
        };

    public:
        streamer();

        void activate_section(section& section);

        void add_current_sections(std::uint16_t* sections_to_load, std::uint32_t sections_to_load_count, position_type type);

        void add_section_activate_callback(void(*activate_callback)(std::int32_t, bool));

        auto allocate_section_memory(std::uint32_t* total_needing_allocation) -> void*;

        auto allocate_user_memory(std::uint32_t size, const char* debug_name, std::uint32_t offset) -> void*;

        void block_until_loading_complete();

        void calculate_loading_backlog();

        bool check_loading_bar();

        void clear_current_zones();

        void clear_streaming_positions();

        void close_region();

        bool determine_current_zones(std::uint16_t* zones);

        void determine_streaming_sections();

        void disable_zone_switching(const char* reason);

        void disc_bundle_loaded_callback(disc_bundle* disc);

        void enable_zone_switching();

        auto find_section(std::uint16_t section_number) -> section*;

        void free_section_memory();

        void free_user_memory(void* ptr);

        auto get_predicted_zone(const vector3& position) -> std::uint16_t;

        void handle_loading();


        void init_memory_pool(alloc_size_t pool_size);


        auto jettison_least_important_section() -> section*;

        void jettison_section(section& section);

        void load_disc_bundle(disc_bundle& disc);

        void load_section(section& section);

        bool loader(chunk* block);

        bool make_space_in_pool(alloc_size_t size, bool force_unloading);

        void notify_section_activation(std::uint32_t section_number, bool activated);



        void predict_streaming_position(position_type type, const vector3& position, const vector3& velocity, const vector3& direction, bool is_following_car);



        void ready_to_make_space_in_pool_bridge();

        void refresh_loading();

        void remove_current_streaming_sections();

        void remove_section_activate_callback(void(*activate_callback)(std::int32_t, bool));




        void section_loaded_callback(section* section);

        void service_game_state();

        void service_non_game_state();

        void set_streaming_position(position_type type, const vector3& position);

        void switch_zones(const std::uint16_t* zones);



        void unactivate_section(section& section);

        void unload_everything();

        void unload_section(section& section);

        bool unloader(chunk* block);

        void wait_for_current_loading_to_complete();

    public:
        static void disc_bundle_loaded_callback_static(void* param, bool failed);

        static void section_loaded_callback_static(void* param, bool failed);

    private:
        section* sections;
        std::uint32_t section_count;
        disc_bundle* discs;
        disc_bundle* last_disc;
        info* the_info;
        std::uint32_t barrier_count;
        barrier* barriers;
        std::uint32_t loaded_section_count;
        std::uint32_t loading_section_count;
        std::uint32_t activated_section_count;
        std::uint32_t out_of_memory_section_count;
        std::uint32_t moved_section_count;
        std::uint8_t filenames[2][0x40];
        bool split_screen;
        bool perm_file_loading;
        const char* perm_filename;
        void* perm_file_chunks;
        size_t perm_file_size;
        bool perm_file_chunks_loaded;
        void* perm_file_non_region_a;
        __declspec(align(0x10)) position_entry position_entries[2];
        loading_phase phase;
        float loading_backlog;
        bool current_zone_allocated_but_incomplete;
        bool current_zone_out_of_memory;
        bool current_zone_non_replay_load;
        bool current_zone_far_load;
        std::uint8_t current_zone_name[8];
        float start_loading_time;
        std::uint32_t memory_safety_margin;
        size_t amount_not_rendered;
        size_t amount_jettisoned;
        std::uint32_t jettisoned_section_count;
        section* jettisoned_sections[64];
        bool current_zone_needs_refreshing;
        bool zone_switching_disabled;
        const char* zone_switching_disabled_reason;
        std::uint32_t last_wait_until_rendering_done_frame_count;
        std::uint32_t last_printed_frame_count;
        bool skip_next_handle_load;
        std::uint32_t current_section_count;
        section* current_sections[256];
        span<std::uint8_t> memory_heap;
        alloc_size_t user_memory_allocation_size;
        ts_memory_pool* memory_pool;
        bit_table current_section_table;
        std::uint16_t keep_section_table[4];
        void(*callback)(void*);
        void* callback_param;
        void(*make_space_in_pool_callback)(void*);
        void* make_space_in_pool_callback_param;
        std::uint32_t make_space_in_pool_size;
        std::uint32_t section_activate_callback_count;
        void(*section_activate_callback[4])(std::int32_t, bool);
        bool make_space_in_pool_force_defrag;

    public:
        static inline streamer& instance = *reinterpret_cast<streamer*>(0x00B70650);

    private:
        static inline array<std::uint8_t, 350u> section_table_memory_ = array<std::uint8_t, 350u>(0x00B68F68);
    };

    CREATE_ENUM_EXPR_OPERATORS(streamer::position_type);

    ASSERT_SIZE(streamer::barrier, 0x10);
    ASSERT_SIZE(streamer::info, 0x08);
    ASSERT_SIZE(streamer::disc_bundle, 0x1C);
    ASSERT_SIZE(streamer::disc_bundle::member, 0x08);
    ASSERT_SIZE(streamer::section, 0x5C);
    ASSERT_SIZE(streamer::position_entry, 0x90);
    ASSERT_SIZE(streamer, 0x780);
}
