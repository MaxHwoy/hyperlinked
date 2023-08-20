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
            // #TODO
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
            // #TODO
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
        // hook::jump(0x0079EA00, &detour_track_streamer_allocate_section_memory);

        // TrackStreamer::AllocateUserMemory
        hook::jump(0x0079A2E0, &detour_track_streamer_allocate_user_memory);

        // TrackStreamer::BlockUntilLoadingComplete
        hook::jump(0x007A8730, &detour_track_streamer_block_until_loading_complete);

        // TrackStreamer::CalculateLoadingBacklog
        hook::jump(0x0079A3F0, &detour_track_streamer_calculate_loading_backlog);

        // TrackStreamer::CheckLoadingBar
        // hook::jump(0x007A82E0, &detour_track_streamer_check_loading_bar);

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

        // TrackStreamer::FreeSectionMemory
        hook::jump(0x0079A290, &detour_track_streamer_free_section_memory);

        // TrackStreamer::FreeUserMemory
        hook::jump(0x0079A330, &detour_track_streamer_free_user_memory);

        // 

        // TrackStreamer::LoadSection
        hook::jump(0x007A43F0, &detour_track_streamer_load_section);

        // TrackStreamer::Loader
        hook::jump(0x0079E440, &detour_track_streamer_loader);



        // TrackStreamer::UnactivateSection
        hook::jump(0x007A2A20, &detour_track_streamer_unactivate_section);

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
