#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/memory_pool.hpp>

namespace hyper
{
    class fast_mem final
    {
    private:
        struct free_block
        {
            free_block* next;
        };

        struct alloc_desc
        {
            std::uint32_t index;
            const char* name;
            std::uint32_t count;
            std::uint32_t high;
        };

    public:
        auto malloc(alloc_size_t size, const char* debug_name) -> void*;

        void free(void* memory, alloc_size_t size, const char* debug_name);

    private:
        free_block* free_lists_[0x40];
        const char* name_;
        std::uint32_t expansion_size_;
        std::uint32_t locks_;
        bool inited_;
        void* block_;
        std::uint32_t bytes_;
        std::uint32_t used_;
        std::uint32_t donated_;
        std::uint32_t alloc_[0x40];
        std::uint32_t avail_[0x40];
        std::uint32_t alloc_over_;
        void* allocator_;
        alloc_desc* track_;
        std::uint32_t track_max_;
        std::uint32_t track_count_;

    public:
        static inline fast_mem& instance = *reinterpret_cast<fast_mem*>(0x00A99720);
    };

    ASSERT_SIZE(fast_mem, 0x334);
}
