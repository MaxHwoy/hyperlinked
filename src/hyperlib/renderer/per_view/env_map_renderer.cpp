#include <hyperlib/global_vars.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/time_of_day.hpp>
#include <hyperlib/renderer/sky_renderer.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/flare_renderer.hpp>
#include <hyperlib/renderer/light_renderer.hpp>
#include <hyperlib/renderer/model_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlib/renderer/vehicle_render_conn.hpp>
#include <hyperlib/renderer/per_view/env_map_renderer.hpp>

namespace hyper
{
    void env_map_renderer::render(const view::instance& env_map_view, const grand_scenery_cull_info& culler)
    {
        if (env_map_view.active)
        {
            if (render_target* target = env_map_view.attached_target)
            {
                if (target->active && target->d3d_target && target->d3d_depth_stencil)
                {
                    BENCHMARK();

                    world_renderer::use_lowlod_pass = true;

                    renderer::current_cull_mode = ::D3DCULL_CCW;

                    renderer::set_render_target(*target, true, color32::clear());

                    time_of_day::instance->update(env_map_view.rain->cloud_intensity);

                    directx::set_z_write_enable(false, false);

                    for (std::uint32_t i = 0u; i < 8u; ++i)
                    {
                        directx::device()->SetTexture(i, nullptr);
                    }

                    if (game_flow::manager::instance.current_state == game_flow::state::racing)
                    {
                        sky_renderer::render(env_map_view, sky_renderer::view_type::rvm_pip_env, 0.6f);
                    }

                    light_renderer::default_ingame_light_y = 1.25f;

                    directx::set_z_write_enable(true, true);

                    world_renderer::render();

                    culler.stuff_scenery(env_map_view, prepass_flags::add_draw_flag_0x1000);

                    world_renderer::render();

                    if (options::flares_and_streaks_enabled)
                    {
                        flare_renderer::instance.lock();

                        flare_renderer::submit_world_flares(env_map_view, flare::render::env);

                        flare_renderer::submit_car_flares(env_map_view, false);

                        flare_renderer::submit_pool_flares(env_map_view);

                        flare_renderer::instance.unlock();

                        flare_renderer::instance.render(render_view::views[env_map_view.id], global::fac_flush == 0u);
                    }

                    light_renderer::default_ingame_light_y = 1.0f;

                    renderer::current_cull_mode = ::D3DCULL_CW;

                    world_renderer::use_lowlod_pass = false;
                }
            }
        }
    }
}
