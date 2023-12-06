#include <hyperlib/ui/feng_hud.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/sky_renderer.hpp>
#include <hyperlib/renderer/flare_renderer.hpp>
#include <hyperlib/renderer/light_renderer.hpp>
#include <hyperlib/renderer/model_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlib/renderer/vehicle_render_conn.hpp>
#include <hyperlib/renderer/per_view/rvm_renderer.hpp>

namespace hyper
{
    void rvm_renderer::render(const view::instance& rvm_view, const grand_scenery_cull_info& culler)
    {
        if (feng_hud::should_rvm_be_visible() && rvm_view.active)
        {
            if (render_target* target = rvm_view.attached_target)
            {
                if (target->active && target->d3d_target && target->d3d_depth_stencil)
                {
                    BENCHMARK();

                    world_renderer::use_lowlod_pass = true;

                    light_renderer::default_ingame_light_y = 1.0f;

                    renderer::set_render_target(*target, true, color32::clear());

                    sky_renderer::render(rvm_view, sky_renderer::view_type::rvm_pip_env, 2.0f);

                    culler.stuff_scenery(rvm_view, prepass_flags::add_draw_flag_0x1000 | prepass_flags::include_rear_view);

                    world_renderer::render();

                    vehicle_render_conn::render(rvm_view, false);

                    world_renderer::render();

                    model_renderer::render(rvm_view, static_cast<instance_flags>(0u));

                    world_renderer::render();

                    flare_renderer::instance.lock();

                    flare_renderer::submit_world_flares(rvm_view, flare::render::env);

                    flare_renderer::submit_car_flares(rvm_view, false);

                    flare_renderer::submit_pool_flares(rvm_view);

                    flare_renderer::instance.unlock();

                    flare_renderer::instance.render(render_view::views[rvm_view.id], false);

                    world_renderer::use_lowlod_pass = false;
                }
            }
        }
    }
}
