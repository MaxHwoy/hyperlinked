#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/streak.hpp>

#include <hyperlinked/tests.hpp>

namespace hyper
{
    void tests::init()
    {
        // always raining
        // hook::set<std::uint32_t>(0x00B74D20, 1u);

        // Silverton Barriers
        hook::set<std::uint8_t>(0x0061817C, 0xEB);

        int breaker = 0;
    }
}
