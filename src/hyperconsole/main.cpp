#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/eastl.hpp>
#include <hyperlib/collections/string.hpp>
#include <hyperlib/memory/fast_mem.hpp>

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

struct a
{
    ~a() { ::printf("A"); }
};

struct b : public a
{
    ~b() { ::printf("B"); }
};

void run()
{
    hyper::vector_string s1{};
    hyper::vector_string s2("Max");
    hyper::vector_string s3 = s2;
    hyper::vector_string s4(s1);
    hyper::vector_string s5(s2);

    s2 = "AAAAA";
    s3 = "";
}

int main()
{
    //::run();

    hyper::matrix4x4 matrix =
    {
        0.3f, 0.7f, 0.2f, 0.0f,
        2.1f, 0.3f, 0.5f, 0.0f,
        0.9f, 1.4f, 1.1f, 0.0f,
        11.2f, -32.4f, 2.42f, 1.0f
    };

    return 0;
}
