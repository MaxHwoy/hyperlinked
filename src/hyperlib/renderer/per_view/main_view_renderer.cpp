#include <hyperlib/options.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/model_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
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

        call_function<void(__cdecl*)(const view::instance&, const grand_scenery_cull_info&)>(0x0072E5B0)(main_view, culler);
    }
}
