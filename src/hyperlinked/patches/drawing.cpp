#include <hyperlib/hook.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/drawing.hpp>
#include <hyperlinked/patches/drawing.hpp>

namespace hyper
{
    __declspec(naked) void detour_render_light_flare()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // [esp + 0x08] is 'flare'
            // [esp + 0x0C] is 'local_world'
            // [esp + 0x10] is 'intensity_scale'
            // [esp + 0x14] is 'refl_type'
            // [esp + 0x18] is 'render_type'
            // [esp + 0x1C] is 'horizontal_flare_scale'
            // [esp + 0x20] is 'reflection_override'
            // [esp + 0x24] is 'color_override'
            // [esp + 0x28] is 'size_scale'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'size_scale' is now at [esp + 0x2C]
            push ebx; // 'size_scale' is now at [esp + 0x30]
            push ecx; // 'size_scale' is now at [esp + 0x34]
            push edx; // 'size_scale' is now at [esp + 0x38]
            push esi; // 'size_scale' is now at [esp + 0x3C]
            push edi; // 'size_scale' is now at [esp + 0x40]

            push [esp + 0x40]; // repush 'size_scale'
            push [esp + 0x40]; // repush 'color_override'
            push [esp + 0x40]; // repush 'reflection_override'
            push [esp + 0x40]; // repush 'horizontal_flare_scale'
            push [esp + 0x40]; // repush 'render_type'
            push [esp + 0x40]; // repush 'refl_type'
            push [esp + 0x40]; // repush 'intensity_scale'
            push [esp + 0x40]; // repush 'local_world'
            push [esp + 0x40]; // repush 'flare'
            push [esp + 0x40]; // repush 'view'

            call renderer::render_light_flare; // call custom render_light_flare

            add esp, 0x28; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eRenderLightFlare
        }
    }

    __declspec(naked) void detour_render_light_flare_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'view'

            call renderer::render_light_flare_pool; // call custom render_light_flare_pool

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eRenderLightFlarePool
        }
    }

    __declspec(naked) void detour_init_light_flare_pool()
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

            call renderer::init_light_flare_pool; // call custom init_light_flare_pool

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eInitLightFlarePool
        }
    }

    __declspec(naked) void detour_reset_light_flare_pool()
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

            call renderer::reset_light_flare_pool; // call custom reset_light_flare_pool

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eResetLightFlarePool
        }
    }

    __declspec(naked) void detour_streak_manager_initialize()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streak::manager

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streak::manager::initialize; // call custom initialize

            // no need to restore esp since 'initialize' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to StreakManager::Initialize; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_streak_manager_destroy()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streak::manager

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streak::manager::destroy; // call custom destroy

            // no need to restore esp since 'destroy' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to StreakManager::Destroy; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_streak_manager_lock()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // ecx contains pointer to streak::manager

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            call streak::manager::lock; // call custom lock

            // no need to restore esp since 'lock' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to StreakManager::Lock; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_streak_manager_unlock()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // ecx contains pointer to streak::manager

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            call streak::manager::unlock; // call custom unlock

            // no need to restore esp since 'unlock' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to StreakManager::Unlock; note that this is a __thiscall
        }
    }

    void drawing_patches::init()
    {
        // eRenderLightFlare
        hook::jump(0x0074D330, &detour_render_light_flare);

        // eRenderLightFlarePool
        hook::jump(0x007507D0, &detour_render_light_flare_pool);

        // eInitLightFlarePool
        hook::jump(0x0073A170, &detour_init_light_flare_pool);

        // eResetLightFlarePool
        hook::jump(0x0073A1F0, &detour_reset_light_flare_pool);

        // StreakManager::Initialize
        hook::jump(0x00749C10, &detour_streak_manager_initialize);

        // StreakManager::Destroy
        hook::jump(0x00749C80, &detour_streak_manager_destroy);

        // StreakManager::Lock
        hook::jump(0x00749CB0, &detour_streak_manager_lock);

        // StreakManager::Unlock
        hook::jump(0x00749E50, &detour_streak_manager_unlock);
    }
}
