#include <hyperlib/hook.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlinked/patches/renderer/culling.hpp>

namespace hyper
{
    __declspec(naked) void detour_setup_world_culling()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'grand_scenery'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'grand_scenery' is now at [esp + 0x08]
            push ebx; // 'grand_scenery' is now at [esp + 0x0C]
            push ecx; // 'grand_scenery' is now at [esp + 0x10]
            push edx; // 'grand_scenery' is now at [esp + 0x14]
            push esi; // 'grand_scenery' is now at [esp + 0x18]
            push edi; // 'grand_scenery' is now at [esp + 0x1C]

            mov ecx, [esp + 0x1C]; // make 'grand_scenery' as 'this' pointer

            call grand_scenery_cull_info::setup_world_culling; // call custom 'setup_world_culling'

            // no need to restore esp since 'malloc' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SetupWorldCulling
        }
    }

    __declspec(naked) void detour_grand_scenery_cull_info_stuff_scenery()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // [esp + 0x08] is 'pass_flags'
            // ecx contains pointer to grand_scenery_cull_info

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pass_flags' is now at [esp + 0x0C]
            push ebx; // 'pass_flags' is now at [esp + 0x10]
            push ecx; // 'pass_flags' is now at [esp + 0x14]
            push edx; // 'pass_flags' is now at [esp + 0x18]
            push esi; // 'pass_flags' is now at [esp + 0x1C]
            push edi; // 'pass_flags' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'pass_flags'
            push [esp + 0x20]; // repush 'view'

            call grand_scenery_cull_info::stuff_scenery; // call custom stuff_scenery

            // no need to restore esp since 'stuff_scenery' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to GrandSceneryCullInfo::StuffScenery; note that this is a __thiscall
        }
    }

    void culling_patches::init()
    {
        // SetupWorldCulling
        hook::jump(0x007103C0, &detour_setup_world_culling);

        // GrandSceneryCullInfo::StuffScenery
        hook::jump(0x0079AFA0, &detour_grand_scenery_cull_info_stuff_scenery);
    }
}
