#include <hyperlib/streamer/sections.hpp>
#include <hyperlinked/patches/sections.hpp>

namespace hyper
{
    __declspec(naked) void detour_visible_section_boundary_get_distance_outside()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'position'
            // [esp + 0x08] is 'extra_width'
            // ecx contains pointer to visible_section::boundary

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'extra_width' is now at [esp + 0x0C]
            push ebx; // 'extra_width' is now at [esp + 0x10]
            push ecx; // 'extra_width' is now at [esp + 0x14]
            push edx; // 'extra_width' is now at [esp + 0x18]
            push esi; // 'extra_width' is now at [esp + 0x1C]
            push edi; // 'extra_width' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'extra_width'
            push [esp + 0x20]; // repush 'position'
            push ecx;          // push 'bound'

            call visible_section::manager::get_distance_outside; // call custom get_distance_outside

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to VisibleSectionBoundary::GetDistanceOutside; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_get_drivable_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'position'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'position' is now at [esp + 0x08]
            push ecx; // 'position' is now at [esp + 0x0C]
            push edx; // 'position' is now at [esp + 0x10]
            push esi; // 'position' is now at [esp + 0x14]
            push edi; // 'position' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'position'

            call visible_section::manager::get_drivable_section; // call custom get_drivable_section

            // no need to restore esp since 'get_drivable_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::GetDrivableSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_get_depth_name()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'position'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'position' is now at [esp + 0x08]
            push ecx; // 'position' is now at [esp + 0x0C]
            push edx; // 'position' is now at [esp + 0x10]
            push esi; // 'position' is now at [esp + 0x14]
            push edi; // 'position' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'position'

            call visible_section::manager::get_depth_name; // call custom get_depth_name

            // no need to restore esp since 'get_depth_name' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::GetDepthName; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_get_sections_to_load()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'loading'
            // [esp + 0x08] is 'sections_to_load'
            // [esp + 0x0C] is 'max_sections'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'max_sections' is now at [esp + 0x10]
            push ecx; // 'max_sections' is now at [esp + 0x14]
            push edx; // 'max_sections' is now at [esp + 0x18]
            push esi; // 'max_sections' is now at [esp + 0x1C]
            push edi; // 'max_sections' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'max_sections'
            push [esp + 0x20]; // repush 'sections_to_load'
            push [esp + 0x20]; // repush 'loading'

            call visible_section::manager::get_sections_to_load; // call custom get_sections_to_load

            // no need to restore esp since 'get_sections_to_load' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 12; // return immediately to caller function, not back to VisibleSectionManager::GetSectionsToLoad; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_find_boundary()
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

            call visible_section::manager::find_boundary; // call custom find_boundary

            // no need to restore esp since 'find_boundary' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::FindBoundary; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_find_drivable_section()
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

            call visible_section::manager::find_drivable_section; // call custom find_drivable_section

            // no need to restore esp since 'find_drivable_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to VisibleSectionManager::FindDrivableSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_visible_section_manager_find_closest_boundary()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'position'
            // [esp + 0x08] is 'distance'
            // ecx contains pointer to visible_section::manager

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'distance' is now at [esp + 0x0C]
            push ecx; // 'distance' is now at [esp + 0x10]
            push edx; // 'distance' is now at [esp + 0x14]
            push esi; // 'distance' is now at [esp + 0x18]
            push edi; // 'distance' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'distance'
            push [esp + 0x1C]; // repush 'position'

            call visible_section::manager::find_closest_boundary; // call custom find_closest_boundary

            // no need to restore esp since 'find_closest_boundary' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 8; // return immediately to caller function, not back to VisibleSectionManager::FindClosestBoundary; note that this is a __thiscall
        }
    }

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

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'key' is now at [esp + 0x08]
            push ebx; // 'key' is now at [esp + 0x0C]
            push ecx; // 'key' is now at [esp + 0x10]
            push edx; // 'key' is now at [esp + 0x14]
            push esi; // 'key' is now at [esp + 0x18]
            push edi; // 'key' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'key'

            call visible_section::manager::enable_group; // call custom enable_group

            // no need to restore esp since 'enable_group' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

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

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'key' is now at [esp + 0x08]
            push ebx; // 'key' is now at [esp + 0x0C]
            push ecx; // 'key' is now at [esp + 0x10]
            push edx; // 'key' is now at [esp + 0x14]
            push esi; // 'key' is now at [esp + 0x18]
            push edi; // 'key' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'key'

            call visible_section::manager::disable_group; // call custom disable_group

            // no need to restore esp since 'disable_group' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

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
        // VisibleSectionBoundary::GetDistanceOutside
        hook::jump(0x0079B970, &detour_visible_section_boundary_get_distance_outside);

        // VisibleSectionManager::GetDrivableSection
        hook::jump(0x007A3290, &detour_visible_section_manager_get_drivable_section);

        // VisibleSectionManager::GetDepthName
        hook::jump(0x007A00F0, &detour_visible_section_manager_get_depth_name);

        // VisibleSectionManager::GetSectionsToLoad
        hook::jump(0x007A2FC0, &detour_visible_section_manager_get_sections_to_load);

        // VisibleSectionManager::FindBoundary
        hook::jump(0x007A0370, &detour_visible_section_manager_find_boundary);

        // VisibleSectionManager::FindDrivableSection
        hook::jump(0x007A0590, &detour_visible_section_manager_find_drivable_section);

        // VisibleSectionManager::FindClosestBoundary
        hook::jump(0x007A03E0, &detour_visible_section_manager_find_closest_boundary);

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

        // BELOW ARE PATCHES THAT REPLACE THE VISIBLE SECTION MANAGER POINTERS

        // CAnimScene::Init
        hook::set(0x0045F3DF, &visible_section::manager::instance);

        // CAnimScene::Purge
        hook::set(0x0045DA09, &visible_section::manager::instance);

        // CSTATEMGR_DriveBy::UpdateParams
        hook::set(0x00557796, &visible_section::manager::instance);

        // EnableBarrierSceneryGroup
        hook::set(0x00699D40, &visible_section::manager::instance);

        // GIcon::FindSection
        hook::set(0x00615F9C, &visible_section::manager::instance);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A4920, &visible_section::manager::instance);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A4942, &visible_section::manager::instance);

        // LoaderEventManager
        hook::set(0x007A1A64, &visible_section::manager::instance);

        // LoaderLightFlares
        hook::set(0x007477A1, &visible_section::manager::instance);

        // LoaderLights
        hook::set(0x00746111, &visible_section::manager::instance);

        // LoaderScenery
        hook::set(0x007A7494, &visible_section::manager::instance);

        // LoaderVisibleSections
        hook::set(0x007A8AB6, &visible_section::manager::instance);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A481A, &visible_section::manager::instance);

        // TrackStreamer::GetPredictedZone
        hook::set(0x007A45CE, &visible_section::manager::instance);

        // TrackStreamer::GetPredictedZone
        hook::set(0x007A4663, &visible_section::manager::instance);

        // TrackStreamer::InitRegion
        hook::set(0x007A28EC, &visible_section::manager::instance);

        // TrackStreamer::SwitchZones
        hook::set(0x007A7F54, &visible_section::manager::instance);

        // TrackStreamer::SwitchZones
        hook::set(0x007A7F5F, &visible_section::manager::instance);

        // UnloaderLightFlares
        hook::set(0x00747825, &visible_section::manager::instance);

        // UnloaderLights
        hook::set(0x007461C4, &visible_section::manager::instance);

        // UnloaderVisibleSections
        hook::set(0x007A2F16, &visible_section::manager::instance);

        // eRenderWorldLightFlares
        hook::set(0x0074E18F, &visible_section::manager::instance);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A479A, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A47A0, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A47B2, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A4831, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A4837, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A4841, &visible_section::manager::instance.drivable_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A4757, &visible_section::manager::instance.loading_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A475D, &visible_section::manager::instance.loading_section_list);

        // TrackStreamer::DetermineStreamingSections
        hook::set(0x007A478B, &visible_section::manager::instance.loading_section_list);

        // CSTATEMGR_DriveBy::UpdateParams
        hook::set(0x005577B3, &visible_section::manager::instance.pack);

        // GIcon::FindSection
        hook::set(0x00615F84, &visible_section::manager::instance.pack);

        // FindSceneryInstance
        hook::set(0x0079A797, &visible_section::manager::instance.user_infos);

        // FindSceneryHeirarchy
        hook::set(0x0079F116, &visible_section::manager::instance.user_infos);

        // GetScenerySectionHeader
        hook::set(0x0079A747, &visible_section::manager::instance.user_infos);

        // GrandSceneryCullInfo::CullView
        hook::set(0x007A4B6C, &visible_section::manager::instance.user_infos);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A49A8, &visible_section::manager::instance.user_infos[0]);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A49C4, &visible_section::manager::instance.user_infos[1]);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A49E3, &visible_section::manager::instance.user_infos[2]);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A4A02, &visible_section::manager::instance.user_infos[3]);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A4A21, &visible_section::manager::instance.user_infos[4]);

        // GrandSceneryCullInfo::WhatSectionsShouldWeDraw
        hook::set(0x007A4A60, &visible_section::manager::instance.user_infos);

        // SceneryGroup::DisableRendering
        hook::set(0x007A2DFB, &visible_section::manager::instance.user_infos);

        // SceneryGroup::EnableRendering
        hook::set(0x007A2D97, &visible_section::manager::instance.user_infos);

        // SceneryOverrideInfo::AssignOverrides
        hook::set(0x0079F236, &visible_section::manager::instance.user_infos);

        // SceneryOverrideInfo::SetExcludeFlags
        hook::set(0x0079F266, &visible_section::manager::instance.user_infos);

        // eRenderWorldLightFlares
        hook::set(0x0074E1DA, &visible_section::manager::instance.user_infos);
        
        // emTriggerEventsInSection
        hook::set(0x007A3680, &visible_section::manager::instance.user_infos);

        // UnloaderEventManager
        hook::set(0x007A1B23, &visible_section::manager::instance.user_infos);

        // UnloaderEventManager
        hook::set(0x007A1B34, &visible_section::manager::instance.user_infos);

        // UnloaderEventManager
        hook::set(0x007A1B5F, &visible_section::manager::instance.user_infos);

        // UnloaderLightFlares
        hook::set(0x00747815, &visible_section::manager::instance.user_infos);

        // UnloaderLights
        hook::set(0x007461B4, &visible_section::manager::instance.user_infos);

        // UnloaderScenery
        hook::set(0x007A6092, &visible_section::manager::instance.user_infos);

        // UnloaderScenery
        hook::set(0x007A60A1, &visible_section::manager::instance.user_infos);

        // UnloaderScenery
        hook::set(0x007A60CC, &visible_section::manager::instance.user_infos);

        // UnloaderEventManager
        hook::set(0x007A1B42, &visible_section::manager::instance.allocated_user_info_count);

        // UnloaderEventManager
        hook::set(0x007A1B48, visible_section::manager::instance.unallocated_user_info_list.head()->prev_ref());

        // UnloaderEventManager
        hook::set(0x007A1B4F, visible_section::manager::instance.unallocated_user_info_list.head()->prev_ref());

        // UnloaderEventManager
        hook::set(0x007A1B58, visible_section::manager::instance.unallocated_user_info_list.head()->next_ref());

        // UnloaderScenery
        hook::set(0x007A60AF, &visible_section::manager::instance.allocated_user_info_count);

        // UnloaderScenery
        hook::set(0x007A60B5, visible_section::manager::instance.unallocated_user_info_list.head()->prev_ref());

        // UnloaderScenery
        hook::set(0x007A60BC, visible_section::manager::instance.unallocated_user_info_list.head()->prev_ref());

        // UnloaderScenery
        hook::set(0x007A60C5, visible_section::manager::instance.unallocated_user_info_list.head()->next_ref());

        // CloseTopologyAndSceneryGroups
        hook::set(0x00699DC3, &visible_section::manager::instance.enabled_groups);

        // TrackLoader::Unload
        hook::set(0x006B5AFE, &visible_section::manager::instance.enabled_groups);

        // TrackLoader::RedoTopologyAndSceneryGroups
        hook::set(0x006A8AE3, &visible_section::manager::instance.enabled_groups);

        // TrackLoader::CloseTopologyAndSceneryGroups
        hook::set(0x006A8AC3, &visible_section::manager::instance.enabled_groups);

        // CloseTopologyAndSceneryGroups
        hook::set(0x00699DBC, std::size(visible_section::manager::instance.enabled_groups));

        // TrackLoader::Unload
        hook::set(0x006B5AF9, std::size(visible_section::manager::instance.enabled_groups));

        // TrackLoader::RedoTopologyAndSceneryGroups
        hook::set(0x006A8ADE, std::size(visible_section::manager::instance.enabled_groups));

        // TrackLoader::CloseTopologyAndSceneryGroups
        hook::set(0x006A8ABC, std::size(visible_section::manager::instance.enabled_groups));
    }
}
