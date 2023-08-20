#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/drawing.hpp>

#include <hyperlinked/tests.hpp>

namespace hyper
{
    void tests::init()
    {
        //hook::jump(0x007474FC, &sub_007474D0);

#ifdef 0
        // Silverton Barriers
        hook::set<std::uint8_t>(0x0061817C, 0xEB);
#endif

        // HotPosition Load Blocking
        // hook::set<std::uint8_t>(0x007D5246, 0x00);

        int breaker = 0;
    }
}
