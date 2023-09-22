#include <hyperlib/renderer/flare_pool.hpp>
#include <hyperlinked/patches/renderer/flare_pool.hpp>

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

            call flare_pool::render_flare; // call custom render_flare

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

            call flare_pool::render; // call custom render

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

            call flare_pool::init; // call custom init

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

            call flare_pool::reset; // call custom reset

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eResetLightFlarePool
        }
    }

    __declspec(naked) void detour_flare_pool_ctor()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to flare_pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            push ecx; // push pointer to flare_pool

            call flare_pool::ctor; // call custom ctor

            add esp, 0x04; // since we pushed pointer to flare_pool

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to FlarePool::FlarePool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_flare_pool_dtor()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to flare_pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            push ecx; // push pointer to flare_pool

            call flare_pool::dtor; // call custom dtor

            add esp, 0x04; // since we pushed pointer to flare_pool

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to FlarePool::~FlarePool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_flare_pool_lock()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // ecx contains pointer to flare_pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            call flare_pool::lock; // call custom lock

            // no need to restore esp since 'lock' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to FlarePool::Lock; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_flare_pool_unlock()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'view'
            // ecx contains pointer to flare_pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'view' is now at [esp + 0x08]
            push ebx; // 'view' is now at [esp + 0x0C]
            push ecx; // 'view' is now at [esp + 0x10]
            push edx; // 'view' is now at [esp + 0x14]
            push esi; // 'view' is now at [esp + 0x18]
            push edi; // 'view' is now at [esp + 0x1C]

            call flare_pool::unlock; // call custom unlock

            // no need to restore esp since 'unlock' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to FlarePool::Unlock; note that this is a __thiscall
        }
    }

    void flare_pool_patches::init()
    {
        // eRenderLightFlare
        hook::jump(0x0074D330, &detour_render_light_flare);

        // eRenderLightFlarePool
        hook::jump(0x007507D0, &detour_render_light_flare_pool);

        // eInitLightFlarePool
        hook::jump(0x0073A170, &detour_init_light_flare_pool);

        // eResetLightFlarePool
        hook::jump(0x0073A1F0, &detour_reset_light_flare_pool);

        // FlarePool::FlarePool
        hook::jump(0x00749C10, &detour_flare_pool_ctor);

        // FlarePool::~FlarePool
        hook::jump(0x00749C80, &detour_flare_pool_dtor);

        // FlarePool::Lock
        hook::jump(0x00749CB0, &detour_flare_pool_lock);

        // FlarePool::Unlock
        hook::jump(0x00749E50, &detour_flare_pool_unlock);
    }
}
