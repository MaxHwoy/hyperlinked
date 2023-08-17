#include <hyperlib/global_vars.hpp>
#include <hyperlib/utils/utils.hpp>
#include <hyperlib/utils/file.hpp>
#include <hyperlib/assets/loader.hpp>
#include <hyperlib/memory/memory.hpp>
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
        if (callback != nullptr)
        {
            if (this->section_activate_callback_count < std::size(this->section_activate_callback))
            {
                this->section_activate_callback[this->section_activate_callback_count++] = callback;
            }
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

    void streamer::clear_streaming_positions()
    {
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].position_set = false;
        this->position_entries[static_cast<std::uint32_t>(position_type::player1)].following_car = false;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].position_set = false;
        this->position_entries[static_cast<std::uint32_t>(position_type::player2)].following_car = false;
    }

    void streamer::close_region()
    {
        this->unload_everything();
    }



    bool streamer::determine_current_zones(std::uint16_t* zones)
    {
        return call_function<bool(__thiscall*)(streamer*, std::uint16_t*)>(0x007A4850)(this, zones);
    }

    void streamer::disable_zone_switching(const char* reason)
    {
        this->zone_switching_disabled = true;
        this->zone_switching_disabled_reason = reason;
    }


    void streamer::enable_zone_switching()
    {
        this->zone_switching_disabled = false;
        this->zone_switching_disabled_reason = nullptr;
    }



    auto streamer::find_section(std::uint16_t section_number) -> section*
    {
        for (std::uint32_t i = 0u; i < this->section_count; ++i)
        {
            streamer::section* section = this->sections + i;

            if (section->number == section_number)
            {
                return section;
            }
        }

        return nullptr;
    }

    void streamer::free_section_memory()
    {
        this->out_of_memory_section_count = 0u;

        for (std::uint32_t i = 0u; i < this->section_count; ++i)
        {
            streamer::section& section = this->sections[i];

            if (section.status == section::status_type::allocated)
            {
                memory::free(section.memory);

                section.disc = nullptr;
                section.memory = nullptr;
                section.status = section::status_type::unloaded;
            }
        }
    }

    void streamer::free_user_memory(void* ptr)
    {
        memory::free(ptr);
    }



    void streamer::handle_loading()
    {
        call_function<void(__thiscall*)(streamer*)>(0x007A7230)(this);
    }


    void streamer::init_memory_pool(alloc_size_t pool_size)
    {
        pool_size = math::align_pow_2<alloc_size_t>(pool_size, 0x80u);
#if defined(_DEBUG)
        void* ptr = memory::malloc_debug(pool_size, memory::create_allocation_params(memory::pool_type::main, false, false, 0x80u, 0x00u), __FILE__, __LINE__);
#else
        void* ptr = memory::malloc(pool_size, memory::create_allocation_params(memory::pool_type::main, false, false, 0x80u, 0x00u));
#endif
        this->memory_heap = span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(ptr), static_cast<size_t>(pool_size));
        this->memory_pool = new ts_memory_pool(ptr, pool_size, "Hyper Streamer", static_cast<char>(memory::pool_type::streamer));

        memory::set_pool_override_info(memory::pool_type::streamer, this->memory_pool->get_override_info());
    }


    auto streamer::jettison_least_important_section() -> streamer::section*
    {
        return call_function<streamer::section*(__thiscall*)(streamer*)>(0x007A2B30)(this);
    }

    void streamer::jettison_section(streamer::section& section)
    {
        this->amount_jettisoned += section.size;

        this->jettisoned_sections[this->jettisoned_section_count++] = &section;

        if (section.status == section::status_type::activated)
        {
            this->unactivate_section(section);
        }

        if (section.status == section::status_type::loaded)
        {
            this->unload_section(section);
        }

        for (std::uint32_t i = 0; i < this->current_section_count; ++i)
        {
            if (&section == this->current_sections[i])
            {
                this->current_sections[i] = this->current_sections[--this->current_section_count];

                break;
            }
        }
    }



    void streamer::load_section(streamer::section& section)
    {
        this->loading_section_count++;

        section.status = section::status_type::loading;

        const char* filename = reinterpret_cast<const char*>(this->filenames[static_cast<std::uint32_t>(section.type)]);

        if (section.size == section.compressed_size)
        {
            file::add_queued_file(section.memory, filename, section.file_offset, section.size, &streamer::section_loaded_callback, &section, nullptr);
        }
        else
        {
            file::queued_file_params params;

            params.uncompressed_size = section.size;
            params.priority = file::default_queued_file_priority;
            params.block_size = 0x7FFFFFF;
            params.compressed = true;

            file::add_queued_file(section.memory, filename, section.file_offset, section.compressed_size, &streamer::section_loaded_callback, &section, &params);
        }
    }


    bool streamer::make_space_in_pool(alloc_size_t size, bool force_unloading)
    {
        this->wait_for_current_loading_to_complete();

        while (size > memory::count_free_memory(memory::pool_type::streamer))
        {
            streamer::section* best_to_unload = nullptr;

            for (std::uint32_t i = 0u; i < this->section_count; ++i)
            {
                streamer::section& section = this->sections[i];

                if (section.status == section::status_type::loaded && section.currently_visible == 0u)
                {
                    if (best_to_unload == nullptr || section.last_needed_timestamp < best_to_unload->last_needed_timestamp)
                    {
                        best_to_unload = &section;
                    }
                }
            }

            if (best_to_unload != nullptr)
            {
                this->unload_section(*best_to_unload);

                if (best_to_unload->loaded_size != 0u)
                {
                    continue;
                }
            }

            if (!force_unloading)
            {
                break;
            }

            streamer::section* best_to_jettison = this->jettison_least_important_section();

            if (best_to_jettison == nullptr)
            {
                break;
            }

            this->jettison_section(*best_to_jettison);
        }

        return size <= memory::largest_malloc(memory::create_allocation_params(memory::pool_type::streamer, false, false, 0x00u, 0x00u));
    }

    void streamer::notify_section_activation(std::uint32_t section_number, bool activated)
    {
        for (std::uint32_t i = 0u; i < this->section_activate_callback_count; ++i)
        {
            this->section_activate_callback[i](section_number, activated);
        }
    }



    void streamer::ready_to_make_space_in_pool_bridge()
    {
        this->make_space_in_pool(this->make_space_in_pool_size, true);

        void(*callback)(void*) = this->make_space_in_pool_callback;
        void* param = this->make_space_in_pool_callback_param;

        this->make_space_in_pool_callback = nullptr;
        this->make_space_in_pool_callback_param = nullptr;
        this->make_space_in_pool_size = 0u;

        callback(param);
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
            this->current_sections[i]->currently_visible = 0u;
        }

        this->current_section_count = 0;

        this->current_section_table.clear();
    }

    void streamer::remove_section_activate_callback(void(*callback)(std::int32_t, bool))
    {
        for (std::uint32_t i = 0u; i < this->section_activate_callback_count; ++i)
        {
            if (callback == this->section_activate_callback[i])
            {
                this->section_activate_callback[i--] = this->section_activate_callback[--this->section_activate_callback_count];

                this->section_activate_callback[this->section_activate_callback_count] = nullptr;
            }
        }
    }



    void streamer::section_loaded_callback(streamer::section* section)
    {
        assert(section != nullptr);

        // #TODO
    }

    void streamer::switch_zones(const std::uint16_t* zones)
    {
        reinterpret_cast<void(__thiscall*)(streamer*, const std::uint16_t*)>(0x007A7F10)(this, zones);
    }




    void streamer::unactivate_section(streamer::section& section)
    {
        section.unactivated_frame_count = global::frame_counter;

        loader::unload_chunks(section.memory, section.loaded_size);

        this->activated_section_count--;

        this->unload_section(section);

        this->notify_section_activation(section.number, false);
    }

    void streamer::unload_everything()
    {
        while (this->loading_section_count != 0u)
        {
            loader::service_resource_loading();
        }

        for (std::uint32_t i = 0u; i < this->section_count; ++i)
        {
            streamer::section& section = this->sections[i];

            if (section.status == section::status_type::loaded || section.status == section::status_type::activated)
            {
                this->unload_section(section);
            }
        }

        this->free_section_memory();
        this->clear_current_zones();
    }

    void streamer::unload_section(streamer::section& section)
    {
        if (section.status == section::status_type::activated)
        {
            this->unactivate_section(section);
        }

        if (section.status == section::status_type::loaded)
        {
            std::uint32_t count = section.unactivated_frame_count;

            if (count != 0u && count == global::frame_counter && this->last_wait_until_rendering_done_frame_count != global::frame_counter)
            {
                this->last_wait_until_rendering_done_frame_count = global::frame_counter;
            }

            memory::free(section.memory);

            section.memory = nullptr;
            section.unactivated_frame_count = 0u;
            section.loaded_time = 0;
            section.loaded_size = 0u;
            section.status = section::status_type::unloaded;

            this->loaded_section_count--;
        }
    }

    void streamer::wait_for_current_loading_to_complete()
    {
        while (this->phase != loading_phase::idle || (this->activated_section_count + this->out_of_memory_section_count) < this->current_section_count)
        {
            this->handle_loading();

            loader::service_resource_loading();

            utils::thread_yield(8u);
        }
    }



    void streamer::section_loaded_callback(void* param, bool failed)
    {
        assert(!failed);

        streamer::instance.section_loaded_callback(reinterpret_cast<streamer::section*>(param));
    }


    /*
    void streamer::DetermineStreamingSections((void))
    {
        // 007A4680
    }
    void streamer::DiscBundleLoadedCallback((DiscBundleSection*))
    {
        // 007A29D0
    }
    void streamer::DiscBundleLoadedCallback((void*, int))
    {
        // 007A43E0
    }
    void streamer::GetLoadingPriority((TrackStreamingSection*, StreamingPositionEntry*, bool))
    {
        // 0079ECE0
    }
    void streamer::HandleMemoryAllocation(void)
    {
        // 007A7060
    }
    void streamer::InitRegion((char const*, bool))
    {
        // 007A27F0
    }
    void streamer::IsLoadingInProgress((void))
    {
        // 007A7E80
    }
    void streamer::LoadDiscBundle((DiscBundleSection*))
    {
        // 007A5530
    }
    void streamer::Loader((bChunk*))
    {
        // 0079E440
    }
    void streamer::MakeSpaceInPool((int, void (*)(void*), void*))
    {
        // 007A8570
    }
    void streamer::PredictStreamingPosition((int, bVector3 const*, bVector3 const*, bVector3 const*, bool))
    {
        // 0079A170
    }
    void streamer::SectionLoadedCallback((TrackStreamingSection*))
    {
        // 0079E780
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
    void streamer::Unloader((bChunk*))
    {
        // 007A2680
    }
    */
}
