#include <hyperlib/renderer/window.hpp>

namespace hyper
{
    void window::get_resolution(std::uint32_t& x, std::uint32_t& y)
    {
        call_function<void(__thiscall*)(window&, std::uint32_t&, std::uint32_t&)>(0x00712AC0)(window::instance, x, y);
    }
}
