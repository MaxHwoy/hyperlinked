#include <hyperlib/shared.hpp>
#include <hyperlinked/tests.hpp>
#include <hyperlib/renderer/screen_effect.hpp>

namespace hyper
{
    void tests::init()
    {
        // Always Raining
        hook::set<std::uint32_t>(0x00B74D20, 1u);

        // Silverton Barriers
        hook::set<std::uint8_t>(0x0061817C, 0xEB);

        // HotPosition Load Blocking
        // hook::set<std::uint8_t>(0x007D5246, 0x00);
    }
}
