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

    void culling_patches::init()
    {
        // SetupWorldCulling
        hook::jump(0x007103C0, &detour_setup_world_culling);
    }
}
