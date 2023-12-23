#include <hyperlib/renderer/anim_scene.hpp>

namespace hyper
{
    void anim_scene::render(const view::instance& view, std::uint32_t flags)
    {
        call_function<void(__cdecl*)(const view::instance&, std::uint32_t)>(0x00456070)(view, flags);
    }
}
