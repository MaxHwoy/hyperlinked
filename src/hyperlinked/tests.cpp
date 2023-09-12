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
    void print_number_of_rendering_models()
    {
        bool print = false;
        bool clear = false;

        if (print)
        {
            bench::print();
        }

        if (clear)
        {
            bench::reset();
        }

        std::uint32_t count = renderer::rendering_model_count;

        int f = 0;
    }

    __declspec(naked) void test_render_world_ingame()
    {
        __asm
        {
            pushad;

            call print_number_of_rendering_models;

            popad;

            push ebp;
            xor ebp, ebp;
            cmp eax, ebp;

            push 0x0072723D;
            retn;
        }
    }

    void tests::init()
    {
        // always raining
        // hook::set<std::uint32_t>(0x00B74D20, 1u);

        // Silverton Barriers
        hook::set<std::uint8_t>(0x0061817C, 0xEB);

        // HotPosition Load Blocking
        // hook::set<std::uint8_t>(0x007D5246, 0x00);

        // RenderWorldInGame
        hook::jump(0x00727238, &test_render_world_ingame);

        // ScenerySectionHeader::DrawAScenery
        // hook::nop(0x0079FA88, 38);

        int breaker = 0;
    }
}
