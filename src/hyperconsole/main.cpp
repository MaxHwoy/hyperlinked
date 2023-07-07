#include <iostream>

#include <hyperlib/shared.hpp>

#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/memory/memory.hpp>

#include <hyperlib/renderer/culling.hpp>

template <typename T> auto null_ref() -> T&
{
    return *reinterpret_cast<T*>(nullptr);
}

template <typename T> bool is_null_ref(T& ref)
{
    return &ref == nullptr;
}

int main()
{
    hyper::vector2 point(1.0f, -1.0f);
    hyper::vector2 p1(2.0f, 0.0f);
    hyper::vector2 p2(2.0f, 2.0f);

    float distance = hyper::math::distance_to_line(point, p1, p2);

    return 0;
}
