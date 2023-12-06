#include <hyperlib/renderer/renderer.hpp>
#include <hyperlinked/patches/renderer/renderer.hpp>

namespace hyper
{
    __declspec(naked) void detour_render_plat_frame_render()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call renderer::render; // call custom render

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to RenderPlat::Frame::Render
        }
    }

    void renderer_patches::init()
    {
        // RenderPlat::Frame::Render
        hook::jump(0x007311B0, &detour_render_plat_frame_render);
    }
}
