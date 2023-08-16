#include <hyperlib/streamer/sections.hpp>
#include <hyperlinked/patches/sections.hpp>

namespace hyper
{
    __declspec(naked) void detour_visible_section_manager_allocate_user_info()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section_number'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'section_number' is now at [esp + 0x08]
            push ecx; // 'section_number' is now at [esp + 0x0C]
            push edx; // 'section_number' is now at [esp + 0x10]
            push esi; // 'section_number' is now at [esp + 0x14]
            push edi; // 'section_number' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'section_number'

            call visible_section::manager::allocate_user_info; // call custom allocate_user_info

            // no need to restore esp since 'allocate_user_info' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::AllocateUserInfo; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_unallocate_user_info()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section_number'
            // ecx contains pointer to visible_section::manager

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section_number' is now at [esp + 0x08]
            push ebx; // 'section_number' is now at [esp + 0x0C]
            push ecx; // 'section_number' is now at [esp + 0x10]
            push edx; // 'section_number' is now at [esp + 0x14]
            push esi; // 'section_number' is now at [esp + 0x18]
            push edi; // 'section_number' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section_number'

            call visible_section::manager::unallocate_user_info; // call custom unallocate_user_info

            // no need to restore esp since 'unallocate_user_info' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::AllocateUserInfo; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_enable_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'key'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'key' is now at [esp + 0x08]
            push ecx; // 'key' is now at [esp + 0x0C]
            push edx; // 'key' is now at [esp + 0x10]
            push esi; // 'key' is now at [esp + 0x14]
            push edi; // 'key' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'key'

            call visible_section::manager::enable_group; // call custom enable_group

            // no need to restore esp since 'enable_group' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::EnableGroup; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_disable_group()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'key'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'key' is now at [esp + 0x08]
            push ecx; // 'key' is now at [esp + 0x0C]
            push edx; // 'key' is now at [esp + 0x10]
            push esi; // 'key' is now at [esp + 0x14]
            push edi; // 'key' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'key'

            call visible_section::manager::disable_group; // call custom disable_group

            // no need to restore esp since 'disable_group' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::DisableGroup; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_loader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call visible_section::manager::loader; // call custom loader

            // no need to restore esp since 'loader' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::Loader; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_unloader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call visible_section::manager::unloader; // call custom unloader

            // no need to restore esp since 'unloader' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::Unloader; note that this is a __thiscall
        }
    }

    void section_patches::init()
    {
        // VisibleSectionManager::AllocateUserInfo
        hook::jump(0x007A0270, &detour_visible_section_manager_allocate_user_info);

        // VisibleSectionManager::UnallocateUserInfo
        hook::jump(0x007A02C0, &detour_visible_section_manager_unallocate_user_info);

        // VisibleSectionManager::EnableGroup
        hook::jump(0x0079BFB0, &detour_visible_section_manager_enable_group);

        // VisibleSectionManager::DisableGroup
        hook::jump(0x0079BFE0, &detour_visible_section_manager_disable_group);

        // VisibleSectionManager::Loader
        hook::jump(0x007A8880, &detour_visible_section_manager_loader);

        // VisibleSectionManager::Unloader
        hook::jump(0x007A0310, &detour_visible_section_manager_unloader);
    }
}
