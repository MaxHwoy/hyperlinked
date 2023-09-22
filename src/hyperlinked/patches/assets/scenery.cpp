#include <hyperlib/assets/scenery.hpp>
#include <hyperlinked/patches/assets/scenery.hpp>

namespace hyper
{
    __declspec(naked) void detour_scenery_override_info_set_exclude_flags()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'mask'
            // [esp + 0x08] is 'flag'
            // ecx contains pointer to scenery::override_info

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'flag' is now at [esp + 0x0C]
            push ebx; // 'flag' is now at [esp + 0x10]
            push ecx; // 'flag' is now at [esp + 0x14]
            push edx; // 'flag' is now at [esp + 0x18]
            push esi; // 'flag' is now at [esp + 0x1C]
            push edi; // 'flag' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'flag'
            push [esp + 0x20]; // repush 'mask'

            call scenery::override_info::set_exclude_flags; // call custom set_exclude_flags

            // no need to restore esp since 'set_exclude_flags' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to SceneryOverrideInfo::SetExcludeFlags; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_scenery_override_info_assign_overrides()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pack'
            // ecx contains pointer to scenery::override_info

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pack' is now at [esp + 0x08]
            push ebx; // 'pack' is now at [esp + 0x0C]
            push ecx; // 'pack' is now at [esp + 0x10]
            push edx; // 'pack' is now at [esp + 0x14]
            push esi; // 'pack' is now at [esp + 0x18]
            push edi; // 'pack' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'pack'

            call scenery::override_info::assign_overrides; // call custom assign_overrides

            // no need to restore esp since 'assign_overrides' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to SceneryOverrideInfo::AssignOverrides; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_scenery_group_enable_rendering()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'flip_artwork'
            // ecx contains pointer to scenery::group

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'flip_artwork' is now at [esp + 0x08]
            push ebx; // 'flip_artwork' is now at [esp + 0x0C]
            push ecx; // 'flip_artwork' is now at [esp + 0x10]
            push edx; // 'flip_artwork' is now at [esp + 0x14]
            push esi; // 'flip_artwork' is now at [esp + 0x18]
            push edi; // 'flip_artwork' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'flip_artwork'

            call scenery::group::enable_rendering; // call custom enable_rendering

            // no need to restore esp since 'enable_rendering' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to SceneryGroup::EnableRendering; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_scenery_group_disable_rendering()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to scenery::group

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call scenery::group::disable_rendering; // call custom disable_rendering

            // no need to restore esp since 'disable_rendering' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SceneryGroup::DisableRendering; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_enable_scenery_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'key'
            // [esp + 0x08] is 'flip_artwork'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'flip_artwork' is now at [esp + 0x0C]
            push ebx; // 'flip_artwork' is now at [esp + 0x10]
            push ecx; // 'flip_artwork' is now at [esp + 0x14]
            push edx; // 'flip_artwork' is now at [esp + 0x18]
            push esi; // 'flip_artwork' is now at [esp + 0x1C]
            push edi; // 'flip_artwork' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'flip_artwork'
            push [esp + 0x20]; // repush 'key'

            call scenery::group::enable; // call custom enable

            add esp, 0x08; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to EnableSceneryGroup
        }
    }

    __declspec(naked) void detour_disable_scenery_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'key'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'key' is now at [esp + 0x08]
            push ebx; // 'key' is now at [esp + 0x0C]
            push ecx; // 'key' is now at [esp + 0x10]
            push edx; // 'key' is now at [esp + 0x14]
            push esi; // 'key' is now at [esp + 0x18]
            push edi; // 'key' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'key'

            call scenery::group::disable; // call custom disable

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to DisableSceneryGroup
        }
    }

    __declspec(naked) void detour_find_scenery_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'key'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'key' is now at [esp + 0x08]
            push ecx; // 'key' is now at [esp + 0x0C]
            push edx; // 'key' is now at [esp + 0x10]
            push esi; // 'key' is now at [esp + 0x14]
            push edi; // 'key' is now at [esp + 0x18]
            
            push [esp + 0x18]; // repush 'key'
            
            call scenery::group::find; // call custom find
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to FindSceneryGroup
        }
    }

    __declspec(naked) void detour_loader_scenery_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call scenery::group::loader; // call custom loader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to LoaderSceneryGroup
        }
    }

    __declspec(naked) void detour_unloader_scenery_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call scenery::group::unloader; // call custom unloader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to UnloaderSceneryGroup
        }
    }

    __declspec(naked) void detour_loader_scenery()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call scenery::loader; // call custom loader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to LoaderScenery
        }
    }

    __declspec(naked) void detour_unloader_scenery()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call scenery::unloader; // call custom unloader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to UnloaderScenery
        }
    }

    void scenery_patches::init()
    {
        // SceneryOverrideInfo::SetExcludeFlags
        hook::jump(0x0079F250, &detour_scenery_override_info_set_exclude_flags);

        // SceneryOverrideInfo::AssignOverrides
        hook::jump(0x0079F150, &detour_scenery_override_info_assign_overrides);

        // SceneryGroup::EnableRendering
        hook::jump(0x007A2D50, &detour_scenery_group_enable_rendering);

        // SceneryGroup::DisableRendering
        hook::jump(0x007A2DD0, &detour_scenery_group_disable_rendering);

        // EnableSceneryGroup
        hook::jump(0x007A2E30, &detour_enable_scenery_group);

        // DisableSceneryGroup
        hook::jump(0x007A2E90, &detour_disable_scenery_group);

        // FindSceneryGroup
        hook::jump(0x0079F610, &detour_find_scenery_group);

        // LoaderSceneryGroup
        hook::jump(0x0079F540, &detour_loader_scenery_group);

        // UnloaderSceneryGroup
        hook::jump(0x0079F5A0, &detour_unloader_scenery_group);

        // LoaderScenery
        hook::jump(0x007A73F0, &detour_loader_scenery);

        // UnloaderScenery
        hook::jump(0x007A6050, &detour_unloader_scenery);
    }
}
