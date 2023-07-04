#include <hyperlib/shared.hpp>

#include <hyperlib/assets/textures.hpp>
#include <hyperlib/renderer/directx.hpp>

#include <hyperlinked/tests.hpp>

namespace hyper
{
    void test_render_state_call(render_state* state, texture_info* texture)
    {
        state->initialize(texture);
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

    void tests::init()
    {
        hook::jump(0x00725C95, &test_render_state);
    }
}
