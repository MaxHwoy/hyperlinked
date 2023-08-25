#pragma once

#include <cstdint>
#include <hyperlib/linked_list.hpp>
#include <hyperlib/memory/memory_pool.hpp>

namespace hyper
{
    __declspec(align(0x10)) class ts_memory_pool final
    {
    private:
        struct heap_buffer : linked_node<heap_buffer>
        {
            void* buffer;
            alloc_size_t size;
        };

        struct heap_header : linked_node<heap_header>
        {
            alloc_size_t size;
            alloc_size_t prepad;
        };

    private:
        static auto override_malloc(void* pool, alloc_size_t size, std::uint32_t header_size, std::uint32_t params) -> void*;

        static void override_free(void* pool, void* ptr);
        
        static auto override_amount_free(void* pool) -> alloc_size_t;
        
        static auto override_largest_free(void* pool) -> alloc_size_t;

        ts_memory_pool(const ts_memory_pool& other) = delete;

        ts_memory_pool& operator=(const ts_memory_pool& other) = delete;

    public:
        ts_memory_pool(void* buffer, alloc_size_t size, const char* debug_name, char pool_number);

        ~ts_memory_pool();

        void close();

        auto allocate_memory(alloc_size_t size, std::uint32_t header_size, std::uint32_t params) -> void*;

        void release_memory(void* ptr);

        auto get_amount_free() const -> alloc_size_t;
        
        auto get_largest_free_block() -> alloc_size_t;

        auto get_override_info() const -> const memory_pool::override_info*;

    private:
        linked_list<heap_buffer> heap_list_; // additional overflow buffers
        linked_list<heap_header> free_list_; // sorted by address, all aligned at 0x80
        linked_list<heap_header> used_list_; // unsorted
        memory_pool::override_info override_;
        const char* debug_name_;
        void* super_buffer_;
        alloc_size_t super_size_;
        alloc_size_t total_size_;
        alloc_size_t amount_free_;
        alloc_size_t amount_allocated_;
        std::uint32_t num_allocations_;
        char pool_number_;
    };
}
