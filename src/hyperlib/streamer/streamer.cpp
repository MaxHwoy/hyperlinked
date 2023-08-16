#include <hyperlib/streamer/streamer.hpp>

namespace hyper
{
    streamer::streamer() :
        current_section_table(streamer::section_table_memory_.pointer(), streamer::section_table_memory_.length()),
        sections(nullptr),
        section_count(0u),
        discs(nullptr),
        last_disc(nullptr),
        loaded_section_count(0u),
        loading_section_count(0u),
        activated_section_count(0u),
        out_of_memory_section_count(0u),
        moved_section_count(0u),
        filenames{},
        split_screen(false),
        perm_file_loading(false),
        perm_filename(nullptr),
        perm_file_chunks(nullptr),
        perm_file_size(0u),
        perm_file_chunks_loaded(false),
        perm_file_non_region_a(nullptr),
        barrier_count(0u),
        barriers(nullptr),
        current_section_count(0u),
        current_zone_needs_refreshing(false),
        zone_switching_disabled(false),
        zone_switching_disabled_reason(nullptr),
        last_wait_until_rendering_done_frame_count(0u),
        last_printed_frame_count(0u),
        skip_next_handle_load(false),
        memory_heap(),
        user_memory_allocation_size(0u),
        memory_pool(nullptr),
        keep_section_table{},
        callback(nullptr),
        callback_param(nullptr),
        make_space_in_pool_callback(nullptr),
        make_space_in_pool_callback_param(nullptr),
        make_space_in_pool_size(0u),
        section_activate_callback_count(0u),
        section_activate_callback{}
    {
        this->clear_current_zones();
    }

    void streamer::activate_section(streamer::section* section)
    {
        call_function<void(__thiscall*)(streamer*, streamer::section*)>(0x0079E670)(this, section);
    }

    void streamer::add_current_sections(std::uint16_t* sections_to_load, std::uint32_t section_count, position_type type)
    {
        call_function<void(__thiscall*)(streamer*, std::uint16_t*, std::uint32_t, position_type)>(0x0079E8F0)(this, sections_to_load, section_count, type);
    }

    void streamer::add_section_activate_callback(void(*callback)(std::int32_t, bool))
    {
        if (this->section_activate_callback_count < std::size(this->section_activate_callback))
        {
            this->section_activate_callback[this->section_activate_callback_count++] = callback;
        }
    }

    auto streamer::allocate_section_memory(std::uint32_t* total_needing_allocation) -> void*
    {
        return call_function<void*(__thiscall*)(streamer*, std::uint32_t*)>(0x0079EA00)(this, total_needing_allocation);
    }

    auto streamer::allocate_user_memory(std::uint32_t size, const char* debug_name, std::uint32_t offset) -> void*
    {
        std::uint32_t params = size <= memory::largest_malloc(memory::create_allocation_params(memory::pool_type::streamer, false, false, 0x00u, 0x00u))
            ? memory::create_allocation_params(memory::pool_type::streamer, true, false, 0x80u, offset)
            : memory::create_allocation_params(memory::pool_type::main, false, false, 0x80u, offset);
#if defined(_DEBUG)
        return memory::malloc_debug(size, params, debug_name, 0u);
#else
        return memory::malloc(size, params);
#endif
    }

    void streamer::block_until_loading_complete()
    {
        this->refresh_loading();
        this->wait_for_current_loading_to_complete();
    }

    bool streamer::check_loading_bar()
    {
        return call_function<bool(__thiscall*)(streamer*)>(0x007A82E0)(this);
    }

    void streamer::clear_current_zones()
    {
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].current_zone = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].begin_loading_time = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].begin_loading_position.x = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].begin_loading_position.y = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].sections_to_load_count = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].loaded_section_count = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].amount_to_load = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].amount_loaded = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].current_zone = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].begin_loading_time = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].begin_loading_position.x = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].begin_loading_position.y = 0.0f;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].sections_to_load_count = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].loaded_section_count = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].amount_to_load = 0u;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].amount_loaded = 0u;
        this->phase = loading_phase::idle;
        this->loading_backlog = 0.0f;
        this->current_zone_out_of_memory = false;
        this->current_zone_allocated_but_incomplete = false;
        this->current_zone_non_replay_load = false;
        this->current_zone_far_load = true;
        this->current_zone_name[0] = 0;
        this->start_loading_time = 0.0f;
        this->memory_safety_margin = 0u;
        this->amount_jettisoned = 0u;
        this->jettisoned_section_count = 0u;
        
        ::memset(this->jettisoned_sections, 0, sizeof(this->jettisoned_sections));
        
        this->remove_current_streaming_sections();
    }


    bool streamer::determine_current_zones(std::uint16_t* zones)
    {
        return call_function<bool(__thiscall*)(streamer*, std::uint16_t*)>(0x007A4850)(this, zones);
    }



    void streamer::refresh_loading()
    {
        std::uint16_t zones[2]{};

        this->current_zone_needs_refreshing = true;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].predicted_zone_valid_time = -1;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].predicted_zone_valid_time = -1;

        if (!this->zone_switching_disabled && !this->memory_heap.is_empty() && this->determine_current_zones(zones))
        {
            this->switch_zones(zones);
        }
    }

    void streamer::remove_current_streaming_sections()
    {
        for (std::uint32_t i = 0u; i < this->current_section_count; ++i)
        {
            this->current_sections[i]->currently_visible = false;
        }

        this->current_section_count = 0;

        this->current_section_table.clear();
    }




    void streamer::switch_zones(const std::uint16_t* zones)
    {
        reinterpret_cast<void(__thiscall*)(streamer*, const std::uint16_t*)>(0x007A7F10)(this, zones);
    }


    void streamer::wait_for_current_loading_to_complete()
    {
        call_function<void(__thiscall*)(streamer*)>(0x007A7E30)(this);
    }



    /*
    void streamer::ClearStreamingPositions((void))
    {
        // 0079A200
    }
    void streamer::CloseRegion((void))
    {
        // 007A2930
    }
    void streamer::DetermineCurrentZones(std::uint16_t* zones)
    {
        // 007A4850
    }
    void streamer::DetermineStreamingSections((void))
    {
        // 007A4680
    }
    void streamer::DisableZoneSwitching((char const*))
    {
        // 0079A3C0
    }
    void streamer::DiscBundleLoadedCallback((DiscBundleSection*))
    {
        // 007A29D0
    }
    void streamer::DiscBundleLoadedCallback((void*, int))
    {
        // 007A43E0
    }
    void streamer::EnableZoneSwitching((void))
    {
        // 0079A3E0
    }
    void streamer::FindSection((int))
    {
        // 00799ED0
    }
    void streamer::FreeSectionMemory((void))
    {
        // 0079A290
    }
    void streamer::FreeUserMemory((void*))
    {
        // 0079A330
    }
    void streamer::GetLoadingPriority((TrackStreamingSection*, StreamingPositionEntry*, bool))
    {
        // 0079ECE0
    }
    void streamer::HandleLoading((void))
    {
        // 007A7230
    }
    void streamer::HandleMemoryAllocation(void)
    {
        // 007A7060
    }
    void streamer::InitMemoryPool((int))
    {
        // 007A42E0
    }
    void streamer::InitRegion((char const*, bool))
    {
        // 007A27F0
    }
    void streamer::IsLoadingInProgress((void))
    {
        // 007A7E80
    }
    void streamer::JettisonLeastImportantSection((void))
    {
        // 007A2B30
    }
    void streamer::JettisonSection(TrackStreamingSection*)
    {
        // 007A2A90
    }
    void streamer::LoadDiscBundle((DiscBundleSection*))
    {
        // 007A5530
    }
    void streamer::LoadSection(TrackStreamingSection*)
    {
        // 007A43F0
    }
    void streamer::Loader((bChunk*))
    {
        // 0079E440
    }
    void streamer::MakeSpaceInPool((int, bool))
    {
        // 007A81B0
    }
    void streamer::MakeSpaceInPool((int, void (*)(void*), void*))
    {
        // 007A8570
    }
    void streamer::NotifySectionActivation((int, bool))
    {
        // 0079A520
    }
    void streamer::PredictStreamingPosition((int, bVector3 const*, bVector3 const*, bVector3 const*, bool))
    {
        // 0079A170
    }
    void streamer::ReadyToMakeSpaceInPoolBridge(void*)
    {
        // 007A8530
    }
    
    
    void streamer::RemoveSectionActivateCallback(void (*)(int, bool))
    {
        // 0079A4B0
    }
    void streamer::SectionLoadedCallback((TrackStreamingSection*))
    {
        // 0079E780
    }
    void streamer::SectionLoadedCallback((void*, int))
    {
        // 007A2A80
    }
    void streamer::ServiceGameState((void))
    {
        // 007A85E0
    }
    void streamer::ServiceNonGameState((void))
    {
        // 007A7E10
    }
    void streamer::SetStreamingPosition((int, bVector3 const*))
    {
        // 0079A0B0
    }
    void streamer::StartLoadingSections((void))
    {
        // 007A5590
    }
    void streamer::TrackStreamer((void))
    {
        // 007A2540
    }
    void streamer::UnactivateSection((TrackStreamingSection*))
    {
        // 007A2A20
    }
    void streamer::UnloadEverything((void))
    {
        // 0079F030
    }
    void streamer::UnloadSection((TrackStreamingSection*))
    {
        // 0079E700
    }
    void streamer::Unloader((bChunk*))
    {
        // 007A2680
    }
    */
}
