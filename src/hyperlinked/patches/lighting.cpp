#include <hyperlib/renderer/lighting.hpp>
#include <hyperlinked/patches/lighting.hpp>

namespace hyper
{
    __declspec(naked) void detour_el_setup_lights()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'context'
            // [esp + 0x08] is 'shaper'
            // [esp + 0x0C] is 'inst_pos'
            // [esp + 0x10] is 'inst_trs'
            // [esp + 0x14] is 'camera_trs'
            // [esp + 0x18] is 'base'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'base' is now at [esp + 0x1C]
            push ebx; // 'base' is now at [esp + 0x20]
            push ecx; // 'base' is now at [esp + 0x24]
            push edx; // 'base' is now at [esp + 0x28]
            push esi; // 'base' is now at [esp + 0x2C]
            push edi; // 'base' is now at [esp + 0x30]

            push [esp + 0x30]; // repush 'base'
            push [esp + 0x30]; // repush 'camera_trs'
            push [esp + 0x30]; // repush 'inst_trs'
            push [esp + 0x30]; // repush 'inst_pos'
            push [esp + 0x30]; // repush 'shaper'
            push [esp + 0x30]; // repush 'context'

            call lighting::setup_lights; // call custom setup_lights

            add esp, 0x18; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to elSetupLights
        }
    }

    __declspec(naked) void detour_el_setup_envmap()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'context'
            // [esp + 0x08] is 'inst_trs'
            // [esp + 0x0C] is 'camera_trs'
            // [esp + 0x10] is 'center'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'center' is now at [esp + 0x14]
            push ecx; // 'center' is now at [esp + 0x18]
            push edx; // 'center' is now at [esp + 0x1C]
            push esi; // 'center' is now at [esp + 0x20]
            push edi; // 'center' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'center'
            push [esp + 0x24]; // repush 'camera_trs'
            push [esp + 0x24]; // repush 'inst_trs'
            push [esp + 0x24]; // repush 'context'

            call lighting::setup_envmap; // call custom setup_envmap

            add esp, 0x10; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to elSetupEnvMap
        }
    }

    __declspec(naked) void detour_el_reset_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'context'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'context' is now at [esp + 0x08]
            push ebx; // 'context' is now at [esp + 0x0C]
            push ecx; // 'context' is now at [esp + 0x10]
            push edx; // 'context' is now at [esp + 0x14]
            push esi; // 'context' is now at [esp + 0x18]
            push edi; // 'context' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'context'

            call lighting::reset_light_context; // call custom reset_light_context

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to elResetLightContext
        }
    }

    __declspec(naked) void detour_el_setup_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'context'
            // [esp + 0x08] is 'shaper'
            // [esp + 0x0C] is 'inst_trs'
            // [esp + 0x10] is 'camera_trs'
            // [esp + 0x14] is 'center'
            // [esp + 0x18] is 'base'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'base' is now at [esp + 0x1C]
            push ecx; // 'base' is now at [esp + 0x20]
            push edx; // 'base' is now at [esp + 0x24]
            push esi; // 'base' is now at [esp + 0x28]
            push edi; // 'base' is now at [esp + 0x2C]

            push [esp + 0x2C]; // repush 'base'
            push [esp + 0x2C]; // repush 'camera_trs'
            push [esp + 0x2C]; // repush 'inst_trs'
            push [esp + 0x2C]; // repush 'inst_pos'
            push [esp + 0x2C]; // repush 'shaper'
            push [esp + 0x2C]; // repush 'context'

            call lighting::setup_light_context; // call custom setup_light_context

            add esp, 0x18; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to elSetupLightContext
        }
    }

    __declspec(naked) void detour_el_clone_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'dst'
            // [esp + 0x08] is 'inst_trs'
            // [esp + 0x0C] is 'camera_trs'
            // [esp + 0x10] is 'center'
            // [esp + 0x14] is 'base'
            // [esp + 0x18] is 'src'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'src' is now at [esp + 0x1C]
            push ecx; // 'src' is now at [esp + 0x20]
            push edx; // 'src' is now at [esp + 0x24]
            push esi; // 'src' is now at [esp + 0x28]
            push edi; // 'src' is now at [esp + 0x2C]

            push [esp + 0x2C]; // repush 'src'
            push [esp + 0x2C]; // repush 'base'
            push [esp + 0x2C]; // repush 'camera_trs'
            push [esp + 0x2C]; // repush 'inst_trs'
            push [esp + 0x2C]; // repush 'inst_pos'
            push [esp + 0x2C]; // repush 'dst'

            call lighting::clone_light_context; // call custom clone_light_context

            add esp, 0x18; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to elCloneLightContext
        }
    }

    __declspec(naked) void detour_el_rotate_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'dst'
            // [esp + 0x08] is 'src'
            // [esp + 0x0C] is 'inst_trs'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'inst_trs' is now at [esp + 0x10]
            push ebx; // 'inst_trs' is now at [esp + 0x14]
            push ecx; // 'inst_trs' is now at [esp + 0x18]
            push edx; // 'inst_trs' is now at [esp + 0x1C]
            push esi; // 'inst_trs' is now at [esp + 0x20]
            push edi; // 'inst_trs' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'inst_trs'
            push [esp + 0x24]; // repush 'src'
            push [esp + 0x24]; // repush 'dst'

            call lighting::rotate_light_context; // call custom rotate_light_context

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to elRotateLightContext
        }
    }

    __declspec(naked) void detour_el_query_light_database()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'query'
            // [esp + 0x08] is 'position'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'position' is now at [esp + 0x0C]
            push ecx; // 'position' is now at [esp + 0x10]
            push edx; // 'position' is now at [esp + 0x14]
            push esi; // 'position' is now at [esp + 0x18]
            push edi; // 'position' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'position'
            push [esp + 0x1C]; // repush 'query'

            call lighting::query_light_database; // call custom query_light_database

            add esp, 0x08; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to elQueryLightDatabase
        }
    }

    void lighting_patches::init()
    {
        // elSetupLights
        hook::jump(0x00746A00, &detour_el_setup_lights);

        // elSetupEnvMap
        hook::jump(0x007397B0, &detour_el_setup_envmap);

        // elResetLightContext
        hook::jump(0x00739920, &detour_el_reset_light_context);

        // elSetupLightContext
        hook::jump(0x007472D0, &detour_el_setup_light_context);

        // elCloneLightContext
        hook::jump(0x00747340, &detour_el_clone_light_context);

        // elRotateLightContext
        hook::jump(0x00746520, &detour_el_rotate_light_context);

        // elQueryLightDatabase
        hook::jump(0x00746230, &detour_el_query_light_database);
    }
}
