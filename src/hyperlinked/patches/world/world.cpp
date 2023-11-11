#include <hyperlib/world/world.hpp>
#include <hyperlinked/patches/world/world.hpp>

namespace hyper
{
    __declspec(naked) void detour_setup_topology_and_scenery()
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

            call world::init_topology_and_scenery_groups; // call custom init_topology_and_scenery_groups

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SetupTopologyAndScenery
        }
    }

    __declspec(naked) void detour_init_topology_and_scenery_groups()
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

            call world::init_topology_and_scenery_groups; // call custom init_topology_and_scenery_groups

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to InitTopologyAndSceneryGroups
        }
    }

    __declspec(naked) void detour_redo_topology_and_scenery_groups()
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

            call world::redo_topology_and_scenery_groups; // call custom redo_topology_and_scenery_groups

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to RedoTopologyAndSceneryGroups
        }
    }

    void world_patches::init()
    {
        // SetupTopologyAndScenery
        hook::jump(0x007990A0, &detour_setup_topology_and_scenery);

        // InitTopologyAndSceneryGroups
        hook::jump(0x006A8A80, &detour_init_topology_and_scenery_groups);

        // RedoTopologyAndSceneryGroups
        hook::jump(0x006A8AD0, &detour_redo_topology_and_scenery_groups);
    }
}
