#include <hyperlib/renderer/per_view/rvm_renderer.hpp>

namespace hyper
{
    void rvm_renderer::render(const view::instance& rvm_view, const grand_scenery_cull_info& culler)
    {
        call_function<void(__cdecl*)(const grand_scenery_cull_info&)>(0x0072E350)(culler);
    }
}
