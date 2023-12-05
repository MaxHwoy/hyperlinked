#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/fog_renderer.hpp>
#include <hyperlib/renderer/sky_renderer.hpp>
#include <hyperlib/renderer/light_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlib/renderer/vehicle_render_conn.hpp>
#include <hyperlib/renderer/per_view/pip_renderer.hpp>

namespace hyper
{
    void pip_renderer::render(const view::instance& pip_view, const grand_scenery_cull_info& culler)
    {
        if (pip_view.active)
        {
            if (render_target* target = pip_view.attached_target)
            {
                if (target->active && target->d3d_target && target->d3d_depth_stencil)
                {
                    BENCHMARK();

                    renderer::set_render_target(*target, true, color32::clear());

                    fog_renderer::assign_parameters(fog_renderer::params::instance, view::instance::views[view_id::player1]);

                    sky_renderer::render(pip_view, sky_renderer::view_type::rvm_pip_env, light_renderer::default_ingame_light_y);

                    culler.stuff_scenery(pip_view, prepass_flags::add_draw_flag_0x1000);

                    vehicle_render_conn::render(pip_view, false);

                    world_renderer::render();
                }
            }
        }
    }
}
