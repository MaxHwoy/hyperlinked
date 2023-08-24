#include <hyperlib/streamer/streamer.hpp>
#include <hyperlinked/patches/streamer.hpp>

namespace hyper
{
    __declspec(naked) void detour_track_streamer_activate_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::activate_section; // call custom activate_section

            // no need to restore esp since 'activate_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::ActivateSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_add_current_streaming_sections()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'sections_to_load'
            // [esp + 0x08] is 'sections_to_load_count'
            // [esp + 0x0C] is 'type'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'type' is now at [esp + 0x10]
            push ebx; // 'type' is now at [esp + 0x14]
            push ecx; // 'type' is now at [esp + 0x18]
            push edx; // 'type' is now at [esp + 0x1C]
            push esi; // 'type' is now at [esp + 0x20]
            push edi; // 'type' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'type'
            push [esp + 0x24]; // repush 'sections_to_load_count'
            push [esp + 0x24]; // repush 'sections_to_load'

            call streamer::add_current_sections; // call custom add_current_sections

            // no need to restore esp since 'add_current_sections' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 12; // return immediately to caller function, not back to TrackStreamer::AddCurrentStreamingSections; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_add_section_activate_callback()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'callback'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'callback' is now at [esp + 0x08]
            push ebx; // 'callback' is now at [esp + 0x0C]
            push ecx; // 'callback' is now at [esp + 0x10]
            push edx; // 'callback' is now at [esp + 0x14]
            push esi; // 'callback' is now at [esp + 0x18]
            push edi; // 'callback' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'callback'

            call streamer::add_section_activate_callback; // call custom add_section_activate_callback

            // no need to restore esp since 'add_section_activate_callback' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::AddSectionActivateCallback; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_allocate_section_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'total_needing_allocation'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'total_needing_allocation' is now at [esp + 0x08]
            push ecx; // 'total_needing_allocation' is now at [esp + 0x0C]
            push edx; // 'total_needing_allocation' is now at [esp + 0x10]
            push esi; // 'total_needing_allocation' is now at [esp + 0x14]
            push edi; // 'total_needing_allocation' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'total_needing_allocation'

            call streamer::allocate_section_memory; // call custom allocate_section_memory

            // no need to restore esp since 'allocate_section_memory' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::AllocateSectionMemory; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_allocate_user_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'
            // [esp + 0x08] is 'debug_name'
            // [esp + 0x0C] is 'offset'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'offset' is now at [esp + 0x10]
            push ecx; // 'offset' is now at [esp + 0x14]
            push edx; // 'offset' is now at [esp + 0x18]
            push esi; // 'offset' is now at [esp + 0x1C]
            push edi; // 'offset' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'offset'
            push [esp + 0x20]; // repush 'debug_name'
            push [esp + 0x20]; // repush 'size'

            call streamer::allocate_user_memory; // call custom allocate_user_memory

            // no need to restore esp since 'allocate_user_memory' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 12; // return immediately to caller function, not back to TrackStreamer::AllocateUserMemory; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_assign_loading_priority()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::assign_loading_priority; // call custom assign_loading_priority

            // no need to restore esp since 'assign_loading_priority' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::AssignLoadingPriority; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_block_until_loading_complete()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::block_until_loading_complete; // call custom block_until_loading_complete

            // no need to restore esp since 'block_until_loading_complete' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::BlockUntilLoadingComplete; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_calculate_loading_backlog()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::calculate_loading_backlog; // call custom calculate_loading_backlog

            // no need to restore esp since 'calculate_loading_backlog' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::CalculateLoadingBacklog; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_check_loading_bar()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless (actually just al, but it doesn't matter b/c verified)
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call streamer::check_loading_bar; // call custom check_loading_bar

            // no need to restore esp since 'check_loading_bar' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::CheckLoadingBar; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_clear_current_zones()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::clear_current_zones; // call custom clear_current_zones

            // no need to restore esp since 'clear_current_zones' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::ClearCurrentZones; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_clear_streaming_positions()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::clear_streaming_positions; // call custom clear_streaming_positions

            // no need to restore esp since 'clear_streaming_positions' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::ClearStreamingPositions; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_close_region()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::close_region; // call custom close_region

            // no need to restore esp since 'close_region' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::CloseRegion; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_determine_current_zones()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'zones'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless (actually just al, but it doesn't matter b/c verified)
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'zones' is now at [esp + 0x08]
            push ecx; // 'zones' is now at [esp + 0x0C]
            push edx; // 'zones' is now at [esp + 0x10]
            push esi; // 'zones' is now at [esp + 0x14]
            push edi; // 'zones' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'zones'

            call streamer::determine_current_zones; // call custom determine_current_zones

            // no need to restore esp since 'determine_current_zones' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::DetermineCurrentZones; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_determine_streaming_sections()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::determine_streaming_sections; // call custom determine_streaming_sections

            // no need to restore esp since 'determine_streaming_sections' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::DetermineStreamingSections; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_disable_zone_switching()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'reason'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'reason' is now at [esp + 0x08]
            push ebx; // 'reason' is now at [esp + 0x0C]
            push ecx; // 'reason' is now at [esp + 0x10]
            push edx; // 'reason' is now at [esp + 0x14]
            push esi; // 'reason' is now at [esp + 0x18]
            push edi; // 'reason' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'reason'

            call streamer::disable_zone_switching; // call custom disable_zone_switching

            // no need to restore esp since 'disable_zone_switching' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::DisableZoneSwitching; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_disc_bundle_loaded_callback()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'disc'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'disc' is now at [esp + 0x08]
            push ebx; // 'disc' is now at [esp + 0x0C]
            push ecx; // 'disc' is now at [esp + 0x10]
            push edx; // 'disc' is now at [esp + 0x14]
            push esi; // 'disc' is now at [esp + 0x18]
            push edi; // 'disc' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'disc'

            call streamer::disc_bundle_loaded_callback; // call custom disc_bundle_loaded_callback

            // no need to restore esp since 'disc_bundle_loaded_callback' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::DiscBundleLoadedCallback; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_enable_zone_switching()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::enable_zone_switching; // call custom enable_zone_switching

            // no need to restore esp since 'enable_zone_switching' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::EnableZoneSwitching; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_find_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section_number'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'section_number' is now at [esp + 0x08]
            push ecx; // 'section_number' is now at [esp + 0x0C]
            push edx; // 'section_number' is now at [esp + 0x10]
            push esi; // 'section_number' is now at [esp + 0x14]
            push edi; // 'section_number' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'section_number'

            call streamer::find_section; // call custom find_section

            // no need to restore esp since 'find_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::FindSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_finished_loading()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::finished_loading; // call custom finished_loading

            // no need to restore esp since 'finished_loading' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::FinishedLoading; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_free_section_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::free_section_memory; // call custom free_section_memory

            // no need to restore esp since 'free_section_memory' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::FreeSectionMemory; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_free_user_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'ptr'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'ptr' is now at [esp + 0x08]
            push ebx; // 'ptr' is now at [esp + 0x0C]
            push ecx; // 'ptr' is now at [esp + 0x10]
            push edx; // 'ptr' is now at [esp + 0x14]
            push esi; // 'ptr' is now at [esp + 0x18]
            push edi; // 'ptr' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'ptr'

            call streamer::free_user_memory; // call custom free_user_memory

            // no need to restore esp since 'free_user_memory' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::FreeSectionMemory; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_get_combined_section_number()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section_number'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'section_number' is now at [esp + 0x08]
            push ecx; // 'section_number' is now at [esp + 0x0C]
            push edx; // 'section_number' is now at [esp + 0x10]
            push esi; // 'section_number' is now at [esp + 0x14]
            push edi; // 'section_number' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'section_number'

            call streamer::get_combined_section_number; // call custom get_combined_section_number

            // no need to restore esp since 'get_combined_section_number' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::GetCombinedSectionNumber; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_get_loading_priority()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // [esp + 0x08] is 'entry'
            // [esp + 0x0C] is 'use_direction'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'use_direction' is now at [esp + 0x10]
            push ecx; // 'use_direction' is now at [esp + 0x14]
            push edx; // 'use_direction' is now at [esp + 0x18]
            push esi; // 'use_direction' is now at [esp + 0x1C]
            push edi; // 'use_direction' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'use_direction'
            push [esp + 0x20]; // repush 'entry'
            push [esp + 0x20]; // repush 'section'

            call streamer::get_loading_priority; // call custom get_loading_priority

            // no need to restore esp since 'get_loading_priority' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 12; // return immediately to caller function, not back to TrackStreamer::GetLoadingPriority; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_get_predicted_zone()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'entry'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'entry' is now at [esp + 0x08]
            push ecx; // 'entry' is now at [esp + 0x0C]
            push edx; // 'entry' is now at [esp + 0x10]
            push esi; // 'entry' is now at [esp + 0x14]
            push edi; // 'entry' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'entry'

            call streamer::get_predicted_zone; // call custom get_predicted_zone

            // no need to restore esp since 'get_predicted_zone' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::GetPredictedZone; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_handle_loading()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::handle_loading; // call custom handle_loading

            // no need to restore esp since 'handle_loading' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::HandleLoading; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_handle_memory_allocation()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call streamer::handle_memory_allocation; // call custom handle_memory_allocation

            // no need to restore esp since 'handle_memory_allocation' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::HandleMemoryAllocation; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_init_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pool_size'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pool_size' is now at [esp + 0x08]
            push ebx; // 'pool_size' is now at [esp + 0x0C]
            push ecx; // 'pool_size' is now at [esp + 0x10]
            push edx; // 'pool_size' is now at [esp + 0x14]
            push esi; // 'pool_size' is now at [esp + 0x18]
            push edi; // 'pool_size' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'pool_size'

            call streamer::init_memory_pool; // call custom init_memory_pool

            // no need to restore esp since 'init_memory_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::InitMemoryPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_init_region()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'file'
            // [esp + 0x08] is 'is_split_screen'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'is_split_screen' is now at [esp + 0x0C]
            push ebx; // 'is_split_screen' is now at [esp + 0x10]
            push ecx; // 'is_split_screen' is now at [esp + 0x14]
            push edx; // 'is_split_screen' is now at [esp + 0x18]
            push esi; // 'is_split_screen' is now at [esp + 0x1C]
            push edi; // 'is_split_screen' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'is_split_screen'
            push [esp + 0x20]; // repush 'file'

            call streamer::init_region; // call custom init_region

            // no need to restore esp since 'init_region' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to TrackStreamer::InitRegion; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_is_loading_in_progress()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless (actually just al, but it doesn't matter b/c verified)
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call streamer::is_loading_in_progress; // call custom is_loading_in_progress

            // no need to restore esp since 'is_loading_in_progress' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::IsLoadingInProgress; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_jettison_least_important_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call streamer::jettison_least_important_section; // call custom jettison_least_important_section

            // no need to restore esp since 'jettison_least_important_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::JettisonLeastImportantSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_jettison_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::jettison_section; // call custom jettison_section

            // no need to restore esp since 'jettison_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::JettisonSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_load_disc_bundle()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'disc'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'disc' is now at [esp + 0x08]
            push ebx; // 'disc' is now at [esp + 0x0C]
            push ecx; // 'disc' is now at [esp + 0x10]
            push edx; // 'disc' is now at [esp + 0x14]
            push esi; // 'disc' is now at [esp + 0x18]
            push edi; // 'disc' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'disc'

            call streamer::load_disc_bundle; // call custom load_disc_bundle

            // no need to restore esp since 'load_disc_bundle' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::LoadDiscBundle; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_load_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::load_section; // call custom load_section

            // no need to restore esp since 'load_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::LoadSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_loader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call streamer::loader; // call custom loader

            // no need to restore esp since 'loader' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::Loader; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_make_space_in_pool_bool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'
            // [esp + 0x08] is 'force_unloading'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'force_unloading' is now at [esp + 0x0C]
            push ecx; // 'force_unloading' is now at [esp + 0x10]
            push edx; // 'force_unloading' is now at [esp + 0x14]
            push esi; // 'force_unloading' is now at [esp + 0x18]
            push edi; // 'force_unloading' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'force_unloading'
            push [esp + 0x1C]; // repush 'size'

            call streamer::make_space_in_pool; // call custom make_space_in_pool

            // no need to restore esp since 'make_space_in_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 8; // return immediately to caller function, not back to TrackStreamer::MakeSpaceInPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_make_space_in_pool_call()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'
            // [esp + 0x08] is 'make_space_callback'
            // [esp + 0x0C] is 'param'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'param' is now at [esp + 0x10]
            push ebx; // 'param' is now at [esp + 0x14]
            push ecx; // 'param' is now at [esp + 0x18]
            push edx; // 'param' is now at [esp + 0x1C]
            push esi; // 'param' is now at [esp + 0x20]
            push edi; // 'param' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'param'
            push [esp + 0x24]; // repush 'make_space_callback'
            push [esp + 0x24]; // repush 'size'

            call streamer::make_space_in_pool_with_callback; // call custom make_space_in_pool_with_callback

            // no need to restore esp since 'make_space_in_pool_with_callback' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 12; // return immediately to caller function, not back to TrackStreamer::MakeSpaceInPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_notify_section_activation()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section_number'
            // [esp + 0x08] is 'activated'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'activated' is now at [esp + 0x0C]
            push ebx; // 'activated' is now at [esp + 0x10]
            push ecx; // 'activated' is now at [esp + 0x14]
            push edx; // 'activated' is now at [esp + 0x18]
            push esi; // 'activated' is now at [esp + 0x1C]
            push edi; // 'activated' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'activated'
            push [esp + 0x20]; // repush 'section_number'

            call streamer::notify_section_activation; // call custom notify_section_activation

            // no need to restore esp since 'notify_section_activation' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to TrackStreamer::NotifySectionActivation; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_predict_streaming_position()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'position'
            // [esp + 0x0C] is 'velocity'
            // [esp + 0x10] is 'direction'
            // [esp + 0x14] is 'is_following_car'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'is_following_car' is now at [esp + 0x18]
            push ebx; // 'is_following_car' is now at [esp + 0x1C]
            push ecx; // 'is_following_car' is now at [esp + 0x20]
            push edx; // 'is_following_car' is now at [esp + 0x24]
            push esi; // 'is_following_car' is now at [esp + 0x28]
            push edi; // 'is_following_car' is now at [esp + 0x2C]

            push [esp + 0x2C]; // repush 'is_following_car'
            push [esp + 0x2C]; // repush 'direction'
            push [esp + 0x2C]; // repush 'velocity'
            push [esp + 0x2C]; // repush 'position'
            push [esp + 0x2C]; // repush 'type'

            call streamer::predict_streaming_position; // call custom predict_streaming_position

            // no need to restore esp since 'predict_streaming_position' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 20; // return immediately to caller function, not back to TrackStreamer::PredictStreamingPosition; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_refresh_loading()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::refresh_loading; // call custom refresh_loading

            // no need to restore esp since 'refresh_loading' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::RefreshLoading; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_remove_current_streaming_sections()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::remove_current_streaming_sections; // call custom remove_current_streaming_sections

            // no need to restore esp since 'remove_current_streaming_sections' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::RemoveCurrentStreamingSections; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_remove_section_activate_callback()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'activate_callback'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'activate_callback' is now at [esp + 0x08]
            push ebx; // 'activate_callback' is now at [esp + 0x0C]
            push ecx; // 'activate_callback' is now at [esp + 0x10]
            push edx; // 'activate_callback' is now at [esp + 0x14]
            push esi; // 'activate_callback' is now at [esp + 0x18]
            push edi; // 'activate_callback' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'activate_callback'

            call streamer::remove_section_activate_callback; // call custom remove_section_activate_callback

            // no need to restore esp since 'remove_section_activate_callback' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::RemoveSectionActivateCallback; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_section_loaded_callback()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::section_loaded_callback; // call custom section_loaded_callback

            // no need to restore esp since 'section_loaded_callback' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::SectionLoadedCallback; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_service_game_state()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::service_game_state; // call custom service_game_state

            // no need to restore esp since 'service_game_state' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::ServiceGameState; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_service_non_game_state()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::service_non_game_state; // call custom service_non_game_state

            // no need to restore esp since 'service_non_game_state' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::ServiceNonGameState; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_set_streaming_position()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'position'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'position' is now at [esp + 0x0C]
            push ebx; // 'position' is now at [esp + 0x10]
            push ecx; // 'position' is now at [esp + 0x14]
            push edx; // 'position' is now at [esp + 0x18]
            push esi; // 'position' is now at [esp + 0x1C]
            push edi; // 'position' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'position'
            push [esp + 0x20]; // repush 'type'

            call streamer::set_streaming_position; // call custom set_streaming_position

            // no need to restore esp since 'set_streaming_position' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to TrackStreamer::SetStreamingPosition; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_start_loading_sections()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::start_loading_sections; // call custom start_loading_sections

            // no need to restore esp since 'start_loading_sections' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::StartLoadingSections; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_switch_zones()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'zones'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'zones' is now at [esp + 0x08]
            push ebx; // 'zones' is now at [esp + 0x0C]
            push ecx; // 'zones' is now at [esp + 0x10]
            push edx; // 'zones' is now at [esp + 0x14]
            push esi; // 'zones' is now at [esp + 0x18]
            push edi; // 'zones' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'zones'

            call streamer::switch_zones; // call custom switch_zones

            // no need to restore esp since 'switch_zones' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::SwitchZones; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_unactivate_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::unactivate_section; // call custom unactivate_section

            // no need to restore esp since 'unactivate_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::UnactivateSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_unjettison_sections()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::unjettison_sections; // call custom unjettison_sections

            // no need to restore esp since 'unjettison_sections' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::UnJettisonSections; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_unload_everything()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::unload_everything; // call custom unload_everything

            // no need to restore esp since 'unload_everything' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::UnloadEverything; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_unload_section()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'section'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'section' is now at [esp + 0x08]
            push ebx; // 'section' is now at [esp + 0x0C]
            push ecx; // 'section' is now at [esp + 0x10]
            push edx; // 'section' is now at [esp + 0x14]
            push esi; // 'section' is now at [esp + 0x18]
            push edi; // 'section' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'section'

            call streamer::unload_section; // call custom unload_section

            // no need to restore esp since 'unload_section' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::UnloadSection; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_unloader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call streamer::unloader; // call custom unloader

            // no need to restore esp since 'unloader' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::Unloader; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_track_streamer_wait_for_current_loading_to_complete()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call streamer::wait_for_current_loading_to_complete; // call custom wait_for_current_loading_to_complete

            // no need to restore esp since 'wait_for_current_loading_to_complete' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to TrackStreamer::WaitForCurrentLoadingToComplete; note that this is a __thiscall
        }
    }

    void streamer_patches::init()
    {
        // TrackStreamer::ActivateSection
        hook::jump(0x0079E670, &detour_track_streamer_activate_section);

        // TrackStreamer::AddCurrentStreamingSections
        hook::jump(0x0079E8F0, &detour_track_streamer_add_current_streaming_sections);

        // TrackStreamer::AddSectionActivateCallback
        hook::jump(0x0079A490, &detour_track_streamer_add_section_activate_callback);

        // TrackStreamer::AllocateSectionMemory
        hook::jump(0x0079EA00, &detour_track_streamer_allocate_section_memory);

        // TrackStreamer::AllocateUserMemory
        hook::jump(0x0079A2E0, &detour_track_streamer_allocate_user_memory);

        // TrackStreamer::AssignLoadingPriority
        hook::jump(0x0079EEE0, &detour_track_streamer_assign_loading_priority);

        // TrackStreamer::BlockUntilLoadingComplete
        hook::jump(0x007A8730, &detour_track_streamer_block_until_loading_complete);

        // TrackStreamer::CalculateLoadingBacklog
        hook::jump(0x0079A3F0, &detour_track_streamer_calculate_loading_backlog);

        // TrackStreamer::CheckLoadingBar
        hook::jump(0x007A82E0, &detour_track_streamer_check_loading_bar);

        // TrackStreamer::ClearCurrentZones
        hook::jump(0x0079E530, &detour_track_streamer_clear_current_zones);

        // TrackStreamer::ClearStreamingPositions
        hook::jump(0x0079A200, &detour_track_streamer_clear_streaming_positions);

        // TrackStreamer::CloseRegion
        hook::jump(0x007A2930, &detour_track_streamer_close_region);

        // TrackStreamer::DetermineCurrentZones
        hook::jump(0x007A4850, &detour_track_streamer_determine_current_zones);

        // TrackStreamer::DetermineStreamingSections
        hook::jump(0x007A4680, &detour_track_streamer_determine_streaming_sections);

        // TrackStreamer::DisableZoneSwitching
        hook::jump(0x0079A3C0, &detour_track_streamer_disable_zone_switching);

        // TrackStreamer::DiscBundleLoadedCallback
        hook::jump(0x007A29D0, &detour_track_streamer_disc_bundle_loaded_callback);

        // TrackStreamer::EnableZoneSwitching
        hook::jump(0x0079A3E0, &detour_track_streamer_enable_zone_switching);

        // TrackStreamer::FindSection
        hook::jump(0x00799ED0, &detour_track_streamer_find_section);

        // TrackStreamer::FinishedLoading
        hook::jump(0x0079EF90, &detour_track_streamer_finished_loading);

        // TrackStreamer::FreeSectionMemory
        hook::jump(0x0079A290, &detour_track_streamer_free_section_memory);

        // TrackStreamer::FreeUserMemory
        hook::jump(0x0079A330, &detour_track_streamer_free_user_memory);

        // TrackStreamer::GetCombinedSectionNumber
        hook::jump(0x00799F60, &detour_track_streamer_get_combined_section_number);

        // TrackStreamer::GetLoadingPriority
        hook::jump(0x0079ECE0, &detour_track_streamer_get_loading_priority);

        // TrackStreamer::GetPredictedZone
        hook::jump(0x007A44B0, &detour_track_streamer_get_predicted_zone);

        // TrackStreamer::HandleLoading
        hook::jump(0x007A7230, &detour_track_streamer_handle_loading);

        // TrackStreamer::HandleMemoryAllocation
        hook::jump(0x007A7060, &detour_track_streamer_handle_memory_allocation);

        // TrackStreamer::InitMemoryPool
        hook::jump(0x007A42E0, &detour_track_streamer_init_memory_pool);

        // TrackStreamer::InitRegion
        hook::jump(0x007A27F0, &detour_track_streamer_init_region);

        // TrackStreamer::IsLoadingInProgress
        hook::jump(0x007A7E80, &detour_track_streamer_is_loading_in_progress);

        // TrackStreamer::JettisonLeastImportantSection
        hook::jump(0x007A2B30, &detour_track_streamer_jettison_least_important_section);

        // TrackStreamer::JettisonSection
        hook::jump(0x007A2A90, &detour_track_streamer_jettison_section);

        // TrackStreamer::LoadDiscBundle
        hook::jump(0x007A5530, &detour_track_streamer_load_disc_bundle);

        // TrackStreamer::LoadSection
        hook::jump(0x007A43F0, &detour_track_streamer_load_section);

        // TrackStreamer::Loader
        hook::jump(0x0079E440, &detour_track_streamer_loader);

        // TrackStreamer::MakeSpaceInPool
        hook::jump(0x007A81B0, &detour_track_streamer_make_space_in_pool_bool);

        // TrackStreamer::MakeSpaceInPool
        hook::jump(0x007A8570, &detour_track_streamer_make_space_in_pool_call);

        // TrackStreamer::NotifySectionActivation
        hook::jump(0x0079A520, &detour_track_streamer_notify_section_activation);

        // TrackStreamer::PredictStreamingPosition
        hook::jump(0x0079A170, &detour_track_streamer_predict_streaming_position);

        // TrackStreamer::RefreshLoading
        hook::jump(0x007A8690, &detour_track_streamer_refresh_loading);

        // TrackStreamer::RemoveCurrentStreamingSections
        hook::jump(0x0079A220, &detour_track_streamer_remove_current_streaming_sections);

        // TrackStreamer::RemoveSectionActivateCallback
        hook::jump(0x0079A4B0, &detour_track_streamer_remove_section_activate_callback);

        // TrackStreamer::SectionLoadedCallback
        hook::jump(0x0079E780, &detour_track_streamer_section_loaded_callback);

        // TrackStreamer::ServiceGameState
        hook::jump(0x007A85E0, &detour_track_streamer_service_game_state);

        // TrackStreamer::ServiceNonGameState
        hook::jump(0x007A7E10, &detour_track_streamer_service_non_game_state);

        // TrackStreamer::SetStreamingPosition
        hook::jump(0x0079A0B0, &detour_track_streamer_set_streaming_position);

        // TrackStreamer::StartLoadingSections
        hook::jump(0x007A5590, &detour_track_streamer_start_loading_sections);

        // TrackStreamer::SwitchZones
        hook::jump(0x007A7F10, &detour_track_streamer_switch_zones);

        // TrackStreamer::UnactivateSection
        hook::jump(0x007A2A20, &detour_track_streamer_unactivate_section);

        // TrackStreamer::UnJettisonSections
        hook::jump(0x0079E890, &detour_track_streamer_unjettison_sections);

        // TrackStreamer::UnloadEverything
        hook::jump(0x0079F030, &detour_track_streamer_unload_everything);

        // TrackStreamer::UnloadSection
        hook::jump(0x0079E700, &detour_track_streamer_unload_section);

        // TrackStreamer::Unloader
        hook::jump(0x007A2680, &detour_track_streamer_unloader);

        // TrackStreamer::WaitForCurrentLoadingToComplete
        hook::jump(0x007A7E30, &detour_track_streamer_wait_for_current_loading_to_complete);
    }
}
