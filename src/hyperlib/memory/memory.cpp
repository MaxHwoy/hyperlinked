#include <algorithm>
#include <cassert>
#include <hyperlib/math.hpp>
#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/slot_pool.hpp>

namespace hyper
{
    bool memory::initialized_ = false;

    bool memory::memory_tracing_ = false;

    bool memory::free_unused_buffers_ = true;

    bool memory::verify_free_patterns_ = false;

    bool memory::auto_verify_pool_integrity_ = false;

    size_t memory::total_allocations_ = 0u;

    memory_pool memory::pools_[static_cast<std::uint32_t>(memory::pool_type::count)]{};

    memory::memory_pool_info memory::pool_infos_[static_cast<std::uint32_t>(memory::pool_type::count)]{};

    slot_pool_manager memory::slot_manager_{};

    void memory::initialize()
    {
        if (!memory::initialized_)
        {
            memory::initialize_pool(pool_type::main, nullptr, 0u, "Main Pool");
            memory::pools_[static_cast<std::uint32_t>(pool_type::main)].set_limitness(true);
            memory::initialized_ = true;
        }
    }

    void memory::close()
    {
        if (memory::initialized_)
        {
            memory::pools_[static_cast<std::uint32_t>(pool_type::main)].close();

            memory::initialized_ = false;
        }
    }

    void memory::initialize_pool(memory::pool_type type, void* address, alloc_size_t size, const char* debug_name)
    {
        memory::pool_infos_[static_cast<std::uint32_t>(type)].top_means_larger_address = false;
        memory::pool_infos_[static_cast<std::uint32_t>(type)].overflow_pool_number = -1;
        memory::pool_infos_[static_cast<std::uint32_t>(type)].default_alignment = 0x10u;
        memory::pools_[static_cast<std::uint32_t>(type)].initialize(address, size, static_cast<char>(type), debug_name);
    }

    void memory::close_pool(memory::pool_type type)
    {
        memory::pools_[static_cast<std::uint32_t>(type)].close();
    }

    auto memory::create_persistent_pool(alloc_size_t size) -> pool_type
    {
        std::int32_t persistent = memory::get_free_memory_pool_number();

        assert(persistent >= 0);

#if defined(_DEBUG)
        void* buffer = memory::malloc_debug(size, 0u, __FILE__, __LINE__);
#else
        void* buffer = memory::malloc(size, 0u);
#endif
        memory::initialize_pool(static_cast<pool_type>(persistent), buffer, size, "Persistent Pool");

        return static_cast<pool_type>(persistent);
    }

    auto memory::get_allocation_count() -> size_t
    {
        return memory::total_allocations_;
    }

    void memory::set_pool_override_info(memory::pool_type type, const memory_pool::override_info* override_info)
    {
        memory_pool_info& pool_info = memory::pool_infos_[static_cast<std::uint32_t>(type)];

        pool_info.overflow_pool_number = -1;
        pool_info.override_info = override_info;
    }

    void memory::reserve_memory_pool(memory::pool_type type)
    {
        memory::pool_infos_[static_cast<std::uint32_t>(type)].number_reserved = true;
    }

    void memory::set_memory_pool_debug_fill(memory::pool_type type, bool fill)
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            pool.set_debug_fill(fill);
        }
    }

    void memory::set_memory_pool_limitness(memory::pool_type type, bool unlimited)
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            pool.set_limitness(unlimited);
        }
    }

    void memory::set_memory_pool_top_direction(memory::pool_type type, bool top_means_larger_address)
    {
        memory::pool_infos_[static_cast<std::uint32_t>(type)].top_means_larger_address = top_means_larger_address;
    }

    void memory::set_memory_pool_default_alignment(memory::pool_type type, std::uint32_t alignment)
    {
        memory::pool_infos_[static_cast<std::uint32_t>(type)].default_alignment = alignment;
    }

    void memory::set_memory_pool_overflow_number(memory::pool_type type, std::int32_t number)
    {
        memory::pool_infos_[static_cast<std::uint32_t>(type)].overflow_pool_number = number;
    }

    void memory::verify_pool_integrity(memory::pool_type type)
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            pool.verify_pool_integrity(memory::verify_free_patterns_);
        }
    }

    bool memory::is_in_memory_pool(memory::pool_type type, const void* ptr)
    {
        if (ptr != nullptr)
        {
            return static_cast<pool_type>(memory_pool::allocation_pool(ptr)) == type;
        }

        return false;
    }

    auto memory::get_malloc_size(const void* ptr) -> alloc_size_t
    {
        return ptr == nullptr ? 0u : memory_pool::allocation_size(ptr);
    }
    
    auto memory::get_pool_allocations(memory::pool_type type, void** allocations, std::uint32_t max_allocations) -> std::uint32_t
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            return pool.get_allocations(allocations, max_allocations);
        }

        return 0u;
    }

    void memory::print_pool_allocations(memory::pool_type type)
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            pool.print_allocations();
        }
    }

    auto memory::get_free_memory_pool_number() -> std::int32_t
    {
        for (std::int32_t i = 0; i < static_cast<std::uint32_t>(pool_type::count); ++i)
        {
            memory_pool_info& pool_info = memory::pool_infos_[i];

            if (!pool_info.number_reserved && pool_info.override_info == nullptr && !memory::pools_[i].initialized())
            {
                return i;
            }
        }

        return -1;
    }

    auto memory::get_memory_pool_name(memory::pool_type type) -> const char*
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            return pool.debug_name();
        }

        const memory_pool::override_info* override_info = memory::pool_infos_[static_cast<std::uint32_t>(type)].override_info;

        if (override_info != nullptr)
        {
            return override_info->name();
        }

        return nullptr;
    }

    auto memory::get_memory_pool_index(const char* name) -> std::int32_t
    {
        for (std::int32_t i = 0; i < static_cast<std::uint32_t>(pool_type::count); ++i)
        {
            memory_pool& pool = memory::pools_[i];

            if (pool.initialized())
            {
                if (!::strcmp(pool.debug_name(), name))
                {
                    return i;
                }
            }
            else
            {
                const memory_pool::override_info* override_info = memory::pool_infos_[i].override_info;

                if (override_info != nullptr && !::strcmp(override_info->name(), name))
                {
                    return i;
                }
            }
        }

        return -1;
    }

    auto memory::largest_malloc(std::uint32_t params) -> alloc_size_t
    {
        std::uint32_t pool_number = static_cast<std::uint32_t>(memory::get_pool_number(params));

        memory_pool& pool = memory::pools_[pool_number];

        if (!pool.initialized())
        {
            const memory_pool::override_info* override_info = memory::pool_infos_[pool_number].override_info;

            if (override_info != nullptr)
            {
                return override_info->get_largest_free_block();
            }

            return 0u;
        }

        if (pool.is_unlimited())
        {
            return static_cast<alloc_size_t>(std::numeric_limits<std::int32_t>::max());
        }

        alloc_size_t largest_block = pool.get_largest_free_block();

        alloc_size_t minimum_alloc = memory_pool::minimum_allocation() + memory::get_offset(params);

        if (largest_block < minimum_alloc)
        {
            return 0u;
        }

        largest_block -= minimum_alloc;

        std::uint32_t default_alignment = memory::get_alignment(params);

        if (default_alignment == 0u)
        {
            default_alignment = memory::pool_infos_[pool_number].default_alignment;
        }

        if (default_alignment < 0x80)
        {
            default_alignment = 0x80;
        }

        if (largest_block < default_alignment)
        {
            return 0u;
        }

        return largest_block - default_alignment;
    }

    auto memory::count_free_memory(memory::pool_type type) -> alloc_size_t
    {
        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(type)];

        if (pool.initialized())
        {
            if (pool.is_unlimited())
            {
                return static_cast<alloc_size_t>(std::numeric_limits<std::int32_t>::max());
            }

            return pool.get_amount_free();
        }

        const memory_pool::override_info* override_info = memory::pool_infos_[static_cast<std::uint32_t>(type)].override_info;

        if (override_info != nullptr)
        {
            return override_info->get_amount_free();
        }

        return 0u;
    }

    auto memory::ware_malloc(alloc_size_t size, const char* debug_text, std::uint32_t debug_line, std::uint32_t params) -> void*
    {
        pool_type pool_number = memory::get_pool_number(params);

        memory_pool_info* pool_info = memory::pool_infos_ + static_cast<std::uint32_t>(pool_number);

        std::int32_t overflow_pool_number = pool_info->overflow_pool_number;

        if (overflow_pool_number >= 0)
        {
            std::uint32_t overflow_params = memory::overwrite_pool(params, overflow_pool_number);

            if (memory::largest_malloc(overflow_params) > size)
            {
                pool_number = static_cast<pool_type>(pool_info->overflow_pool_number);

                pool_info = memory::pool_infos_ + static_cast<std::uint32_t>(pool_number);

                params = overflow_params;
            }
        }

        if (pool_info->top_means_larger_address)
        {
            params = memory::toggle_top_start(params);
        }

        const memory_pool::override_info* override_info = pool_info->override_info;

        if (override_info != nullptr)
        {
            return override_info->allocate_memory(static_cast<char>(pool_number), size, debug_text, static_cast<std::int32_t>(debug_line), params);
        }

        if (!memory::initialized_)
        {
            memory::initialize();
        }

        if (memory::auto_verify_pool_integrity_)
        {
            memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(pool_number)];

            if (pool.initialized())
            {
                pool.verify_pool_integrity(memory::verify_free_patterns_);
            }
        }

        std::uint32_t alignment = memory::get_alignment(params);

        if (alignment == 0u)
        {
            alignment = pool_info->default_alignment;
        }

        assert(math::is_pow_2(alignment));

        memory_pool& pool = memory::pools_[static_cast<std::uint32_t>(pool_number)];

        std::uint32_t offset = memory::get_offset(params);
        
        assert(pool.initialized());

        void* ptr = pool.allocate_memory(size, alignment, offset, memory::get_start_from_top(params), memory::get_use_best_fit(params), debug_text, static_cast<std::int32_t>(debug_line));

        if (ptr == nullptr)
        {
            pool.print_allocations();

            alloc_size_t amount_free = pool.get_amount_free();

            alloc_size_t largest = memory::largest_malloc(params);

            const char* name = pool.pool_name();

            ::printf("ERROR:  Out of memory in pool %s allocating %s (size = %d).  Largest possible = %d  Total = %d\n", name, debug_text, size, largest, amount_free);

            assert(false);
        }

        memory::total_allocations_++;

        return ptr;
    }

    auto memory::game_malloc(alloc_size_t size, const void* caller_address, std::uint32_t params) -> void*
    {
        return memory::ware_malloc(size, reinterpret_cast<const char*>(caller_address), static_cast<std::uint32_t>(-1), params);
    }

    auto memory::malloc(alloc_size_t size, std::uint32_t params) -> void*
    {
        return memory::ware_malloc(size, nullptr, 0u, params);
    }

    auto memory::malloc_debug(alloc_size_t size, std::uint32_t params, const char* debug_text, std::uint32_t debug_line) -> void*
    {
        return memory::ware_malloc(size, debug_text, debug_line, params);
    }

    void memory::free(void* ptr)
    {
        if (ptr != nullptr)
        {
            char magic = memory_pool::allocation_magic(ptr);

            if (magic == memory_pool::alloc_magic)
            {
                memory::pools_[memory_pool::allocation_pool(ptr)].release_memory(ptr);
            }
            else if (magic == memory_pool::alloc_override_magic)
            {
                const memory_pool::override_info* override_info = memory::pool_infos_[memory_pool::allocation_pool(ptr)].override_info;

                assert(override_info != nullptr);

                override_info->release_memory(ptr);
            }
            else
            {
                assert(false);
            }
        }
    }

    auto memory::new_slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type) -> slot_pool*
    {
        slot_pool* pool = slot_pool_manager::new_slot_pool(slot_size, slot_count, name, type);

        if (pool != nullptr && memory::slot_manager_.initialized())
        {
            memory::slot_manager_.add_slot_pool(pool);
        }

        return pool;
    }

    auto memory::delete_slot_pool(slot_pool* pool)
    {
        if (pool)
        {
            if (memory::slot_manager_.initialized())
            {
                memory::slot_manager_.remove_slot_pool(pool);
            }

            slot_pool_manager::delete_slot_pool(pool);
        }
    }

    auto memory::is_slot_pool_full(const slot_pool* pool) -> alloc_size_t
    {
        return pool->is_full();
    }

    auto memory::count_total_slots(const slot_pool* pool) -> alloc_size_t
    {
        return pool->total_slots();
    }

    auto memory::count_free_slots(const slot_pool* pool) -> alloc_size_t
    {
        return pool->free_slots();
    }

    auto memory::malloc_slot(slot_pool* pool) -> void*
    {
        return pool->malloc();
    }

    void memory::free_slot(slot_pool* pool, void* ptr)
    {
        pool->free(ptr);
    }
}
