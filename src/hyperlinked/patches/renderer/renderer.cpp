#include <hyperlib/renderer/renderer.hpp>
#include <hyperlinked/patches/renderer/renderer.hpp>

namespace hyper
{
    __declspec(naked) void detour_reset_renderer()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call renderer::reset; // call custom reset

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to ResetRenderer
        }
    }

    void renderer_patches::init()
    {
        // ResetRenderer
        hook::jump(0x0072B370, &detour_reset_renderer);
    }
}
