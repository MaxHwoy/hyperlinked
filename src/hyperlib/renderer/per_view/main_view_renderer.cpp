#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/per_view/main_view_renderer.hpp>

namespace hyper
{
    void main_view_renderer::render(const view::instance& main_view, const grand_scenery_cull_info& culler)
    {
        if (main_view.active && main_view.attached_target != nullptr && main_view.attached_target->active)
        {
            call_function<void(__cdecl*)(const view::instance&, const grand_scenery_cull_info&)>(0x0072E5B0)(main_view, culler);
        }
    }
}
