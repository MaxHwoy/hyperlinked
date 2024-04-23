#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/skid_renderer.hpp>

namespace hyper
{
    void skid_renderer::render(const view::instance& view)
    {
        effect_world_prelit::instance->set_current_pass(0u);

        call_function<void(__cdecl*)(const view::instance&)>(0x0079D6A0)(view);

        effect_world_prelit::instance->finalize();
    }
}
