#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/renderer/model_renderer.hpp>

namespace hyper
{
    void model_renderer::render(const view::instance& view, instance_flags flags)
    {
        call_function<void(__cdecl*)(const view::instance&, instance_flags)>(0x007D1C40)(view, flags);
    }
}
