#pragma once

#include <hyperlib/ts_memory_pool.hpp>

namespace hyper
{
    struct bit_table
    {
        std::uint32_t num_bits;
        std::uint8_t* bits;
    };

    struct track_streamer
    {
        void* track_streaming_sections;
        int num_track_streaming_sections;
        void* disc_bundle_sections;
        void* last_disc_bundle_section;
        void* track_streaming_info;
        int num_barriers;
        void* barriers;
        int num_sections_loaded;
        int num_sections_loading;
        int num_sections_activated;
        int num_sections_out_of_memory;
        int num_sections_moved;
        char stream_filenames[2][64];
        bool split_screen;
        bool perm_file_loading;
        const char* perm_filename;
        void* perm_file_chunks;
        int perm_file_size;
        bool perm_file_chunks_loaded;
        void* perm_file_non_region_a;
        __declspec(align(16)) char streaming_position_entries[2][0x90];
        int loading_phase;
        float loading_backlog;
        bool current_zone_allocated_but_incomplete;
        bool current_zone_out_of_memory;
        bool current_zone_non_replay_load;
        bool current_zone_far_load;
        char current_zone_name[8];
        float start_loading_time;
        int memory_safety_margin;
        int amount_not_rendered;
        int amount_jettisoned;
        int num_jettisoned_sections;
        void* jettisoned_sections[64];
        bool current_zone_needs_refreshing;
        bool zone_switching_disabled;
        const char* zone_switching_disabled_reason;
        unsigned int last_wait_until_rendering_done_frame_count;
        unsigned int last_printed_frame_count;
        bool skip_next_handle_load;
        int num_current_streaming_sections;
        void* current_streaming_sections[256];
        void* memory_pool_heap;
        alloc_size_t memory_pool_size;
        alloc_size_t user_memory_allocation_size;
        ts_memory_pool* memory_pool;
        bit_table current_visible_section_table;
        __int16 keep_section_table[4];
        void(__cdecl* callback)(void*);
        void* callback_param;
        void(__cdecl* make_space_in_pool_callback)(void*);
        void* make_space_in_pool_callback_param;
        int make_space_in_pool_size;
        unsigned int num_section_activate_callbacks;
        void(__cdecl* section_activate_callback[4])(int, bool);
        bool make_space_in_pool_force_defrag;
    };
}
