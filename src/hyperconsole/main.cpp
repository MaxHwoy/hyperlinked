#include <iostream>

#include <hyperlib/shared.hpp>

#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/memory/memory.hpp>

#include <hyperlib/renderer/culling.hpp>

enum class test : std::uint32_t
{
    first = 1,
    second = 2,
    third = 3,
};


CREATE_ENUM_FLAG_OPERATORS(test);

int main()
{
    test t = test::first;

    //++t;

    return 0;
}
