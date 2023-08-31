#include <hyperlib/memory/fast_mem.hpp>
#include <hyperlib/collections/bstl.hpp>

namespace hyper
{
    auto bstl::allocator::allocate(alloc_size_t size, std::uint32_t flags) -> void*
    {
        return fast_mem::instance.malloc(size, nullptr);
    }

    auto bstl::allocator::allocate(alloc_size_t size, alloc_size_t alignment, alloc_size_t offset, std::uint32_t flags) -> void*
    {
        return fast_mem::instance.malloc(size, nullptr);
    }

    void bstl::allocator::deallocate(void* memory, alloc_size_t size)
    {
        fast_mem::instance.free(memory, size, nullptr);
    }
}
