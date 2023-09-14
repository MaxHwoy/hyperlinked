#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/lighting.hpp>
#include <hyperlib/renderer/streak.hpp>

#include <hyperlib/bench.hpp>
#include <hyperlinked/tests.hpp>

namespace hyper
{
    void tests::init()
    {
        // always raining
        // hook::set<std::uint32_t>(0x00B74D20, 1u);

        // Silverton Barriers
        hook::set<std::uint8_t>(0x0061817C, 0xEB);

        // HotPosition Load Blocking
        // hook::set<std::uint8_t>(0x007D5246, 0x00);

        int breaker = 0;
    }
}
