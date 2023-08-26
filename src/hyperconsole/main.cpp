#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/utils/utils.hpp>

#include <hyperlib/bench.hpp>

void test()
{
    BENCHMARK();

    for (std::uint32_t i = 0u; i <= std::numeric_limits<std::uint16_t>::max(); ++i)
    {
        std::uint16_t lhs = static_cast<std::uint16_t>(0x4000u) - i;
        std::uint16_t rhs = static_cast<std::uint16_t>(0x4000u - i);

        assert(lhs == rhs);
    }
}

int main()
{
    for (std::uint32_t i = 0u; i < 1000u; ++i)
    {
        ::test();
    }

    hyper::bench::print();

    return 0;
}
