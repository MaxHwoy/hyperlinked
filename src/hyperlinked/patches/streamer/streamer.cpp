#include <hyperlib/streamer/streamer.hpp>
#include <hyperlinked/patches/streamer/streamer.hpp>

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

    __declspec(naked) void detour_track_streamer_is_user_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'ptr'
            // ecx contains pointer to streamer

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'ptr' is now at [esp + 0x08]
            push ecx; // 'ptr' is now at [esp + 0x0C]
            push edx; // 'ptr' is now at [esp + 0x10]
            push esi; // 'ptr' is now at [esp + 0x14]
            push edi; // 'ptr' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'ptr'

            call streamer::is_user_memory; // call custom is_user_memory

            // no need to restore esp since 'is_user_memory' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::IsUserMemory; note that this is a __thiscall
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

        // TrackStreamer::InitRegion
        hook::jump(0x007A27F0, &detour_track_streamer_init_region);

        // TrackStreamer::IsLoadingInProgress
        hook::jump(0x007A7E80, &detour_track_streamer_is_loading_in_progress);

        // TrackStreamer::IsUserMemory
        hook::jump(0x0079A340, &detour_track_streamer_is_user_memory);

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

        // BELOW ARE PATCHES THAT REPLACE THE TRACK STREAMER POINTERS

        // CAnimPlayer::Load
        hook::set(0x004590A0, &streamer::instance);

        // CAnimPlayer::Load
        hook::set(0x004590AA, &streamer::instance);

        // CAnimPlayer::Load
        hook::set(0x004590B8, &streamer::instance);

        // CAnimPlayer::Load
        hook::set(0x004590CB, &streamer::instance);

        // CAnimPlayer::Load
        hook::set(0x004590DB, &streamer::instance);

        // CAnimPlayer::Load
        hook::set(0x004590EC, &streamer::instance);

        // CAnimPlayer::Unload
        hook::set(0x004591A0, &streamer::instance);

        // CAnimPlayer::Unload
        hook::set(0x0045920E, &streamer::instance);

        // .text:0045BDB0
        hook::set(0x0045BDB1, &streamer::instance);

        // sub_0045BDF0+2D
        hook::set(0x0045BE1E, &streamer::instance);

        // sub_0045F900
        hook::set(0x0045F91C, &streamer::instance);

        // UpdateCameraMovers
        hook::set(0x00490B1E, &streamer::instance);

        // UpdateCameraMovers
        hook::set(0x00490C1A, &streamer::instance);

        // sub_00502C70+26
        hook::set(0x00502C97, &streamer::instance);

        // EAXAemsManager::InitiateLoad
        hook::set(0x00516D99, &streamer::instance);

        // EAXAemsManager::InitiateLoad
        hook::set(0x00516DC0, &streamer::instance);

        // sub_00520810+AD
        hook::set(0x005208BE, &streamer::instance);

        // EAXAemsManager::Update
        hook::set(0x005219EA, &streamer::instance);

        // sub_0056DE90+12
        hook::set(0x0056DEA3, &streamer::instance);

        // sub_0056DE90
        hook::set(0x0056DEAD, &streamer::instance);

        // sub_0056DE90+26
        hook::set(0x0056DEB7, &streamer::instance);

        // FETrackPreviewStreamer::~FETrackPreviewStreamer
        hook::set(0x0056F953, &streamer::instance);

        // FETrackPreviewStreamer::~FETrackPreviewStreamer
        hook::set(0x0056F95D, &streamer::instance);

        // FETrackPreviewStreamer::~FETrackPreviewStreamer
        hook::set(0x0056F967, &streamer::instance);

        // sub_0056F9D0+18
        hook::set(0x0056F9E9, &streamer::instance);

        // sub_0056F9D0
        hook::set(0x0056FA5E, &streamer::instance);

        // FEPackageData::GetDataChunk
        hook::set(0x00575BEB, &streamer::instance);

        // .text:0057B807
        hook::set(0x0057B808, &streamer::instance);

        // sub_0057EF60+71
        hook::set(0x0057EFD2, &streamer::instance);

        // sub_0057EFF0+25
        hook::set(0x0057F016, &streamer::instance);

        // sub_0057F030+13
        hook::set(0x0057F044, &streamer::instance);

        // sub_0057F030+22
        hook::set(0x0057F053, &streamer::instance);

        // sub_0057F0A0+2B
        hook::set(0x0057F0CC, &streamer::instance);

        // sub_0057F110+13
        hook::set(0x0057F124, &streamer::instance);

        // sub_0057F110+22
        hook::set(0x0057F133, &streamer::instance);

        // .text
        hook::set(0x0057F176, &streamer::instance);

        // sub_00583D10
        hook::set(0x00583D4F, &streamer::instance);

        // sub_0058E3E0+59
        hook::set(0x0058E43A, &streamer::instance);

        // FEImageStreamerManager::FEImageStreamerManager
        hook::set(0x0059710B, &streamer::instance);

        // FEImageStreamerManager::FEImageStreamerManager
        hook::set(0x00597137, &streamer::instance);

        // FEImageStreamerManager::FEImageStreamerManager
        hook::set(0x00597162, &streamer::instance);

        // .text:0059FAAD
        hook::set(0x0059FAAE, &streamer::instance);

        // FECareerStateManager::HandleScreenTick
        hook::set(0x005A58F0, &streamer::instance);

        // FECareerStateManager::HandleScreenTick
        hook::set(0x005A592C, &streamer::instance);

        // FECareerStateManager::HandleScreenTick
        hook::set(0x005A595B, &streamer::instance);

        // FECareerStateManager::HandleScreenTick
        hook::set(0x005A5998, &streamer::instance);

        // FECareerStateManager::HandleScreenTick
        hook::set(0x005A59EF, &streamer::instance);

        // UITrackMapStreamer::UITrackMapStreamer
        hook::set(0x005B76FA, &streamer::instance);

        // UITrackMapStreamer::UITrackMapStreamer
        hook::set(0x005B770F, &streamer::instance);

        // FEAnyMovieScreen::FEAnyMovieScreen
        hook::set(0x005BB79D, &streamer::instance);

        // WorldMap::SetupTrackPreviewStreamingData
        hook::set(0x005C3811, &streamer::instance);

        // sub_005D76F0+20
        hook::set(0x005D7711, &streamer::instance);

        // sub_005D76F0+68
        hook::set(0x005D7759, &streamer::instance);

        // .text:005E0CCB
        hook::set(0x005E0CCC, &streamer::instance);

        // .text:005E0CF7
        hook::set(0x005E0CF8, &streamer::instance);

        // GIcon::FindSection
        hook::set(0x00615FC2, &streamer::instance);

        // GCanyonRaceStatus::IsIntermissionSync
        hook::set(0x00616DB0, &streamer::instance);

        // GManager::NotifyWorldService
        hook::set(0x0062AB02, &streamer::instance);

        // GManager::WarpToMarker
        hook::set(0x0062AE13, &streamer::instance);

        // GManager::SpawnAllLoadedSectionIcons
        hook::set(0x0062B186, &streamer::instance);

        // GManager::SpawnAllLoadedSectionIcons
        hook::set(0x0062B19F, &streamer::instance);

        // GManager::CanPlaySMS
        hook::set(0x0062B481, &streamer::instance);

        // GRaceStatus::NotifyScriptWhenLoaded
        hook::set(0x0062D983, &streamer::instance);

        // GRaceStatus::NotifyScriptWhenLoaded
        hook::set(0x0062D9FD, &streamer::instance);

        // GManager::PostClearSessionData
        hook::set(0x0063174B, &streamer::instance);

        // GRaceStatus::SetRoaming
        hook::set(0x00641377, &streamer::instance);

        // GRaceStatus::SetRoaming
        hook::set(0x0064138A, &streamer::instance);

        // sub_00646F6D+49D
        hook::set(0x0064740B, &streamer::instance);

        // sub_00646F6D+520
        hook::set(0x0064748E, &streamer::instance);

        // GManager::PostStartActivities
        hook::set(0x006494CA, &streamer::instance);

        // Game_CanyonRaceSetup
        hook::set(0x00656C10, &streamer::instance);

        // Game_WarpPlayerAndWingmanToMarker
        hook::set(0x0065B677, &streamer::instance);

        // Game_WarpPlayerAndWingmanToMarker
        hook::set(0x0065B690, &streamer::instance);

        // Game_WarpPlayerAndWingmanToMarker
        hook::set(0x0065B6C5, &streamer::instance);

        // Game_WarpPlayerAndWingmanToMarker
        hook::set(0x0065B6E6, &streamer::instance);

        // Game_KnockoutPursuit
        hook::set(0x0065DC70, &streamer::instance);

        // Game_KnockoutPursuit
        hook::set(0x0065DCC0, &streamer::instance);

        // Game_KnockoutPursuit
        hook::set(0x0065DCD6, &streamer::instance);

        // .text:00699F70
        hook::set(0x00699F71, &streamer::instance);

        // HandleTrackStreamerLoadingBar
        hook::set(0x0069A044, &streamer::instance);

        // HandleTrackStreamerLoadingBar
        hook::set(0x0069A064, &streamer::instance);

        // RegionLoader::FinishedLoading
        hook::set(0x006A89EF, &streamer::instance);

        // DelayWaitForLoadingScreen
        hook::set(0x006A8C01, &streamer::instance);

        // TrackLoader::Unload
        hook::set(0x006B5A54, &streamer::instance);

        // LoadGlobalChunks
        hook::set(0x006B6F29, &streamer::instance);

        // MiniMainLoop
        hook::set(0x006B78F0, &streamer::instance);

        // MiniMainLoop
        hook::set(0x006B78FA, &streamer::instance);

        // MainLoop
        hook::set(0x006B7B7F, &streamer::instance);

        // RegionLoader::Unload
        hook::set(0x006B7C26, &streamer::instance);

        // RegionLoader::Unload
        hook::set(0x006B7D91, &streamer::instance);

        // LoadFrontEndRegion
        hook::set(0x006B7E4E, &streamer::instance);

        // GameFlowLoadingFrontEndPart4
        hook::set(0x006B7EF5, &streamer::instance);

        // GameFlowLoadingFrontEndPart4
        hook::set(0x006B7EFF, &streamer::instance);

        // GameFlowLoadingFrontEndPart4
        hook::set(0x006B7F09, &streamer::instance);

        // BeginGameFlowUnloadingFrontend
        hook::set(0x006BC179, &streamer::instance);

        // QuickGame::IsStateDone
        hook::set(0x00762B2C, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764D79, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764D8E, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DAC, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DB6, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DC7, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DD1, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DE0, &streamer::instance);

        // Sim::SetStream
        hook::set(0x00764DEA, &streamer::instance);

        // NISActivity::ServiceLoads
        hook::set(0x00766702, &streamer::instance);

        // NISActivity::ServiceLoads
        hook::set(0x007667B3, &streamer::instance);

        // NISActivity::ServiceLoads
        hook::set(0x0076694F, &streamer::instance);

        // sub_00768CC0
        hook::set(0x00768D4A, &streamer::instance);

        // NISActivity::Load
        hook::set(0x007696C8, &streamer::instance);

        // QuickGame::QuickGame
        hook::set(0x0076CF2E, &streamer::instance);

        // QuickGame::QuickGame
        hook::set(0x0076CF6D, &streamer::instance);

        // QuickGame::QuickGame
        hook::set(0x0076CF82, &streamer::instance);

        // QuickGame::QuickGame
        hook::set(0x0076CF8C, &streamer::instance);

        // QuickGame::QuickGame
        hook::set(0x0076CF96, &streamer::instance);

        // OnlineGame::OnlineGame
        hook::set(0x0076D49C, &streamer::instance);

        // OnlineGame::OnlineGame
        hook::set(0x0076D4B1, &streamer::instance);

        // OnlineGame::OnlineGame
        hook::set(0x0076D4BB, &streamer::instance);

        // OnlineGame::OnlineGame
        hook::set(0x0076D4C5, &streamer::instance);

        // LoaderTrackStreamer
        hook::set(0x007A2536, &streamer::instance);

        // TrackStreamer::SectionLoadedCallback
        hook::set(0x007A2A86, &streamer::instance);

        // UnloaderTrackStreamer
        hook::set(0x007A42D6, &streamer::instance);

        // TrackStreamer::DiscBundleLoadedCallback
        hook::set(0x007A43E6, &streamer::instance);

        // TrackStreamer::StartLoadingSections
        hook::set(0x007A5604, &streamer::instance);

        // bRefreshTrackStreamer
        hook::set(0x007A870C, &streamer::instance);

        // bRefreshTrackStreamer
        hook::set(0x007A871E, &streamer::instance);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A54, &streamer::instance);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A67, &streamer::instance);

        // .text:007AB29B
        hook::set(0x007AB29C, &streamer::instance);

        // sub_007AB4D0+1C
        hook::set(0x007AB4ED, &streamer::instance);

        // .text:007BD433
        hook::set(0x007BD434, &streamer::instance);

        // GhostCarManager::Deserialize
        hook::set(0x007BD527, &streamer::instance);

        // World_Service
        hook::set(0x007DCD35, &streamer::instance);

        // Game_SetRaceActivity
        hook::set(0x007E297C, &streamer::instance);

        // CarLoader::LoadSkinTextures
        hook::set(0x007E4364, &streamer::instance);

        // CarLoader::SetMemoryPoolSize
        hook::set(0x007E499C, &streamer::instance);

        // FECarClassSelectStateManager::HandleScreenTick
        hook::set(0x00847AEA, &streamer::instance);

        // BeginGameFlowUnloadingFrontend
        hook::set(0x006BC142, &streamer::instance.perm_file_loading);

        // BeginGameFlowUnloadingFrontend
        hook::set(0x006BC156, &streamer::instance.perm_file_loading);

        // bRefreshTrackStreamer
        hook::set(0x007A86E5, &streamer::instance.position_entries[static_cast<std::uint32_t>(streamer::position_type::player1)].predicted_zone_valid_time);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A2C, &streamer::instance.position_entries[static_cast<std::uint32_t>(streamer::position_type::player1)].predicted_zone_valid_time);

        // bRefreshTrackStreamer
        hook::set(0x007A86EA, &streamer::instance.position_entries[static_cast<std::uint32_t>(streamer::position_type::player2)].predicted_zone_valid_time);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A31, &streamer::instance.position_entries[static_cast<std::uint32_t>(streamer::position_type::player2)].predicted_zone_valid_time);

        // sub_005058C0
        hook::set(0x005058EE, &streamer::instance.phase);

        // sub_0050E850
        hook::set(0x0050EA63, &streamer::instance.phase);

        // sub_0051C190
        hook::set(0x0051C1A8, &streamer::instance.phase);

        // Speech::Manager::Update
        hook::set(0x0077C65F, &streamer::instance.loading_backlog);

        // sub_00785220
        hook::set(0x00785265, &streamer::instance.loading_backlog);

        // GRaceStatus::NotifyScriptWhenLoaded
        hook::set(0x0062D9F4, &streamer::instance.current_zone_far_load);

        // sub_00646F6D
        hook::set(0x00647485, &streamer::instance.current_zone_far_load);

        // bRefreshTrackStreamer
        hook::set(0x007A86F7, &streamer::instance.current_zone_needs_refreshing);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A3E, &streamer::instance.current_zone_needs_refreshing);

        // bRefreshTrackStreamer
        hook::set(0x007A86EF, &streamer::instance.zone_switching_disabled);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A36, &streamer::instance.zone_switching_disabled);

        // sub_0057EF60
        hook::set(0x0057EFAC, &streamer::instance.memory_heap);

        // sub_0057EFF0
        hook::set(0x0057EFF1, &streamer::instance.memory_heap);

        // sub_0057F030
        hook::set(0x0057F031, &streamer::instance.memory_heap);

        // sub_0057F0A0
        hook::set(0x0057F0BD, &streamer::instance.memory_heap);

        // sub_0057F110
        hook::set(0x0057F111, &streamer::instance.memory_heap);

        // bRefreshTrackStreamer
        hook::set(0x007A86FF, &streamer::instance.memory_heap);

        // VisibleSectionManager::Loader
        hook::set(0x007A8A46, &streamer::instance.memory_heap);
    }
}
