#include <hyperlib/ui/feng_hud.hpp>

namespace hyper
{
    bool feng_hud::should_rvm_be_visible()
    {
        return call_function<bool(__cdecl*)(std::uint32_t)>(0x005E3D60)(1u);
    }
}
