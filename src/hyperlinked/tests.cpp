#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>

#include <hyperlinked/tests.hpp>

namespace hyper
{
    void test_render_state_call(render_state* state, texture_info* texture)
    {
        state->initialize(texture);
    }

    void test_setup_world_culling_call(grand_scenery_cull_info* grand)
    {
        grand->setup_world_culling();
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

            push edx;

            call test_setup_world_culling_call;

            add esp, 0x04;

            pop edi;
            pop esi;
            pop edx;
            pop ecx;
            pop ebx;
            pop eax;

            push 0x0072ECAF;
            retn;
        }
    }

    void tests::init()
    {
        hook::jump(0x00725C95, &test_render_state);

        hook::jump(0x0072ECAA, &test_setup_world_culling);
    }
}
