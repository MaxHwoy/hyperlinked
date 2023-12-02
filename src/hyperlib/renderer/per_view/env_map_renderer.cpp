#include <hyperlib/renderer/per_view/env_map_renderer.hpp>

namespace hyper
{
    void env_map_renderer::render(const view::instance& env_map_view, const grand_scenery_cull_info& culler)
    {
        call_function<void(__cdecl*)(const view::instance&, const grand_scenery_cull_info&)>(0x0072E460)(env_map_view, culler);
    }
}
