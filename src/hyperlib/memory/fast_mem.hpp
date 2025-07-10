#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/memory_pool.hpp>
#include <hyperlib/memory/ea_allocator.hpp>

namespace hyper
{
    class fast_mem final
    {
    private:
        struct free_block
        {
            free_block* next;
        };

        struct block_desc
        {
            block_desc* next;
            alloc_size_t size;
            std::uint32_t pad[2];
        };

        struct alloc_desc
        {
            std::uint32_t index;
            const char* name;
            std::uint32_t count;
            std::uint32_t high;
        };

    private:
        static void emergency_initialization(std::uint32_t& bytes, const char*& name, std::uint32_t& expansion_size, std::uint32_t& track_max);

        void core_setup();

        void init();

        void deinit();

        bool assign_to_free(std::uint32_t size);

        bool split_or_expand(std::uint32_t size);

        bool create_block(std::uint32_t list_index);

        auto core_alloc(alloc_size_t size, const char* debug_name) -> void*;

        void core_free(void* ptr);

    public:
        ~fast_mem();

        auto malloc(alloc_size_t size, const char* debug_name) -> void*;

        void free(void* ptr, alloc_size_t size, const char* debug_name);

    private:
        free_block* free_lists_[0x40]; // 0x10 increments
        const char* name_;
        std::uint32_t expansion_size_;
        std::uint32_t locks_;
        bool inited_;
        block_desc* block_;
        std::uint32_t bytes_;
        std::uint32_t used_;
        std::uint32_t donated_;
        std::uint32_t alloc_[0x40];
        std::uint32_t avail_[0x40];
        std::uint32_t alloc_over_;
        ea_allocator::i_allocator* allocator_;
        alloc_desc* track_;
        std::uint32_t track_max_;
        std::uint32_t track_count_;

    public:
        static inline fast_mem& instance = *reinterpret_cast<fast_mem*>(0x00A99720);

        ASSERT_SIZE(fast_mem::free_block, 0x04);
        ASSERT_SIZE(fast_mem::block_desc, 0x10);
        ASSERT_SIZE(fast_mem::alloc_desc, 0x10);
    };

    ASSERT_SIZE(fast_mem, 0x334);
}
