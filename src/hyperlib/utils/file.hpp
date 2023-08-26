#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class file final
    {
    public:
        enum class queued_file_status : std::uint32_t
        {
            waiting = 0x0,
            reading = 0x1,
            done = 0x2,
            error = 0x3,
        };

        struct queued_file_params
        {
            std::int32_t priority;
            size_t block_size;
            bool compressed;
            size_t uncompressed_size;
        };

        struct queued_file : linked_node<queued_file>
        {
            void* buffer;
            const char* filename;
            size_t file_pos;
            size_t num_bytes;
            size_t num_read;
            queued_file_params params;
            void* callback_function;
            void* callback_param;
            void* callback_param2;
            bool callback_mode_use_param2;
            uintptr_t handle;
            queued_file_status status;
            float start_read_time;
        };

        struct queued_file_bundle
        {
            char* read_buffer;
            size_t read_buffer_bot;
            size_t read_buffer_top;
            size_t num_bytes_queued;
            std::uint16_t memory_pool_number;
            std::uint16_t queued_file_count;
            queued_file* queued_files[10];
        };

    public:
        static void set_queued_file_min_priority(std::int32_t priority);

        static void add_queued_file(void* buffer, const char* filename, size_t file_pos, size_t num_bytes, void(*callback)(void*, bool), void* callback_param, queued_file_params* params);

    public:
        static inline std::int32_t& default_queued_file_priority = *reinterpret_cast<std::int32_t*>(0x00A62EC8);

        static inline std::int32_t& min_queued_file_priority = *reinterpret_cast<std::int32_t*>(0x00A99680);
    };

    ASSERT_SIZE(file::queued_file_params, 0x10);
    ASSERT_SIZE(file::queued_file, 0x48);
    ASSERT_SIZE(file::queued_file_bundle, 0x3C);
}
