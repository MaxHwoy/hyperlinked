#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/slot_pool.hpp>

int main()
{
    const size_t count = 11u;

    hyper::instance_pool<std::int32_t>* pool = hyper::slot_pool_manager::new_slot_pool<std::int32_t>(5u, "Test Pool", hyper::memory::pool_type::main);

    // pool->set_flags(pool->get_flags() & ~hyper::slot_pool::flags::warn_if_nonempty_delete);

    std::int32_t* ptrs[count];

    for (size_t i = 0u; i < count; ++i)
    {
        ptrs[i] = pool->allocate();
    }

    for (size_t i = 0u; i < count; ++i)
    {
        *ptrs[i] = i + 1;
    }

    pool->cleanup_expanded_slot_pools();

    pool->free(ptrs[2u]);
    pool->free(ptrs[5u]);
    pool->free(ptrs[6u]);
    pool->free(ptrs[7u]);
    pool->free(ptrs[9u]);
    pool->free(ptrs[10u]);
    
    pool->cleanup_expanded_slot_pools();

    pool->free(ptrs[0u]);

    pool->cleanup_expanded_slot_pools();

    pool->free(ptrs[8u]);

    pool->cleanup_expanded_slot_pools();

    pool->free(ptrs[1u]);
    pool->free(ptrs[3u]);
    pool->free(ptrs[4u]);

    pool->cleanup_expanded_slot_pools();

    hyper::slot_pool_manager::delete_slot_pool(pool);

    return 0;
}
