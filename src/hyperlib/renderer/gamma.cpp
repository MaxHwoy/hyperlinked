#include <hyperlib/renderer/gamma.hpp>

namespace hyper
{
    void gamma::set_brightness()
    {
        call_function<void(__thiscall*)(gamma*)>(0x0073BC80)(this);
    }

    void gamma::set_brightness_level(float brightness)
    {
        call_function<void(__thiscall*)(gamma*, float)>(0x00748650)(this, brightness);
    }
}
