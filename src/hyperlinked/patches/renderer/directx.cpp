#include <hyperlib/renderer/directx.hpp>
#include <hyperlinked/patches/renderer/directx.hpp>

namespace hyper
{
    __declspec(naked) void detour_create_d3d_present_parameters()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'res_x'
            // [esp + 0x08] is 'res_y'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'res_y' is now at [esp + 0x0C]
            push ebx; // 'res_y' is now at [esp + 0x10]
            push ecx; // 'res_y' is now at [esp + 0x14]
            push edx; // 'res_y' is now at [esp + 0x18]
            push esi; // 'res_y' is now at [esp + 0x1C]
            push edi; // 'res_y' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'res_y'
            push [esp + 0x20]; // repush 'res_x'

            call directx::create_d3d_present_params; // call custom create_d3d_present_params

            add esp, 0x08; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to CreateD3DPresentParameters
        }
    }

    __declspec(naked) void detour_reset_all_rendering_states()
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

            call directx::reset_rendering_states; // call custom reset_rendering_states

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to ResetAllRenderingStates
        }
    }

    void directx_patches::init()
    {
        // CreateD3DPresentParameters
        hook::jump(0x00710100, &detour_create_d3d_present_parameters);

        // ResetAllRenderingStates
        hook::jump(0x0071BED0, &detour_reset_all_rendering_states);
    }
}
