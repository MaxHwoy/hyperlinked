#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/gameplay/dal.hpp>
#include <hyperlib/gameplay/g_race.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/streamer/parameter_map.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/drawing.hpp>

#define RANDOMIZE_FLARE_TIMINGS

namespace hyper
{
    array<rendering_model, 4096u> renderer::rendering_models(0x00AB2780);

    bool renderer::flare_pool_inited_ = false;

    std::uint32_t renderer::active_flare_count_ = 0u;

    bool renderer::active_flare_types_[4]{};

    float renderer::active_flare_times_[4]{};

    float renderer::active_flare_blink_[4] { 0.5f, 0.4f, 0.45f, 1.0f / 3.0f };

    std::uint32_t renderer::flare_texture_keys_[5]
    {
        hashing::bin("HEADLIGHTFLAREINNER"),
        hashing::bin("HEADLIGHTFLAREOUTER"),
        hashing::bin("HEADLIGHTGLOW"),
        hashing::bin("HEADLIGHTCATSEYE"),
        hashing::bin("LAMPPOSTFLARE"),
    };

    texture::e_texture renderer::flare_texture_infos_[5]{};

    array<flare::params*, static_cast<size_t>(flare::type::count) * 2u> renderer::flare_params_(0x00A6BF88);

    bitset<static_cast<size_t>(view_id::count)> renderer::flare_mask_{};

    bitset<static_cast<size_t>(view_id::count)> renderer::view_to_flare_({
        static_cast<size_t>(view_id::player1),
        static_cast<size_t>(view_id::player2),
        static_cast<size_t>(view_id::player1_rvm),
        static_cast<size_t>(view_id::player1_reflection),
        static_cast<size_t>(view_id::player2_reflection),
        static_cast<size_t>(view_id::env_z_pos),
        static_cast<size_t>(view_id::env_x_pos),
        static_cast<size_t>(view_id::env_z_neg),
        static_cast<size_t>(view_id::env_x_neg),
        static_cast<size_t>(view_id::env_y_pos),
        static_cast<size_t>(view_id::env_y_neg),
    });

    auto renderer::create_flare_view_mask(view_id id) -> std::uint32_t
    {
        return (1u << static_cast<std::uint32_t>(id)) | ((id == view_id::player1) * 0x50u) | ((id == view_id::player2) * 0x80u);
    }

    bool renderer::can_render_flares_in_view(view_id id)
    {
        return renderer::view_to_flare_.get(id);
    }

    bool renderer::is_friend_flare_view_already_committed(view_id id)
    {
        renderer::flare_mask_.set(id, true);

        switch (id)
        {
            case view_id::player1:
                return renderer::flare_mask_.get(view_id::player1_reflection);

            case view_id::player2:
                return renderer::flare_mask_.get(view_id::player2_reflection);

            case view_id::player1_reflection:
                return renderer::flare_mask_.get(view_id::player1);

            case view_id::player2_reflection:
                return renderer::flare_mask_.get(view_id::player2);

            default:
                return false;
        }
    }

    auto renderer::get_next_light_flare_in_pool(std::uint32_t mask) -> flare::instance*
    {
        std::uint32_t& count = renderer::active_flare_count_;

        if (count < std::size(renderer::flare_pool_))
        {
            renderer::flare_bits_[count] = mask;

            return &renderer::flare_pool_[count++];
        }

        return nullptr;
    }

    void renderer::remove_current_light_flare_in_pool()
    {
        --renderer::active_flare_count_;
    }

    void renderer::init_light_flare_pool()
    {
        if (!renderer::flare_pool_inited_)
        {
            renderer::flare_pool_inited_ = true;

            for (std::uint32_t i = 0u; i < std::size(renderer::flare_texture_infos_); ++i)
            {
                texture::set_e_texture_key(renderer::flare_texture_infos_[i], renderer::flare_texture_keys_[i]);
            }

            for (std::uint32_t i = 0u; i < std::size(renderer::flare_pool_); ++i)
            {
                flare::instance& instance = renderer::flare_pool_[i];

                instance.prev() = nullptr;
                instance.next() = nullptr;
                instance.key = 0u;
                instance.section_number = 0u;
                instance.tint = color32::clear();
                instance.position = vector3(0.0f, 0.0f, 0.0f);
                instance.direction = vector3(0.0f, 0.0f, -1.0f);
                instance.flags = static_cast<flare::flags>(0u);
                instance.type = static_cast<flare::type>(0u);
                instance.reflect_pos_z = 999.0f;
            }
        }
    }

    void renderer::reset_light_flare_pool()
    {
        for (std::uint32_t i = 0u; i < std::size(renderer::active_flare_times_); ++i)
        {
            float& time = renderer::active_flare_times_[i];

            time += renderer::world_time_elapsed;

            if (time > renderer::active_flare_blink_[i])
            {
                time = 0.0f;

                renderer::active_flare_types_[i] = !renderer::active_flare_types_[i];
            }
        }

        renderer::flare_mask_.clear();
        renderer::active_flare_count_ = 0u;
    }

    void renderer::render_light_flare_pool(const view::instance* view)
    {
        if (renderer::flare_pool_off || view == nullptr)
        {
            return;
        }

        view_id id = view->id;

        std::uint32_t mask = 1u << static_cast<std::uint32_t>(id);

        const camera* camera = view->camera;

        float scaling = math::clamp((camera->velocity_key.position.magnitude() - 30.0f) * 0.02f, 0.0f, 1.0f);

        float horiz_scaling = scaling * scaling;

        for (std::uint32_t i = 0u; i < renderer::active_flare_count_; ++i)
        {
            flare::instance& flare = renderer::flare_pool_[i];

            bool can_render = true;

            bool not_glow = id != view_id::player1_glow && id != view_id::player2_glow;

            switch (flare.type)
            {
                case flare::type::catseye_red:
                    can_render = not_glow && ((g_race::status::is_racing() && !g_race::status::is_checkpoint() && !g_race::status::is_drift()) || g_race::status::is_in_pursuit());
                    break;

                case flare::type::catseye_orange:
                    can_render = not_glow;
                    break;

                case flare::type::generic_2:
                    can_render = *reinterpret_cast<bool*>(0x00B42F04);
                    break;

                case flare::type::blinking_red:
                    can_render = not_glow && renderer::active_flare_types_[0];
                    break;

                case flare::type::blinking_green:
                    can_render = not_glow && renderer::active_flare_types_[2];
                    break;

#if defined(RANDOMIZE_FLARE_TIMINGS)
                case flare::type::blinking_amber:
                    can_render = not_glow && renderer::active_flare_types_[static_cast<std::uint32_t>(::fabs(flare.position.x)) & 0x03u];
                    break;

                case flare::type::generic_8:
                    can_render = not_glow && renderer::active_flare_types_[static_cast<std::uint32_t>(::fabs(flare.position.x)) & 0x03u];
                    break;
#else
                case flare::type::blinking_amber:
                    can_render = not_glow && renderer::active_flare_types_[1];
                    break;

                case flare::type::generic_8:
                    can_render = not_glow && renderer::active_flare_types_[2];
                    break;
#endif
            }

            if (can_render && (renderer::flare_bits_[i] & mask) != 0)
            {
                float intensity = flare.type == flare::type::sun_flare ? view::instance::views[view_id::player1].get_flare_intensity() : 1.0f;

                switch (id)
                {
                    case view_id::player1_rvm:
                    case view_id::env_z_pos:
                    case view_id::env_x_pos:
                    case view_id::env_z_neg:
                    case view_id::env_x_neg:
                    case view_id::env_y_pos:
                    case view_id::env_y_neg:
                        renderer::render_light_flare(view, flare, nullptr, intensity, flare::reflection::none, flare::render::env, horiz_scaling, 0.0f, flare.tint, 1.0f);
                        break;

                    case view_id::player1_reflection:
                    case view_id::player2_reflection:
                        renderer::render_light_flare(view, flare, nullptr, intensity, flare::reflection::fast, flare::render::refl, horiz_scaling, 0.0f, flare.tint, 1.0f);
                        break;

                    default:
                        renderer::render_light_flare(view, flare, nullptr, 1.0f, flare::reflection::none, flare::render::norm, horiz_scaling, 0.0f, flare.tint, 1.0f);
                        break;
                }
            }
        }
    }

    void renderer::render_light_flare(const view::instance* view, flare::instance& flare, const matrix4x4* local_world, float intensity_scale, flare::reflection refl_type, flare::render render_type, float horizontal_flare_scale, float reflection_override, color32 color_override, float size_scale)
    {
        if (renderer::draw_light_flares && view != nullptr)
        {
            float dampness = 0.0f;

            const rain* rain = view->rain;

            if (rain != nullptr)
            {
                dampness = rain->road_dampness;
            }

            float extend_factor = 0.0f;

            if (render_type == flare::render::refl)
            {
                extend_factor = (10.0f - 1.2f) * dampness + 1.2f;
            }

            view_id id = view->id;

            const camera* camera = view->camera;

            flare::type type = flare.type;
            flare::flags flag = flare.flags;

            vector3 position(flare.position);
            vector3 direction(flare.direction);

            if (local_world != nullptr)
            {
                math::transform_point(*local_world, position);
            }

            if (id == view_id::player1_reflection || id == view_id::player2_reflection)
            {
                if (reflection_override == 0.0f)
                {
                    if (flare.reflect_pos_z == 999.0f || refl_type == flare::reflection::topo)
                    {
                        if (camera->current_key.position.z - 10.0f > position.z)
                        {
                            return;
                        }

                        if (game_flow::manager::instance.current_state == game_flow::state::racing)
                        {
                            float height = 0.0f;

                            vector3 real3d_pos(position);

                            space_settings space(space_axis::ny, space_axis::pz, space_axis::px);

                            space.swap(real3d_pos);

                            collision::manager caster(0u, 3u);

                            if (!caster.get_world_height_at_point_rigorous(real3d_pos, height, nullptr))
                            {
                                return; // actual return, game does not do the ground hit check for no reason, hyperclap blackbox
                            }

                            flare.reflect_pos_z = height + height - position.z; // reflect relative to road
                        }
                        else
                        {
                            flare.reflect_pos_z = position.z;
                        }
                    }
                }
                else
                {
                    if (camera->current_key.position.z - 10.0f > position.z)
                    {
                        return;
                    }

                    flare.reflect_pos_z = reflection_override - (position.z - reflection_override);
                }
            }

            if (render_type == flare::render::refl)
            {
                if (flare.reflect_pos_z == 999.0f || flare.reflect_pos_z == 9999.0f)
                {
                    return;
                }

                position.z = flare.reflect_pos_z;
                
                math::flip_sign(direction.z);

                bounds bbox(position, extend_factor);

                if (view->get_visible_state_sb(bbox) == visible_state::outside)
                {
                    return;
                }
            }
            else
            {
                if (render_type == flare::render::env)
                {
                    bounds bbox(position, 2.0f);

                    if (view->get_visible_state_sb(bbox) == visible_state::outside)
                    {
                        return;
                    }
                }
                else
                {
                    if (view->get_visible_state_sb(position, nullptr) == visible_state::outside)
                    {
                        return;
                    }
                }
            }

            if ((type >= flare::type::catseye_orange && type <= flare::type::catseye_blue) ||
                (type >= flare::type::generic_1 && type <= flare::type::generic_7) ||
                (type == flare::type::generic_9))
            {
                if (id != view_id::player1 && id != view_id::player2)
                {
                    return;
                }

                camera_mover* mover = view->get_camera_mover();

                if (mover != nullptr)
                {
                    const camera_anchor* anchor = mover->get_anchor();

                    if (anchor != nullptr)
                    {
                        vector3 to_flare_dir = (flare.position - anchor->position).normalized();

                        float dot_product = vector3::dot(to_flare_dir, anchor->rotation.row(0u).as_vector3());

                        const float rain_in_headlights = 0.96f;

                        if (dot_product < rain_in_headlights)
                        {
                            return;
                        }

                        float mul_1st = (dot_product - rain_in_headlights) / (1.0f - rain_in_headlights);

                        float mul_2nd = math::max(vector3::dot(camera->current_key.direction, anchor->rotation.row(0u).as_vector3()), 0.0f);

                        intensity_scale = mul_1st * mul_2nd;
                    }
                }
            }

            vector3 to_camera_dir = (camera->current_key.position - position);

            float to_camera_dist2 = to_camera_dir.sqr_magnitude();

            float min_dist = 30000.0f;
            float max_dist = 40000.0f;

            if (render_type == flare::render::env)
            {
                min_dist = 7000.0f;
                max_dist = 10000.0f;

                intensity_scale *= 1.5f;
                size_scale *= 2.0f;
            }

            if (to_camera_dist2 <= max_dist)
            {
                float alpha_factor = 1.0f;

                if (min_dist < to_camera_dist2)
                {
                    alpha_factor -= (to_camera_dist2 - min_dist) / (max_dist - min_dist);
                }

                if (local_world != nullptr)
                {
                    math::transform_vector(*local_world, direction);
                }

                to_camera_dir = to_camera_dir.normalized();

                float dot_product = vector3::dot(direction, to_camera_dir);

                if ((flag & flare::flags::bi_directional) != 0)
                {
                    dot_product = ::fabs(dot_product);
                }
                else
                {
                    dot_product = math::max(dot_product, 0.0f);
                }

                float pixelation = view->pixelation;

                if (id == view_id::player1 || id == view_id::player2)
                {
                    pixelation = 360.0f;
                }

                vector3 world_pos(position);

                const matrix4x4& camera_trs = camera->current_key.matrix;

                math::transform_point(camera_trs, world_pos);

                float flare_pixels = (1.0f / pixelation) * world_pos.z;

                for (std::uint32_t i = 0u; i < 2u; ++i)
                {
                    const flare::params* params = renderer::flare_params_[static_cast<std::uint32_t>(type) * 2u + i];

                    if (params != nullptr)
                    {
                        vector3 camera_right;
                        vector3 camera_up;
                        vector3 camera_forward;

                        camera_trs.column(0u, camera_right);
                        camera_trs.column(1u, camera_up);
                        camera_trs.column(2u, camera_forward);

                        std::uint32_t tex_id = params->texture_id;

                        if (render_type == flare::render::refl)
                        {
                            tex_id = 2u; // HEADLIGHTGLOW forced (for now)
                        }

                        const texture::info* texture = renderer::flare_texture_infos_[tex_id].texture;

                        if (texture != nullptr)
                        {
                            float intensity = intensity_scale;

                            if ((flag & (flare::flags::uni_directional | flare::flags::bi_directional)) != 0)
                            {
                                intensity *= ::pow(dot_product, params->power);

                                intensity = math::min(intensity, 1.0f);
                            }

                            float flare_size = math::lerp(params->min_size, params->max_size, intensity);

                            if (flare_size > 0.0f)
                            {
                                const void* nis_instance = *reinterpret_cast<const void**>(0x00B4D964);

                                vector3 true_flare_pos;

                                if (nis_instance == nullptr)
                                {
                                    true_flare_pos = position + to_camera_dir * params->z_bias;
                                }
                                else
                                {
                                    true_flare_pos = position + to_camera_dir * (params->z_bias * 0.5f); // for NIS half z offset
                                }

                                flare_size *= math::clamp(flare_pixels, params->min_scale, params->max_scale) * size_scale;

                                color32 flare_color;

                                if (color_override == color32::clear())
                                {
                                    color float_color;

                                    if ((type == flare::type::lamppost || type == flare::type::generic_10) && parameter_map::accessor::flare_override_accessor.layer != nullptr)
                                    {
                                        parameter_map::accessor::flare_override_accessor.capture_data(true_flare_pos.x, true_flare_pos.y);

                                        float_color.b = parameter_map::accessor::flare_override_accessor.get_float(0u) * intensity;
                                        float_color.g = parameter_map::accessor::flare_override_accessor.get_float(1u) * intensity;
                                        float_color.r = parameter_map::accessor::flare_override_accessor.get_float(2u) * intensity;
                                        float_color.a = parameter_map::accessor::flare_override_accessor.get_float(3u) * intensity * alpha_factor;

                                        if (float_color.b < 0.0f)
                                        {
                                            float_color.b = params->max_colour.r * intensity;
                                            float_color.g = params->max_colour.g * intensity;
                                            float_color.r = params->max_colour.b * intensity;
                                            float_color.a = params->max_colour.a * intensity * alpha_factor;
                                        }
                                    }
                                    else
                                    {
                                        float_color.b = params->max_colour.r * intensity;
                                        float_color.g = params->max_colour.g * intensity;
                                        float_color.r = params->max_colour.b * intensity;
                                        float_color.a = params->max_colour.a * intensity * alpha_factor;
                                    }

                                    flare_color.r = static_cast<std::uint8_t>(math::min(float_color.r + float_color.r, 255.0f));
                                    flare_color.g = static_cast<std::uint8_t>(math::min(float_color.g + float_color.g, 255.0f));
                                    flare_color.b = static_cast<std::uint8_t>(math::min(float_color.b + float_color.b, 255.0f));
                                    flare_color.a = static_cast<std::uint8_t>(math::min(float_color.a + float_color.a, 255.0f));
                                }
                                else
                                {
                                    flare_color.r = color_override.r;
                                    flare_color.g = color_override.g;
                                    flare_color.b = color_override.b;
                                    flare_color.a = static_cast<std::uint8_t>(math::min(params->max_colour.a * intensity * alpha_factor * 2.0f, 255.0f));
                                }

                                float degree_angle = 0.0f;

                                if (render_type == flare::render::norm)
                                {
                                    vector3 screen_pos;

                                    view->get_screen_position(true_flare_pos, screen_pos);

                                    degree_angle = screen_pos.x / static_cast<float>(directx::resolution_x) * 240.0f;
                                }

                                float horizontal_scale = flare_size;
                                float vertical_scale = flare_size;

                                if (render_type == flare::render::refl)
                                {
                                    if (flare.position.z - flare.reflect_pos_z >= 4.0f)
                                    {
                                        vertical_scale *= extend_factor;
                                    }
                                    else
                                    {
                                        vertical_scale *= extend_factor * 0.5f;
                                    }
                                }

                                if (horizontal_flare_scale > 0.0f)
                                {
                                    // horizontal_scale += horizontal_scale * horizontal_flare_scale;
                                }

                                streak::manager::instance.commit_flare(true_flare_pos, texture, type, flare_color, horizontal_scale, vertical_scale, degree_angle);
                            }
                        }
                    }
                }
            }
        }
    }
}
