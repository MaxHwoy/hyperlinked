#include <iostream>

#include <hyperlib/math.hpp>
#include <hyperlib/linked_list.hpp>
#include <hyperlib/ts_memory_pool.hpp>
#include <hyperlib/memory.hpp>

int main()
{
    hyper::memory::pool_type car_loader = static_cast<hyper::memory::pool_type>(6);

    std::uint32_t params = hyper::memory::create_allocation_params(hyper::memory::memory_pool_type_main, true, true, 0x10u, 0x00u);

    hyper::ts_memory_pool pool(hyper::memory::malloc_debug(0x1000u, params, __FILE__, __LINE__), 0x1000u, "Hyper Streamer", 7);

    hyper::memory::set_pool_override_info(hyper::memory::memory_pool_type_streamer, pool.get_override_info());

    hyper::memory::initialize_pool(car_loader, hyper::memory::malloc_debug(0x300u, 7u, __FILE__, __LINE__), 0x300u, "Cars");

    hyper::memory::set_memory_pool_top_direction(car_loader, true);

    void* ptr0 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);
    void* ptr1 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);
    void* ptr2 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);
    void* ptr3 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);
    void* ptr4 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);
    void* ptr5 = hyper::memory::malloc_debug(0x100u, car_loader, __FILE__, __LINE__);

    hyper::memory::verify_pool_integrity(car_loader);
    hyper::memory::print_pool_allocations(car_loader);

    hyper::memory::free(ptr0);
    hyper::memory::free(ptr1);
    hyper::memory::free(ptr2);
    hyper::memory::free(ptr3);
    hyper::memory::free(ptr4);
    hyper::memory::free(ptr5);

    hyper::memory::verify_pool_integrity(car_loader);
    hyper::memory::print_pool_allocations(car_loader);

    return 0;
}
