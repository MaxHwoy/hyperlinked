#pragma once

#include <cstdint>
#include <mutex>

#include <hyperlib/shared.hpp>
#include <hyperlib/utils/utils.hpp>

namespace hyper
{
    class memory_pool final
    {
    public:
        class override_info final
        {
        public:
            using malloc = void*(*)(void* pool, alloc_size_t size, std::uint32_t header_size, std::uint32_t params);
            using free = void(*)(void* pool, void* ptr);
            using amount_free = alloc_size_t(*)(void* pool);
            using largest_free = alloc_size_t(*)(void* pool);

        private:
            const char* name_;
            void* pool_;
            void* memory_;
            alloc_size_t size_;
            malloc malloc_;
            free free_;
            amount_free amount_free_;
            largest_free largest_free_;

        public:
            override_info() = default;
            override_info(const override_info& other) = default;
            override_info(override_info&& other) = default;
            override_info& operator=(const override_info& other) = default;
            override_info& operator=(override_info&& other) = default;
            override_info(const char* name, void* pool, void* memory, alloc_size_t size, malloc malloc_ptr, free free_ptr, amount_free amount_free_ptr, largest_free largest_free_ptr);

            auto name() const -> const char*;
            auto allocate_memory(char pool_number, alloc_size_t size, const char* debug_text, std::int32_t debug_line, std::uint32_t params) const -> void*;
            void release_memory(void* ptr) const;
            auto get_amount_free() const -> alloc_size_t;
            auto get_largest_free_block() const -> alloc_size_t;
        };

        constexpr inline static std::uint32_t debug_free_fill = 0xEEEEEEEE;
        constexpr inline static std::uint32_t debug_alloc_fill = 0xAAAAAAAA;
        constexpr inline static std::uint32_t free_magic = 0x44443333;
        constexpr inline static char alloc_magic = 0x22;
        constexpr inline static char alloc_override_magic = 0x69;

    private:
        struct buffer_block : public linked_node<buffer_block>
        {
            /* 0x08 */ void* buffer;
            /* 0x0C */ alloc_size_t size;
        };

        struct free_block : public linked_node<free_block>
        {
            /* 0x08 */ alloc_size_t size;
            /* 0x0C */ std::uint32_t magic_number;
        };

        struct alloc_block : public linked_node<alloc_block>
        {
#if defined(TARGET_64BIT)
            /* 0x10 */ alloc_size_t size;
            /* 0x18 */ alloc_size_t requested_size;
            /* 0x20 */ char pool_number;
            /* 0x21 */ char magic_number;
            /* 0x22 */ std::uint16_t prepad;
#if defined(TRACE_HEAP_ALLOCATIONS)
            /* 0x24 */ std::int32_t debug_line;
            /* 0x28 */ const char* debug_text;
#else
            /* 0x24 */ std::uint32_t postpad;
#endif
#else
            /* 0x08 */ alloc_size_t size;
            /* 0x0C */ alloc_size_t requested_size;
            /* 0x10 */ char pool_number;
            /* 0x11 */ char magic_number;
            /* 0x12 */ std::uint16_t prepad;
            /* 0x14 */ std::uint32_t postpad;
#if defined(TRACE_HEAP_ALLOCATIONS)
            /* 0x18 */ std::int32_t debug_line;
            /* 0x1C */ const char* debug_text;
#endif
#endif
        };

        memory_pool(const memory_pool&) = delete;
        memory_pool& operator=(const memory_pool&) = delete;

        void verify_pool_integrity_internal(bool verify_free_pattern);

        void add_free_memory(void* address, alloc_size_t size, const char* debug_name);

        auto allocate_memory_internal(alloc_size_t size, std::uint32_t alignment, std::uint32_t offset, bool start_from_top, bool use_best_fit, alloc_size_t* new_size, std::uint16_t* prepad) -> void*;

        void print_allocations_internal();

    public:
        memory_pool();

        ~memory_pool();

        void initialize(void* address, alloc_size_t size, char pool_number, const char* debug_name);

        void close();

        auto allocate_memory(alloc_size_t size, std::uint32_t alignment, std::uint32_t offset, bool start_from_top, bool use_best_fit, const char* debug_text, std::int32_t debug_line) -> void*;

        void release_memory(void* ptr);

        void verify_pool_integrity(bool verify_free_pattern);

        auto get_amount_free() -> alloc_size_t;

        auto get_largest_free_block() -> alloc_size_t;

        auto get_allocations(void** allocations, std::uint32_t max_allocations) -> std::uint32_t;

        void print_allocations();

        inline bool initialized() const
        {
            return this->initialized_;
        }

        inline bool is_unlimited() const
        {
            return this->unlimited_pool_;
        }

        inline auto pool_name() const -> const char*
        {
            return this->debug_name_;
        }

        inline bool is_inside_initial_zone(const void* ptr) const
        {
            return ptr >= this->initial_address_ && ptr < reinterpret_cast<const char*>(this->initial_address_) + this->initial_size_;
        }

        inline void set_debug_fill(bool enabled)
        {
            this->debug_fill_enabled_ = enabled;
        }

#if !defined(USE_HYPER_MEMORY)
        inline void set_debug_tracing(bool enabled)
        {
            this->debug_tracing_enabled_ = enabled;
        }
#endif

        inline void set_limitness(bool unlimited)
        {
            this->unlimited_pool_ = unlimited;
        }

        inline auto debug_name() const -> const char*
        {
            return this->debug_name_;
        }

        static inline auto allocation_magic(const void* ptr) -> char
        {
            return (reinterpret_cast<const alloc_block*>(ptr) - 1)->magic_number;
        }

        static inline auto allocation_pool(const void* ptr) -> char
        {
            return (reinterpret_cast<const alloc_block*>(ptr) - 1)->pool_number;
        }

        static inline auto allocation_size(const void* ptr) -> alloc_size_t
        {
            return (reinterpret_cast<const alloc_block*>(ptr) - 1)->requested_size;
        }

        constexpr static inline auto minimum_allocation() -> alloc_size_t
        {
            return sizeof(alloc_block);
        }

    private:
#if defined(USE_HYPER_MEMORY)
        linked_list<free_block> free_block_list_;
        linked_list<alloc_block> alloc_block_list_;
        linked_list<buffer_block> buffer_block_list_;
        const char* debug_name_;
        void* initial_address_;
        alloc_size_t initial_size_;
        alloc_size_t num_allocations_;
        alloc_size_t total_num_allocations_;
        alloc_size_t total_allocated_;
        alloc_size_t amount_allocated_;
        alloc_size_t most_amount_allocated_;
        alloc_size_t amount_free_;
        alloc_size_t least_amount_free_;
        bool debug_fill_enabled_;
        bool unlimited_pool_;
        char pool_number_;
        bool initialized_;
#if defined(CONCURRENT_POOL_ACCESS)
        std::mutex mutex_;
#endif
#else
        const char* debug_name_;
        linked_list<free_block> free_block_list_;
        linked_list<alloc_block> alloc_block_list_;
        linked_list<buffer_block> buffer_block_list_;
        void* initial_address_;
        alloc_size_t initial_size_;
        alloc_size_t num_allocations_;
        alloc_size_t total_num_allocations_;
        alloc_size_t total_allocated_;
        alloc_size_t amount_allocated_;
        alloc_size_t most_amount_allocated_;
        alloc_size_t amount_free_;
        alloc_size_t least_amount_free_;
        bool debug_fill_enabled_;
        bool debug_tracing_enabled_;
        bool unlimited_pool_;
        bool initialized_;
        bmutex mutex_;
        char pool_number_;

        ASSERT_SIZE(memory_pool::override_info, 0x20);
#endif
    };

#if !defined(USE_HYPER_MEMORY)
    ASSERT_SIZE(memory_pool, 0x64);
#endif
}
