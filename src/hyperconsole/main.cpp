#include <iostream>

#include <hyperlib/shared.hpp>

int main()
{
    std::uint32_t table[0x04];

    hyper::bit_table bits = hyper::bit_table::create<std::uint32_t>(table);

    for (size_t i = 0u; i < bits.size(); ++i)
    {
        bits.set(i, hyper::math::is_pow_2(i));
    }

    for (size_t i = 0u; i < bits.size(); ++i)
    {
        ::printf("%d -> %d\n", i, static_cast<std::uint32_t>(bits.get(i)));
    }

    hyper::span<std::uint32_t> sp(table);

    sp[0] = 777;
    sp[5] = 666;

    return 0;
}
