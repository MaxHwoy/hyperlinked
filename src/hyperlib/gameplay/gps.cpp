#include <hyperlib/gameplay/gps.hpp>

namespace hyper
{
    void gps::render(const view::instance& view)
    {
        call_function<void(__cdecl*)(const view::instance&)>(0x0042ADF0)(view);
    }
}
