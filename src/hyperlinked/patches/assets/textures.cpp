#include <hyperlib/assets/textures.hpp>
#include <hyperlinked/patches/assets/textures.hpp>

namespace hyper
{
    __declspec(naked) void detour_render_state_init()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'texture'
            // ecx contains pointer to texture::render_state

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'texture' is now at [esp + 0x08]
            push ebx; // 'texture' is now at [esp + 0x0C]
            push ecx; // 'texture' is now at [esp + 0x10]
            push edx; // 'texture' is now at [esp + 0x14]
            push esi; // 'texture' is now at [esp + 0x18]
            push edi; // 'texture' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'texture'

            call texture::render_state::initialize; // call custom initialize

            // no need to restore esp since 'initialize' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to RenderState::Init; note that this is a __thiscall
        }
    }

    void texture_patches::init()
    {
        // RenderState::Init
        hook::jump(0x0073B9E0, &detour_render_state_init);
    }
}
