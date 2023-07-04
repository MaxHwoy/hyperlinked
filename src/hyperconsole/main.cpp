#include <iostream>

#include <hyperlib/shared.hpp>

#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/memory/memory.hpp>

enum class sample
{
    a = 1,
    b = 2,
};

int main()
{
    ::printf("0x%08X\n", hyper::hashing::bin("MaxHwoy"));

    return 0;
}
