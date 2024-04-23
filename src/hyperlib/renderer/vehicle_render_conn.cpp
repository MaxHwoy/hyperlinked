#include <hyperlib/renderer/vehicle_render_conn.hpp>

namespace hyper
{
    void vehicle_render_conn::render_fe(const view::instance& view, bool reflection)
    {
        call_function<void(__cdecl*)(const view::instance&, std::uint32_t)>(0x007E0F00)(view, reflection);
    }

    void vehicle_render_conn::render_world(const view::instance& view, bool reflection)
    {
        call_function<void(__cdecl*)(const view::instance&, std::uint32_t)>(0x007CCA20)(view, reflection);
    }

    void vehicle_render_conn::render_flares(const view::instance& view, bool reflection, std::uint32_t reflection_flags)
    {
        call_function<void(__cdecl*)(const view::instance&, std::uint32_t, std::uint32_t)>(0x007CCAD0)(view, reflection, reflection_flags);
    }
}
