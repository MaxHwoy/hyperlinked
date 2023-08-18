#include <hyperlib/global_vars.hpp>
#include <hyperlib/utils/utils.hpp>
#include <hyperlib/utils/file.hpp>
#include <hyperlib/assets/loader.hpp>
#include <hyperlib/memory/memory.hpp>
#include <hyperlib/streamer/streamer.hpp>

//#define SORT_STREAMING_SECTIONS

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

    void streamer::activate_section(streamer::section& section)
    {
        std::uint32_t params = memory::create_allocation_params(memory::pool_type::streamer, false, true, 0x80u, 0x00u);

        global::allow_duplicate_solids++;
        global::duplicate_texture_warning_enabled = 0;

        loader::load_temp_perm_chunks(&section.memory, &section.loaded_size, params, reinterpret_cast<const char*>(section.name));

        global::allow_duplicate_solids--;
        global::duplicate_texture_warning_enabled = 1;

        section.status = section::status_type::activated;
        section.loaded_time = 0;

        this->activated_section_count++;

        this->notify_section_activation(section.number, true);
    }

    void streamer::add_current_sections(std::uint16_t* sections_to_load, std::uint32_t sections_to_load_count, position_type type)
    {
        for (std::uint32_t i = 0u; i < sections_to_load_count; ++i)
        {
            std::uint16_t section_number = sections_to_load[i];

            this->current_section_table.set(section_number, true);

            streamer::section* section = this->find_section(section_number);

            if (section != nullptr)
            {
                section->last_needed_timestamp = global::real_time_frames;

                if (section->currently_visible == 0u)
                {
                    this->current_sections[this->current_section_count++] = section;
                }

                if (((1u << static_cast<std::uint32_t>(type)) & section->currently_visible) == 0u)
                {
                    section->currently_visible |= static_cast<std::uint8_t>(1u << static_cast<std::uint32_t>(type));

                    if (section->status <= section::status_type::loading)
                    {
                        this->position_entries[static_cast<std::uint32_t>(type)].amount_to_load += section->size;
                        this->position_entries[static_cast<std::uint32_t>(type)].sections_to_load_count++;
                    }
                }
            }
        }
    }

    void streamer::add_section_activate_callback(void(*activate_callback)(std::int32_t, bool))
    {
        if (activate_callback != nullptr)
        {
            if (this->section_activate_callback_count < std::size(this->section_activate_callback))
            {
                this->section_activate_callback[this->section_activate_callback_count++] = activate_callback;
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

    void streamer::calculate_loading_backlog()
    {
        float total_backlog = 0.0f;

        for (std::uint32_t i = 0u; i < this->current_section_count; ++i)
        {
            streamer::section* section = this->current_sections[i];

            if (section->currently_visible != 0u)
            {
                section::status_type status = section->status;

                if (status != section::status_type::loaded && status != section::status_type::activated)
                {
                    float section_backlog = static_cast<float>(section->size >> 10u) * 0.0001f;

                    switch (section->base_loading_priority)
                    {
                        case 1:
                            section_backlog *= 0.4f;
                            break;

                        case 2:
                            section_backlog *= 0.2f;
                            break;

                        default:
                            break;
                    }

                    total_backlog += section_backlog;
                }
            }
        }

        this->loading_backlog = total_backlog;
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
#if defined(_DEBUG)
        ::memset(this->jettisoned_sections, 0, sizeof(this->jettisoned_sections));
#endif
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
        bool refresh = false;

        for (std::uint32_t i = 0; i < std::size(this->position_entries); ++i)
        {
            std::uint16_t predicted_zone = 0u;

            position_entry& entry = this->position_entries[i];

            if (entry.position_set)
            {
                predicted_zone = this->get_predicted_zone(entry.position);
            }

            if (predicted_zone == entry.predicted_zone)
            {
                entry.predicted_zone_valid_time++;
            }
            else
            {
                entry.predicted_zone = predicted_zone;

                if (entry.predicted_zone_valid_time < 0)
                {
                    entry.predicted_zone_valid_time = 1000;
                }
                else
                {
                    entry.predicted_zone_valid_time = 1;
                }
            }

            if (predicted_zone != entry.current_zone)
            {
                if (entry.predicted_zone_valid_time < 0)
                {
                    predicted_zone = entry.current_zone;
                }

                if (predicted_zone != entry.current_zone)
                {
                    refresh = true;
                }
            }

            zones[i] = predicted_zone;
        }

        return refresh || this->current_zone_needs_refreshing;
    }

    void streamer::determine_streaming_sections()
    {
        std::uint16_t sections_to_load[0x180];

        this->remove_current_streaming_sections();

        std::uint32_t sections_to_load_count = 3u;

        sections_to_load[0] = game_provider::shared_texture_section;
        sections_to_load[1] = game_provider::shared_solid_section;
        sections_to_load[2] = game_provider::shared_scenery_section;

        for (std::uint32_t i = 0u; i < std::size(this->keep_section_table); ++i)
        {
            if (this->keep_section_table[i] != 0u)
            {
                sections_to_load[sections_to_load_count++] = this->keep_section_table[i];
            }
        }

        this->add_current_sections(sections_to_load, sections_to_load_count, position_type::player1);
        this->add_current_sections(sections_to_load, sections_to_load_count, position_type::player2);

        visible_section::manager& manager = visible_section::manager::instance;

        for (std::uint32_t p = 0u; p < std::size(this->position_entries); ++p)
        {
            std::uint16_t current_zone = this->position_entries[p].current_zone;

            if (current_zone != 0u && current_zone != std::numeric_limits<std::uint16_t>::max()) // #TODO
            {
                const visible_section::loading* loading = manager.find_loading_section(current_zone);

                if (loading == nullptr)
                {
                    const visible_section::drivable* drivable = manager.find_drivable_section(current_zone);

                    if (drivable != nullptr)
                    {
                        ::memcpy(sections_to_load, drivable->visible_sections, (sections_to_load_count = drivable->visible_section_count) * sizeof(std::uint16_t));
                    }
                }
                else
                {
                    sections_to_load_count = manager.get_sections_to_load(loading, sections_to_load, static_cast<std::uint32_t>(std::size(sections_to_load)));
                }

                this->add_current_sections(sections_to_load, sections_to_load_count, static_cast<position_type>(p));
            }
        }
    }

    void streamer::disable_zone_switching(const char* reason)
    {
        this->zone_switching_disabled = true;
        this->zone_switching_disabled_reason = reason;
    }

    void streamer::disc_bundle_loaded_callback(disc_bundle* disc)
    {
        assert(disc != nullptr);

        this->loading_section_count += disc->member_count - 1u;

        for (std::uint32_t i = 0u; i < disc->member_count; ++i)
        {
            disc->members[i].section->disc = nullptr;

            this->section_loaded_callback(disc->members[i].section);
        }
    }

    void streamer::enable_zone_switching()
    {
        this->zone_switching_disabled = false;
        this->zone_switching_disabled_reason = nullptr;
    }

    auto streamer::find_section(std::uint16_t section_number) -> section*
    {
#if defined(SORT_STREAMING_SECTIONS)
        return reinterpret_cast<section*>(utils::scan_hash_table_key16(section_number, this->sections, this->section_count, offsetof(section, number), sizeof(section)));
#else
        for (std::uint32_t i = 0u; i < this->section_count; ++i)
        {
            streamer::section* section = this->sections + i;

            if (section->number == section_number)
            {
                return section;
            }
        }

        return nullptr;
#endif
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

    //

    auto streamer::get_predicted_zone(const vector3& position) -> std::uint16_t
    {
        return call_function<std::uint16_t(__thiscall*)(streamer*, const vector3&)>(0x007A44B0)(this, position);
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
#if defined(_DEBUG)
                this->current_sections[this->current_section_count] = nullptr;
#endif
                break;
            }
        }
    }

    void streamer::load_disc_bundle(streamer::disc_bundle& disc)
    {
        if (disc.member_count != 0u)
        {
            void* memory = disc.members[0].section->memory;

            for (std::uint32_t i = 0u; i < disc.member_count; ++i)
            {
                disc.members[i].section->status = section::status_type::loading;
            }

            this->loading_section_count++;

            const char* filename = reinterpret_cast<const char*>(this->filenames[1]);

            file::add_queued_file(memory, filename, disc.file_offset, disc.file_size, streamer::disc_bundle_loaded_callback_static, &disc, nullptr);
        }
    }

    void streamer::load_section(streamer::section& section)
    {
        this->loading_section_count++;

        section.status = section::status_type::loading;

        const char* filename = reinterpret_cast<const char*>(this->filenames[static_cast<std::uint32_t>(section.type)]);

        if (section.size == section.compressed_size)
        {
            file::add_queued_file(section.memory, filename, section.file_offset, section.size, &streamer::section_loaded_callback_static, &section, nullptr);
        }
        else
        {
            file::queued_file_params params;

            params.uncompressed_size = section.size;
            params.priority = file::default_queued_file_priority;
            params.block_size = 0x7FFFFFF;
            params.compressed = true;

            file::add_queued_file(section.memory, filename, section.file_offset, section.compressed_size, &streamer::section_loaded_callback_static, &section, &params);
        }
    }

    bool streamer::loader(chunk* block)
    {
        if (block->id() == block_id::track_streaming_discs)
        {
            this->discs = reinterpret_cast<disc_bundle*>(block->data());
            this->last_disc = reinterpret_cast<disc_bundle*>(block->end());

            disc_bundle* current = this->discs;

            while (current != this->last_disc)
            {
                for (std::uint32_t i = 0u; i < current->member_count; ++i)
                {
                    current->members[i].section = this->find_section(current->members[i].section_number);
                }

                uintptr_t address = reinterpret_cast<uintptr_t>(current);

                address += (sizeof(disc_bundle) - sizeof(disc_bundle::members)) + current->member_count * sizeof(disc_bundle::member);

                current = reinterpret_cast<disc_bundle*>(address);
            }

            return true;
        }
        else
        {
            switch (block->id())
            {
                case block_id::track_streaming_sections:
                    this->sections = reinterpret_cast<section*>(block->data());
                    this->section_count = block->size() / sizeof(section);
#if defined(SORT_STREAMING_SECTIONS)
                    utils::sort(this->sections, this->section_count, [](const section& lhs, const section& rhs) -> bool { return lhs.number < rhs.number; });
#endif
                    return true;

                case block_id::track_streaming_infos:
                    this->the_info = reinterpret_cast<info*>(block->data());
                    return true;

                case block_id::track_streaming_barriers:
                    this->barriers = reinterpret_cast<barrier*>(block->data());
                    this->barrier_count = block->size() / sizeof(barrier);
                    return true;

                default:
                    return false;
            }
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



    void streamer::predict_streaming_position(position_type type, const vector3& position, const vector3& velocity, const vector3& direction, bool is_following_car)
    {
        position_entry& entry = this->position_entries[static_cast<std::uint32_t>(type)];

        entry.position = position;
        entry.velocity = velocity;
        entry.direction = direction;

        entry.position2D = position.as_vector2();
        entry.velocity2D = velocity.as_vector2();
        entry.direction2D = direction.as_vector2();

        entry.position_set = true;
        entry.following_car = is_following_car;
    }



    void streamer::ready_to_make_space_in_pool_bridge()
    {
        this->make_space_in_pool(this->make_space_in_pool_size, true);

        void(*pool_callback)(void*) = this->make_space_in_pool_callback;
        void* param = this->make_space_in_pool_callback_param;

        this->make_space_in_pool_callback = nullptr;
        this->make_space_in_pool_callback_param = nullptr;
        this->make_space_in_pool_size = 0u;

        pool_callback(param);
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
#if defined(_DEBUG)
            this->current_sections[i] = nullptr;
#endif
        }

        this->current_section_count = 0;

        this->current_section_table.clear();
    }

    void streamer::remove_section_activate_callback(void(*activate_callback)(std::int32_t, bool))
    {
        for (std::uint32_t i = 0u; i < this->section_activate_callback_count; ++i)
        {
            if (activate_callback == this->section_activate_callback[i])
            {
                this->section_activate_callback[i--] = this->section_activate_callback[--this->section_activate_callback_count];

                this->section_activate_callback[this->section_activate_callback_count] = nullptr;
            }
        }
    }



    void streamer::section_loaded_callback(streamer::section* section)
    {
        assert(section != nullptr);

        section->loaded_size = section->size;
        section->status = section::status_type::loaded;
        section->loaded_time = global::real_time_frames;

        this->loaded_section_count++;
        this->loading_section_count--;

        std::uint8_t visibility = section->currently_visible;

        if (visibility != 0u)
        {
            this->activate_section(*section);
        }

        if ((visibility & 1) != 0)
        {
            this->position_entries[static_cast<std::uint32_t>(position_type::player1)].amount_loaded += section->size;
            this->position_entries[static_cast<std::uint32_t>(position_type::player1)].loaded_section_count++;
        }

        if ((visibility & 2) != 0)
        {
            this->position_entries[static_cast<std::uint32_t>(position_type::player2)].amount_loaded += section->size;
            this->position_entries[static_cast<std::uint32_t>(position_type::player2)].loaded_section_count++;
        }

        this->calculate_loading_backlog();
    }

    void streamer::service_game_state()
    {
        std::uint16_t zones[2]{};

        if (!this->zone_switching_disabled && !this->memory_heap.is_empty() && this->determine_current_zones(zones))
        {
            this->switch_zones(zones);
        }

        this->amount_not_rendered = 0u;

        for (std::uint32_t i = 0u; i < this->current_section_count; ++i)
        {
            streamer::section* section = this->current_sections[i];

            if (!section->was_rendered && game_provider::is_regular_section_number(section->number))
            {
                this->amount_not_rendered += section->size;
            }

            section->was_rendered = false;
        }
    }

    void streamer::service_non_game_state()
    {
        this->handle_loading();
    }

    void streamer::set_streaming_position(position_type type, const vector3& position)
    {
        position_entry& entry = this->position_entries[static_cast<std::uint32_t>(type)];

        entry.position = position;
        entry.velocity = vector3::zero();
        entry.direction = vector3::zero();

        entry.position2D = position.as_vector2();
        entry.velocity2D = vector2::zero();
        entry.direction2D = vector2::zero();

        entry.position_set = true;
        entry.following_car = false;
        entry.predicted_zone_valid_time = -1;
        entry.predicted_zone = 0u;

        this->current_zone_needs_refreshing = true;
    }

    void streamer::switch_zones(const std::uint16_t* zones)
    {
        call_function<void(__thiscall*)(streamer*, const std::uint16_t*)>(0x007A7F10)(this, zones);
    }




    void streamer::unactivate_section(streamer::section& section)
    {
        section.unactivated_frame_count = global::frame_counter;

        loader::unload_chunks(section.memory, section.loaded_size);

        this->activated_section_count--;

        section.status = section::status_type::loaded;

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

    bool streamer::unloader(chunk* block)
    {
        switch (block->id())
        {
            case block_id::track_streaming_sections:
                this->unload_everything();
                this->sections = nullptr;
                this->section_count = 0u;
                return true;

            case block_id::track_streaming_infos:
                this->the_info = nullptr;
                return true;

            case block_id::track_streaming_barriers:
                this->barriers = nullptr;
                this->barrier_count = 0u;
                return true;

            case block_id::track_streaming_discs:
                this->discs = nullptr;
                this->last_disc = nullptr;
                return true;

            default:
                return false;
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



    void streamer::disc_bundle_loaded_callback_static(void* param, bool failed)
    {
        assert(!failed);

        streamer::instance.disc_bundle_loaded_callback(reinterpret_cast<streamer::disc_bundle*>(param));
    }

    void streamer::section_loaded_callback_static(void* param, bool failed)
    {
        assert(!failed);

        streamer::instance.section_loaded_callback(reinterpret_cast<streamer::section*>(param));
    }


    /*
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
    void streamer::MakeSpaceInPool((int, void (*)(void*), void*))
    {
        // 007A8570
    }
    void streamer::StartLoadingSections((void))
    {
        // 007A5590
    }
    */
}
