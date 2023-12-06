#include <hyperlib/options.hpp>
#include <hyperlib/global_vars.hpp>
#include <hyperlib/memory/frame_pool.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/sky_renderer.hpp>
#include <hyperlib/renderer/light_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>

namespace hyper
{
    void sky_renderer::draw_skydome(const view::instance& view, view_type type)
    {
        if (type != view_type::none && options::sky_enabled)
        {
            if (game_flow::manager::instance.current_state == game_flow::state::loading_region ||
                game_flow::manager::instance.current_state == game_flow::state::loading_track)
            {
                return;
            }

            if (!sky_renderer::view_type_to_sky_enabled_table_[type])
            {
                return;
            }

            matrix4x4* transform = frame_pool::instance.malloc<matrix4x4>();

            if (transform != nullptr)
            {
                *transform = matrix4x4::identity();

                float size_scale = 1.0f;

                float z_position = 0.0f;

                bool rotate_sky = false;

                switch (type)
                {
                    case view_type::player:
                        size_scale = 1.0f;
                        break;

                    case view_type::reflection:
                        math::flip_sign(transform->m33);
                        size_scale = 0.06f;
                        rotate_sky = true;
                        z_position = 130.0f;
                        break;
                }

                switch (view.id)
                {
                    case view_id::player1_rvm:
                    case view_id::player1_pip:
                        size_scale = 0.015f;
                        break;

                    case view_id::env_z_pos:
                    case view_id::env_x_pos:
                    case view_id::env_z_neg:
                    case view_id::env_x_neg:
                    case view_id::env_y_pos:
                    case view_id::env_y_neg:
                        size_scale = 0.02f;
                        break;
                }

                transform->row(3u).as_vector3() = view.camera->current_key.position.as_vector3();

                if (view.id == view_id::player1 || view.id == view_id::player2 || view.id == view_id::player1_rvm)
                {
                    if (sky_renderer::modify_sky_in_main_views_)
                    {
                        transform->m11 *= size_scale * sky_renderer::sky_additive_scale_;
                        transform->m22 *= size_scale * sky_renderer::sky_additive_scale_;
                        transform->m33 *= size_scale * sky_renderer::sky_additive_scale_;
                        transform->m43 += z_position;

                        if (rotate_sky)
                        {
                            matrix4x4 rotation;

                            sky_renderer::sky_rotation_ = static_cast<std::uint16_t>(static_cast<std::int32_t>(global::world_time_elapsed * 300.0f) + sky_renderer::sky_rotation_);

                            math::create_rotation_z(sky_renderer::sky_rotation_, rotation);

                            math::multiply_matrix(rotation, *transform, *transform);
                        }
                    }
                }
                else
                {
                    transform->m11 *= size_scale;
                    transform->m22 *= size_scale;
                    transform->m33 *= size_scale;
                    transform->m43 += z_position;
                }

                view.render(sky_renderer::skydome, transform, nullptr, draw_flags::sky_shade, nullptr, nullptr);
            }
        }
    }

    void sky_renderer::render(const view::instance& view, view_type type, float lighting_scale)
    {
        float previous = light_renderer::default_ingame_light_y;

        light_renderer::default_ingame_light_y = lighting_scale;

        sky_renderer::draw_skydome(view, type);

        world_renderer::render();

        light_renderer::default_ingame_light_y = previous;
    }
}
