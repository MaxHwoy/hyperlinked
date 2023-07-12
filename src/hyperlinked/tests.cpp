#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/streak.hpp>

#include <hyperlinked/tests.hpp>

namespace hyper
{
    void test_render_state_call(texture::render_state* state, texture::info* texture)
    {
        state->initialize(texture);
    }

    void test_setup_world_culling_call(grand_scenery_cull_info* grand)
    {
        grand->setup_world_culling();
    }

    void test_render_car_flares(
        view::instance* view,
        flare::instance* flare,
        const matrix4x4* local_world,
        float intensity_scale,
        flare::reflection refl_type,
        flare::render render_type,
        float horizontal_flare_scale,
        float reflection_override,
        std::uint32_t color_override,
        float size_scale)
    {
        renderer::render_light_flare(view, *flare, local_world, intensity_scale, refl_type, render_type, horizontal_flare_scale, reflection_override, *reinterpret_cast<color32*>(&color_override), size_scale);
    }

    __declspec(naked) void test_render_state()
    {
        __asm
        {
            // 'texture' is at esi
            // 'render_state' is at 'ecx'

            pushad;

            push esi;
            push ecx;
            
            call test_render_state_call;

            add esp, 0x08;

            popad;

            mov dword ptr ds : [esp], 0x00725C9A;
            retn; 
        }
    }

    __declspec(naked) void test_setup_world_culling()
    {
        __asm
        {
            push eax;
            push ebx;
            push ecx;
            push edx;
            push esi;
            push edi;

            push [esp + 0x1C];

            call test_setup_world_culling_call;

            add esp, 0x04;

            pop edi;
            pop esi;
            pop edx;
            pop ecx;
            pop ebx;
            pop eax;

            retn;
        }
    }

    __declspec(naked) void test_render_light_flare_pool()
    {
        __asm
        {
            push eax;
            push ebx;
            push ecx;
            push edx;
            push esi;
            push edi;

            push [esp + 0x1C];

            call renderer::render_light_flare_pool;

            add esp, 0x04;

            pop edi;
            pop esi;
            pop edx;
            pop ecx;
            pop ebx;
            pop eax;

            retn;
        }
    }

    __declspec(naked) void test_car_render_flares()
    {
        __asm
        {
            // size_scale at esp + 0x24

            push eax;
            push ebx;
            push ecx;
            push edx;
            push esi;
            push edi;

            push [esp + 0x3C]; // size_scale
            push [esp + 0x3C]; // color_override
            push [esp + 0x3C]; // reflection_override
            push [esp + 0x3C]; // horizontal_flare_scale,
            push [esp + 0x3C]; // render_type
            push [esp + 0x3C]; // refl_type
            push [esp + 0x3C]; // intensity_scale
            push [esp + 0x3C]; // local_world
            push [esp + 0x3C]; // flare
            push [esp + 0x3C]; // view

            call test_render_car_flares;

            add esp, 0x28;

            pop edi;
            pop esi;
            pop edx;
            pop ecx;
            pop ebx;
            pop eax;

            push 0x007CC646;
            retn;
        }
    }

    __declspec(naked) void test_init_light_flare_pool()
    {
        __asm
        {
            pushad;

            call renderer::init_light_flare_pool;

            popad;

            push 0x0073A38A;
            retn;
        }
    }

    __declspec(naked) void test_reset_light_flare_pool()
    {
        __asm
        {
            pushad;

            call renderer::reset_light_flare_pool;

            popad;

            push 0x0073A3BF;
            retn;
        }
    }

    __declspec(naked) void test_init_streak_manager()
    {
        __asm
        {
            or esp, 0;
            pushad;

            call streak::manager::initialize;

            popad;

            retn;
        }
    }

    void tests::init()
    {
        // always raining
        // hook::set<std::uint32_t>(0x00B74D20, 1u);

        hook::jump(0x00725C95, &test_render_state);

        hook::jump(0x007103C0, &test_setup_world_culling);

        hook::jump(0x007507D0, &test_render_light_flare_pool);

        hook::jump(0x007CC641, &test_car_render_flares);

        hook::jump(0x0073A385, &test_init_light_flare_pool);

        hook::jump(0x0073A3BA, &test_reset_light_flare_pool);

        hook::jump(0x00749C10, &test_init_streak_manager);
    }
}
