#include <hyperlib/renderer/view.hpp>
#include <hyperlinked/patches/renderer/view.hpp>

namespace hyper
{
    __declspec(naked) void detour_e_view_plat_info_calculate_view_matrices()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // [esp + 0x08] is 'near_clip'
            // [esp + 0x0C] is 'far_clip'
            // [esp + 0x10] is 'unused'
            // [esp + 0x14] is 'calculate_frustum'
            // ecx contains pointer to view::platform_info

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'calculate_frustum' is now at [esp + 0x18]
            push ebx; // 'calculate_frustum' is now at [esp + 0x1C]
            push ecx; // 'calculate_frustum' is now at [esp + 0x20]
            push edx; // 'calculate_frustum' is now at [esp + 0x24]
            push esi; // 'calculate_frustum' is now at [esp + 0x28]
            push edi; // 'calculate_frustum' is now at [esp + 0x2C]

            push [esp + 0x2C]; // repush 'calculate_frustum'
            push [esp + 0x28]; // repush 'far_clip'
            push [esp + 0x28]; // repush 'near_clip'

            mov ecx, [esp + 0x28]; // move view::instance into ecx

            call view::instance::calculate_view_matrices; // call custom calculate_view_matrices

            // no need to restore esp since 'calculate_view_matrices' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 20; // return immediately to caller function, not back to eViewPlatInfo::CalculateViewMatricies; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_view_setup_world_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to view::instance

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call view::instance::setup_world_light_context; // call custom setup_world_light_context

            // no need to restore esp since 'setup_world_light_context' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eView::SetupWorldLightContext; note that this is a __thiscall
        }
    }

    void view_patches::init()
    {
        // eViewPlatInfo::CalculateViewMatricies
        hook::jump(0x0071B780, &detour_e_view_plat_info_calculate_view_matrices);

        // eView::SetupWorldLightContext
        hook::jump(0x00559DE0, &detour_e_view_setup_world_light_context);
    }
}
