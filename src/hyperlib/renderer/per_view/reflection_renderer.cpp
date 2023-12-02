#include <hyperlib/renderer/per_view/reflection_renderer.hpp>

namespace hyper
{
    void reflection_renderer::render(const view::instance& reflection_view, const view::instance& player_view, const grand_scenery_cull_info& culler)
    {
        call_function<void(__cdecl*)(const grand_scenery_cull_info&)>(0x0072E0F0)(culler);
    }
}
