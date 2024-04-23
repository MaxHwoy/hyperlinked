#include <hyperlib/assets/emitters.hpp>
#include <hyperlib/renderer/particle_renderer.hpp>

namespace hyper
{
    void particle_renderer::render(const view::instance& view)
    {
        call_function<void(__thiscall*)(emitter::system*, const view::instance&)>(0x0075AA10)(&emitter::system::instance, view);
    }
}
