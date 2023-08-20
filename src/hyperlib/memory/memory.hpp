#pragma once

#include <cstdlib>
#include <cstdint>

#include <hyperlib/memory/memory_pool.hpp>

namespace hyper
{
    class slot_pool;

    class slot_pool_manager;

    class memory final
    {
    public:
        enum class pool_type : std::int32_t
        {
            main = 0x00,
            streamer = 0x07,
            online = 0x08,
            count = 0x10,
        };

#if defined(TARGET_64BIT)
        constexpr static inline alloc_size_t memory_alignment = 0x08u;
#else
        constexpr static inline alloc_size_t memory_alignment = 0x04u;
#endif

    private:
        struct memory_pool_info
        {
            bool number_reserved;
            bool top_means_larger_address;
            std::int32_t overflow_pool_number;
            std::uint32_t default_alignment;
            const memory_pool::override_info* override_info;
        };

    public:
        // bMemoryInit
        static void initialize();
        
        // bMemoryClose
        static void close();

        // bInitMemoryPool
        static void initialize_pool(pool_type type, void* address, alloc_size_t size, const char* debug_name);

        // bCloseMemoryPool
        static void close_pool(pool_type type);

        // bMemoryCreatePersistentPool
        static auto create_persistent_pool(alloc_size_t size) -> pool_type;

        // bMemoryGetAllocationNumber
        static auto get_allocation_count() -> size_t;

        // bIsMemoryPoolUnlimited
        static bool is_memory_pool_unlimited(pool_type type);

        // bSetMemoryPoolOverrideInfo
        static void set_pool_override_info(pool_type type, const memory_pool::override_info* override_info);

        // bReserveMemoryPool
        static void reserve_memory_pool(pool_type type);

        // bSetMemoryPoolDebugFill
        static void set_memory_pool_debug_fill(pool_type type, bool fill);

        // bSetMemoryPoolLimitness
        static void set_memory_pool_limitness(memory::pool_type type, bool unlimited);

        // bSetMemoryPoolTopDirection
        static void set_memory_pool_top_direction(pool_type type, bool top_means_larger_address);

        // bSetMemoryPoolDefaultAlignment
        static void set_memory_pool_default_alignment(pool_type type, std::uint32_t alignment);

        // bSetMemoryPoolOverflowNumber
        static void set_memory_pool_overflow_number(pool_type type, std::int32_t number);

        // bVerifyPoolIntegrity
        static void verify_pool_integrity(pool_type type);

        // bIsInMemoryPool
        static bool is_in_memory_pool(pool_type type, const void* ptr);

        // bGetMallocSize
        static auto get_malloc_size(const void* ptr) -> alloc_size_t;

        // bMemoryGetAllocations
        static auto get_pool_allocations(pool_type type, void** allocations, std::uint32_t max_allocations) -> std::uint32_t;

        // bMemoryPrintAllocationsByAddress
        static void print_pool_allocations(pool_type type);

        // bGetFreeMemoryPoolNum
        static auto get_free_memory_pool_number() -> std::int32_t;

        // bGetMemoryPoolDebugName
        static auto get_memory_pool_name(pool_type type) -> const char*;

        // bGetMemoryPoolNum
        static auto get_memory_pool_index(const char* name) -> std::int32_t;

        // bLargestMalloc
        static auto largest_malloc(std::uint32_t params) -> alloc_size_t;

        // bCountFreeMemory
        static auto count_free_memory(pool_type type) -> alloc_size_t;

        // bWareMalloc
        static auto ware_malloc(alloc_size_t size, const char* debug_text, std::uint32_t debug_line, std::uint32_t params) -> void*;

        // bWareMalloc
        static auto game_malloc(alloc_size_t size, const void* caller_address, std::uint32_t params) -> void*;

        // bMalloc
        static auto malloc(alloc_size_t size, std::uint32_t params) -> void*;

        // bMalloc
        static auto malloc_debug(alloc_size_t size, std::uint32_t params, const char* debug_text, std::uint32_t debug_line) -> void*;

        // bFree
        static void free(void* ptr);

        static inline bool initialized()
        {
            return memory::initialized_;
        }

        static inline auto memory_tracing() -> bool
        {
            return memory::memory_tracing_;
        }

        static inline bool free_unused_buffers()
        {
            return memory::free_unused_buffers_;
        }

        static inline bool verify_free_patterns()
        {
            return memory::verify_free_patterns_;
        }

        static inline bool auto_verify_pool_integrity()
        {
            return memory::auto_verify_pool_integrity_;
        }

        static inline void set_memory_tracing(bool value)
        {
            memory::memory_tracing_ = value;
        }

        static inline void set_free_unused_buffers(bool free)
        {
            memory::free_unused_buffers_ = free;
        }

        inline static auto get_pool_number(std::uint32_t params) -> pool_type
        {
            return static_cast<pool_type>(params & 0x0Fu);
        }

        inline static bool get_start_from_top(std::uint32_t params)
        {
            return static_cast<bool>((params >> 6) & 1u);
        }

        inline static bool get_use_best_fit(std::uint32_t params)
        {
            return static_cast<bool>((params >> 7) & 1u);
        }

        inline static auto get_alignment(std::uint32_t params) -> std::uint32_t
        {
            return (params >> 6) & 0x1FFCu;
        }

        inline static auto get_offset(std::uint32_t params) -> std::uint32_t
        {
            return (params >> 17) & 0x1FFCu;
        }

        inline static auto overwrite_pool(std::uint32_t params, std::uint32_t new_pool) -> std::uint32_t
        {
            // note: a ^ (a ^ b) & c = a ^ ((a ^ b) & c) = (a & ~c) | (b & c) from decomp
            return (params & ~0x0Fu) | (new_pool & 0x0F);
        }

        inline static auto toggle_top_start(std::uint32_t params)
        {
            return params ^ 0x40u;
        }

        static inline auto create_allocation_params(pool_type type, bool start_from_top, bool use_best_fit, std::uint32_t alignment, std::uint32_t offset)
        {
            // #TODO is there flag 0x20 or something?
            return (static_cast<std::uint32_t>(type) & 0x0F) |
                   (static_cast<std::uint32_t>(start_from_top) << 6) |
                   (static_cast<std::uint32_t>(use_best_fit) << 7) |
                   ((static_cast<std::uint32_t>(alignment) >> 2) << 8) |
                   ((static_cast<std::uint32_t>(offset) >> 2) << 19);
        }

    private:
        static memory_pool_info pool_infos_[static_cast<std::uint32_t>(pool_type::count)];

        static memory_pool pools_[static_cast<std::uint32_t>(pool_type::count)];

        static slot_pool_manager slot_manager_;

        static bool initialized_;

        static bool memory_tracing_;

        static bool free_unused_buffers_;

        static bool verify_free_patterns_;

        static bool auto_verify_pool_integrity_;

        static size_t total_allocations_;
    };
}
