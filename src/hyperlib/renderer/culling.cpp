#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/streamer/sections.hpp>
#include <hyperlib/renderer/culling.hpp>

namespace hyper
{
    enum class game_flow_state : std::uint32_t
    {
        none = 0x0,
        loading_frontend = 0x1,
        unloading_frontend = 0x2,
        in_frontend = 0x3,
        loading_region = 0x4,
        loading_track = 0x5,
        racing = 0x6,
        unloading_track = 0x7,
        unloading_region = 0x8,
        exit_demo_disc = 0x9,
    };

    struct game_flow_manager
    {
        void(__cdecl* single_function)(std::int32_t);
        std::int32_t single_function_param;
        const char* single_function_name;
        void(__cdecl* looping_function)();
        const char* looping_function_name;
        bool waiting_for_callback;
        const char* callback_name;
        int callback_phase;
        game_flow_state current_game_flow_state;
    };

    ASSERT_SIZE(game_flow_manager, 0x24);

    void grand_scenery_cull_info::setup_scenery_cull_info(view* view, instance_flags flags)
    {
        view_mode mode = renderer::mode;

        scenery_cull_info& cull_info = this->scenery_cull_infos[this->cull_info_count++];

        flags |= instance_flags::exclude_disable_rendering;

        switch (view->id)
        {
            case view_id::player1:
            case view_id::player2:
                flags |= instance_flags::enable_wind | instance_flags::exclude_main_view;
                break;

            case view_id::player1_rvm:
                flags |= instance_flags::include_rear_view | instance_flags::environment_map;
                break;

            case view_id::player1_pip:
                flags |= instance_flags::include_rear_view;
                break;

            case view_id::player1_reflection:
            case view_id::player2_reflection:
                flags |= instance_flags::include_reflection_ng | instance_flags::reflection | instance_flags::include_reflection;
                break;

            case view_id::player1_shadowmap:
            case view_id::player2_shadowmap:
                flags |= instance_flags::enable_wind | instance_flags::envmap_shadow;
                break;

            case view_id::env_x_pos:
            case view_id::env_x_neg:
            case view_id::env_y_pos:
            case view_id::env_y_neg:
            case view_id::env_z_pos:
            case view_id::env_z_neg:
                flags |= instance_flags::include_reflection_ng | instance_flags::include_reflection | instance_flags::envmap_shadow | instance_flags::environment_map;
                break;
        }

        if (mode == view_mode::two_h || mode == view_mode::two_v)
        {
            flags |= instance_flags::exclude_split_screen;
        }

        char* g_race_status = *reinterpret_cast<char**>(0x00A98284);

        if (g_race_status != nullptr && g_race_status[0x6A14] == 1)
        {
            flags |= instance_flags::exclude_racing;
        }
        else
        {
            flags |= instance_flags::exclude_freeroam;
        }

        cull_info.view = view;
        cull_info.flags = flags;
    }

    void grand_scenery_cull_info::setup_world_culling()
    {
        this->cull_info_count = 0u;

        game_flow_state state = reinterpret_cast<game_flow_manager*>(0x00A99B9C)->current_game_flow_state;

        if (renderer::draw_world && state != game_flow_state::loading_region && state != game_flow_state::loading_track)
        {
            for (std::uint32_t i = static_cast<std::uint32_t>(view_id::player1); i <= static_cast<std::uint32_t>(view_id::player1_rvm); ++i)
            {
                view* current = view::views[i];

                if (current->active && current->attached_target->active)
                {
                    this->setup_scenery_cull_info(current, static_cast<instance_flags>(0u));
                }
            }

            for (std::uint32_t i = static_cast<std::uint32_t>(view_id::env_z_pos); i <= static_cast<std::uint32_t>(view_id::env_y_neg); ++i)
            {
                view* current = view::views[i];

                if (current->active && current->attached_target->active)
                {
                    if (renderer::shadow_detail >= 2u)
                    {
                        this->setup_scenery_cull_info(current, instance_flags::environment_map);
                    }
                    else
                    {
                        this->setup_scenery_cull_info(current, instance_flags::environment_map | instance_flags::envmap_shadow);
                    }
                }
            }

            view* reflection = view::views[view_id::player1_reflection];

            if (renderer::road_reflection_enabled && reflection->rain->road_dampness >= 0.01f && reflection->active && reflection->attached_target->active)
            {
                this->setup_scenery_cull_info(reflection, static_cast<instance_flags>(0u));
            }

            view* pip = view::views[view_id::player1_pip];

            if (pip->active && pip->attached_target->active)
            {
                this->setup_scenery_cull_info(reflection, static_cast<instance_flags>(0u));
            }

            this->do_culling();
        }
    }

    void grand_scenery_cull_info::do_culling()
    {
        this->first_draw_info = reinterpret_cast<scenery_draw_info*>(0x00B5A4F0);
        this->current_draw_info = reinterpret_cast<scenery_draw_info*>(0x00B5A4F0);
        this->top_draw_info = reinterpret_cast<scenery_draw_info*>(0x00B68F50);

        for (std::uint32_t i = 0u; i < this->cull_info_count; ++i)
        {
            scenery_cull_info& cull_info = this->scenery_cull_infos[i];

            cull_info.position = cull_info.view->camera->current_key.position;
            cull_info.direction = cull_info.view->camera->current_key.direction;
            cull_info.pixelation = cull_info.view->pixelation;
            cull_info.preculler_section_number = -1;
        }

        for (std::uint32_t i = 0u; i < this->cull_info_count; ++i)
        {
            scenery_cull_info& cull_info = this->scenery_cull_infos[i];

            cull_info.first_draw_info = this->current_draw_info;
            cull_info.current_draw_info = this->current_draw_info;
            cull_info.top_draw_info = this->top_draw_info;

            this->cull_view(cull_info);

            this->current_draw_info = cull_info.current_draw_info;
        }
    }

    void grand_scenery_cull_info::cull_view(scenery_cull_info& cull_info)
    {
        const std::uint32_t max_sections = 0x80;

        std::uint16_t sections[max_sections];

        std::uint32_t section_count = this->what_sections_should_we_draw(sections, max_sections, cull_info);

        for (std::uint32_t i = 0u; i < section_count; ++i)
        {
            visible_section::user_info* user_info = visible_section::manager::instance->user_infos[sections[i]];

            if (user_info != nullptr)
            {
                scenery::pack* pack = user_info->scenery;

                if (pack != nullptr)
                {
                    if (pack->tree_nodes)
                    {
                        this->tree_cull(*pack, cull_info);
                    }
                    else
                    {
                        std::uint32_t count = pack->instance_count;

                        for (std::uint32_t k = 0; k < count; ++k)
                        {
                            this->draw_a_scenery(*pack, k, cull_info, visible_state::partial);
                        }
                    }
                }
            }
        }
    }

    auto grand_scenery_cull_info::what_sections_should_we_draw(std::uint16_t* sections, std::uint32_t section_count, scenery_cull_info& cull_info) -> std::uint32_t
    {
        view_id id = cull_info.view->id;

        const visible_section::drivable* drivable = nullptr;

        if (id == view_id::player1_shadowmap || id == view_id::player2_shadowmap)
        {
            id = static_cast<view_id>(static_cast<std::uint32_t>(id) - static_cast<std::uint32_t>(view_id::player1_shadowmap) + static_cast<std::uint32_t>(view_id::player1));

            const vector3& position = view::views[id]->camera->current_key.position;

            drivable = visible_section::manager::instance->find_drivable_section(position);
        }
        else
        {
            drivable = visible_section::manager::instance->find_drivable_section(cull_info.position);
        }

        if (id == view_id::player1)
        {
            if (drivable == nullptr)
            {
                visible_section::manager::current_zone_number = std::numeric_limits<std::uint32_t>::max();
            }
            else
            {
                visible_section::manager::current_zone_number = drivable->section_number;
            }
        }

        std::uint32_t count = 1u;

        sections[0] = game_provider::shared_scenery_section;

        if (drivable != nullptr)
        {
            section_count = math::min(section_count, static_cast<std::uint32_t>(drivable->visible_section_count));

            ::memcpy(sections + 1u, drivable->visible_sections, section_count * sizeof(std::uint16_t));

            count += section_count;
        }

        return count;
    }

    void grand_scenery_cull_info::tree_cull(const scenery::pack& pack, scenery_cull_info& cull_info)
    {
        const std::uint32_t max_stack = 0x200;

        visible_state state[max_stack];
        std::uint16_t stack[max_stack];

        scenery::tree_node* tree = pack.tree_nodes;

        stack[0] = 0u;
        state[0] = visible_state::partial;

        std::uint32_t top = 1u;

        while (top > 0u)
        {
            top--;

            scenery::tree_node* current = stack[top] + tree;

            visible_state visible = state[top];

            if (visible == visible_state::partial)
            {
                visible = cull_info.view->get_visible_state_sb(current->bbox_min, current->bbox_max, nullptr);
            }

            if (visible != visible_state::outside)
            {
                std::uint32_t count = static_cast<std::uint32_t>(current->child_count);

                for (std::uint32_t i = 0u; i < count; ++i)
                {
                    std::int16_t index = current->children[i];

                    if (index < 0)
                    {
                        assert(top < max_stack);

                        stack[top] = static_cast<std::uint16_t>(-index);
                        state[top] = visible;

                        top++;
                    }
                    else
                    {
                        this->draw_a_scenery(pack, static_cast<std::int32_t>(index), cull_info, visible);
                    }
                }
            }
        }
    }

    void grand_scenery_cull_info::draw_a_scenery(const scenery::pack& pack, std::uint32_t instance_index, scenery_cull_info& cull_info, visible_state state)
    {
        reinterpret_cast<void(__thiscall*)(const scenery::pack*, std::uint32_t, scenery_cull_info*, visible_state)>(0x0079FA60)(&pack, instance_index, &cull_info, state);
    }
}
