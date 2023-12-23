#include <hyperlib/options.hpp>
#include <hyperlib/global_vars.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/gameplay/gps.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/time_of_day.hpp>
#include <hyperlib/renderer/depth_of_field.hpp>
#include <hyperlib/renderer/visual_treatment.hpp>
#include <hyperlib/renderer/post_process.hpp>
#include <hyperlib/renderer/anim_scene.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/fe_renderer.hpp>
#include <hyperlib/renderer/sky_renderer.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/skid_renderer.hpp>
#include <hyperlib/renderer/flare_renderer.hpp>
#include <hyperlib/renderer/light_renderer.hpp>
#include <hyperlib/renderer/model_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlib/renderer/streak_renderer.hpp>
#include <hyperlib/renderer/particle_renderer.hpp>
#include <hyperlib/renderer/vehicle_render_conn.hpp>
#include <hyperlib/renderer/per_view/main_view_renderer.hpp>

namespace hyper
{
    void main_view_renderer::render(const view::instance& main_view, const grand_scenery_cull_info& culler)
    {
        if (!main_view.active || main_view.attached_target == nullptr || !main_view.attached_target->active)
        {
            return;
        }

        BENCHMARK();

        if (options::motion_blur_enabled && motion_blur_render_target::instance().active)
        {
            renderer::set_render_target(motion_blur_render_target::instance(), true, color32::white());

            culler.stuff_scenery(main_view, prepass_flags::add_draw_flag_0x1000 | prepass_flags::include_for_blurring);

            model_renderer::render(main_view, culler.get_cull_info_flags(main_view));

            world_renderer::depth_prepass();
        }

        time_of_day::instance->update(main_view.rain->cloud_intensity);

        renderer::set_render_target(*main_view.attached_target, true, color32::clear());

        directx::reset_rendering_states();

        sky_renderer::render(main_view, sky_renderer::view_type::player, 2.0f);

        light_renderer::default_ingame_light_y = 1.0f;

        if (game_flow::manager::instance.current_state != game_flow::state::racing)
        {
            renderer::current_cull_mode = ::D3DCULL_CCW;

            vehicle_render_conn::render_fe(main_view, true);

            world_renderer::render();

            renderer::current_cull_mode = ::D3DCULL_CW;
        }

        culler.stuff_scenery(main_view, renderer::low_quality_flags | prepass_flags::exclude_non_main_view);

        world_renderer::render();

        model_renderer::render(main_view, culler.get_cull_info_flags(main_view));

        world_renderer::render();

        gps::render(main_view);

        world_renderer::render();

        if (options::motion_blur_enabled)
        {
            post_process::do_motion_blur(post_process::instance, *visual_treatment::instance, main_view.id);
        }

        if (options::visual_treatment && visual_treatment::instance->uves_over_cliff_enabled)
        {
            post_process::do_uves_over_cliff(post_process::instance, *visual_treatment::instance, main_view.id);
        }

        skid_renderer::render(main_view);

        if (game_flow::manager::instance.current_state == game_flow::state::racing)
        {
            vehicle_render_conn::render_world(main_view, false);

            if (depth_of_field::instance.active(main_view.id) && options::motion_blur_enabled)
            {
                world_renderer::render();

                renderer::set_render_target(motion_blur_render_target::instance(), false, color32::clear());

                vehicle_render_conn::render_world(main_view, false);

                world_renderer::depth_prepass();

                world_renderer::render();

                renderer::set_render_target(*main_view.attached_target, false, color32::clear());
            }
        }
        else
        {
            vehicle_render_conn::render_fe(main_view, false);
        }

        world_renderer::render();

        if (options::visual_treatment && visual_treatment::instance->uves_over_cliff_enabled)
        {
            post_process::do_uves_over_cliff_darken(post_process::instance, *visual_treatment::instance, main_view.id);
        }

        directx::reset_rendering_states();

        if (options::flares_and_streaks_enabled)
        {
            effect_world_prelit::instance->set_current_pass(0u);

            directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, false);

            flare_renderer::instance.lock();

            flare_renderer::submit_world_flares(main_view, flare::render::norm);

            flare_renderer::submit_car_flares(main_view, false);

            flare_renderer::submit_pool_flares(main_view);

            flare_renderer::instance.unlock();

            directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, true);

            effect_world_prelit::instance->finalize();

            flare_renderer::instance.render(render_view::views[main_view.id], global::fac_flush == 0u);
        }

        if (options::bone_animations_enabled)
        {
            effect_world::instance->set_current_pass(0u);

            anim_scene::render(main_view, 0u);

            effect_world::instance->finalize();
        }

        if (options::particles_enabled)
        {
            particle_renderer::render(main_view);

            prelit_pool::instance.render(render_view::views[main_view.id]);

            streak_renderer::render(main_view);

            world_renderer::render();

            if (game_flow::manager::instance.current_state == game_flow::state::racing)
            {
                effect_world_prelit::instance->set_current_pass(0u);

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, false);

                main_view.rain->update_and_render();

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, true);

                effect_world_prelit::instance->finalize();
            }
        }

        visual_treatment::instance->update(main_view);
    }
}
