#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/eastl.hpp>

#include <immintrin.h>

class allocator
{
public:
    inline allocator() = default;

    inline allocator(const allocator&) = default;

    inline allocator(allocator&&) = default;

    inline allocator& operator=(const allocator&) = default;

    inline allocator& operator=(allocator&&) = default;

    auto allocate(alloc_size_t size, std::uint32_t flags) -> void*;

    auto allocate(alloc_size_t size, alloc_size_t alignment, alloc_size_t offset, std::uint32_t flags) -> void*;

    void deallocate(void* memory, alloc_size_t size);
};

auto allocator::allocate(alloc_size_t size, std::uint32_t flags) -> void*
{
    return ::malloc(size);
}

auto allocator::allocate(alloc_size_t size, alloc_size_t alignment, alloc_size_t offset, std::uint32_t flags) -> void*
{
    return ::_aligned_malloc(size, alignment);
}

void allocator::deallocate(void* memory, alloc_size_t size)
{
    ::free(memory);
}

struct test
{
    float f;
    const char* s;

    test() = default;

    test(const test& other) = default;

    test(test&& other) = default;

    test& operator=(const test& other) = default;

    test& operator=(test&& other) = default;

    test(float f, const char* s) : f(f), s(s)
    {
    }
};

void run()
{
    hyper::eastl::vector<test, allocator> v({});

    test t(1.0f, "Hozy");
    test s(2.0f, "Max");
    test w(5.0f, "AAAAA");

    v.push_back(t);
    v.push_back(std::move(s));
    v.push_back().s = "None";
    v.emplace_back(3.0f, "rx");

    ::printf("%s\n", v[1].s);
    ::printf("%f\n", v.at(3).f);
    ::printf("%s\n", v.front().s);

    v.reserve(6u);

    v.insert(&v[2], 2u, w);

    ::printf("%s\n", v[2].s);
    ::printf("%s\n", v[3].s);

    v.pop_back();
    v.pop_back();

    ::printf("%f\n", v.back().f);

    ::printf("%u\n", v.capacity());

    v.shrink_to_fit();

    ::printf("%u\n", v.capacity());

    v.erase_no_ordering(v.begin());

    ::printf("%s\n", v.begin()->s);
}

int main()
{
    ::run();

    return 0;
}
