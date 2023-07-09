#include <iostream>

#include <hyperlib/shared.hpp>

#include <hyperlib/bitset.hpp>

#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/memory/memory.hpp>

#include <hyperlib/renderer/culling.hpp>

enum class test : std::uint32_t
{
    first = 1,
    second = 2,
    third = 3,
};


CREATE_ENUM_EXPR_OPERATORS(test);

int main()
{
    test t = test::first + test::second;

    hyper::bitset<static_cast<size_t>(hyper::view_id::count)> bitset(
    {
        static_cast<size_t>(hyper::view_id::player1),
        static_cast<size_t>(hyper::view_id::player2),
        static_cast<size_t>(hyper::view_id::player1_rvm),
        static_cast<size_t>(hyper::view_id::player1_reflection),
        static_cast<size_t>(hyper::view_id::player2_reflection),
    });

    bitset.set(hyper::view_id::env_z_pos, true);
    bitset.set(hyper::view_id::env_x_pos, true);
    bitset.set(hyper::view_id::env_z_neg, true);
    bitset.set(hyper::view_id::env_x_neg, true);
    bitset.set(hyper::view_id::env_y_pos, true);
    bitset.set(hyper::view_id::env_y_neg, true);
    bitset.set(hyper::view_id::player1_rvm, false);

    for (hyper::view_id i = hyper::view_id::flailer; i < hyper::view_id::count; i = static_cast<hyper::view_id>(static_cast<size_t>(i) + 1))
    {
        ::printf("%d ", bitset.get(i));
    }

    return 0;
}
