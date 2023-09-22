#include <hyperlib/renderer/targets.hpp>
#include <hyperlinked/patches/renderer/targets.hpp>

namespace hyper
{
    __declspec(naked) void detour_e_render_view_update()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // ecx contains pointer to render_view

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'view'

            call render_view::update; // call custom update

            // no need to restore esp since 'update' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to eRenderView::Update; note that this is a __thiscall
        }
    }

    void target_patches::init()
    {
        // eRenderView::Update
        hook::jump(0x007498F0, &detour_e_render_view_update);
    }
}
