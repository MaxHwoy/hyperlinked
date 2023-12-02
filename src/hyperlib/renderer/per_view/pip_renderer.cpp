#include <hyperlib/renderer/per_view/pip_renderer.hpp>

namespace hyper
{
    void pip_renderer::render(const view::instance& pip_view, const grand_scenery_cull_info& culler)
    {
        call_function<void(__cdecl*)(const grand_scenery_cull_info&)>(0x0073EA60)(culler);
    }
}
