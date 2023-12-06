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
#include <hyperlib/renderer/per_view/reflection_renderer.hpp>

namespace hyper
{
    void reflection_renderer::render(const view::instance& reflection_view, const view::instance& player_view, const grand_scenery_cull_info& culler)
    {
        if (reflection_view.active)
        {
            if (render_target* target = reflection_view.attached_target)
            {
                if (target->active && target->d3d_target && target->d3d_depth_stencil)
                {
                    BENCHMARK();

                    world_renderer::use_lowlod_pass = true;

                    renderer::current_cull_mode = ::D3DCULL_CCW;

                    renderer::set_render_target(*target, true, color32::clear());

                    time_of_day::instance->update(reflection_view.rain->cloud_intensity);

                    if (options::road_reflection_detail >= 2u && player_view.rain->road_dampness >= 0.01f)
                    {
                        culler.stuff_scenery(reflection_view, prepass_flags::exclude_chopped_roads);

                        world_renderer::render();
                    }

                    if (options::flares_and_streaks_enabled && options::texture_blend_supported && effect_screen_effect::instance != nullptr)
                    {
                        screen_effect& screen = screen_effect::instance;

                        flare_renderer::instance.lock();

                        flare_renderer::submit_world_flares(reflection_view, flare::render::refl);

                        flare_renderer::submit_pool_flares(reflection_view);

                        flare_renderer::instance.unlock();

                        flare_renderer::instance.render(render_view::views[reflection_view.id], global::fac_flush == 0u);

                        effect_screen_effect& effect = *effect_screen_effect::instance;
                        
                        screen_effect::downsample_4x4_texture(effect, reflection_render_target::d3d_texture, screen.acquire_surface_1(screen_effect::downscale::d4x4), nullptr);

                        screen_effect::set_gaussian_kernals(0.0f, -6.0f, 0.0f, 2.0f, 9.0f);

                        screen_effect::multipass_gauss_blur(effect, screen.acquire_texture_1(screen_effect::downscale::d4x4), false, screen.acquire_surface_2(screen_effect::downscale::d4x4), 1.0f);

                        screen_effect::set_gaussian_kernals(0.0f, 0.0f, 1.0f, 1.0f, 3.0f);

                        directx::device()->SetRenderTarget(0u, screen.acquire_surface_2(screen_effect::downscale::d2x2));

                        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

                        flare_renderer::instance.lock();

                        flare_renderer::submit_car_flares(reflection_view, true);

                        flare_renderer::instance.unlock();

                        flare_renderer::instance.render(render_view::views[reflection_view.id], global::fac_flush == 0u);

                        screen_effect::blend_textures(effect, target->d3d_target, screen.acquire_texture_2(screen_effect::downscale::d4x4), screen.acquire_texture_2(screen_effect::downscale::d2x2), 0.0f, 0.0f, "CombineReflectionColourAndHeadlightAlpha");
                    }

                    renderer::current_cull_mode = ::D3DCULL_CW;

                    world_renderer::use_lowlod_pass = false;
                }
            }
        }
    }
}
