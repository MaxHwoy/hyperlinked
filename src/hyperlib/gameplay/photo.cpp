#include <hyperlib/gameplay/photo.hpp>

namespace hyper
{
    auto photo::mode::get_state() -> photo::state
    {
        return this->state_;
    }

    void photo::mode::take_photo()
    {
        call_function<void(__thiscall*)(photo::mode*)>(0x00743380)(this);
    }
}
