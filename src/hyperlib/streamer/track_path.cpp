#include <hyperlib/streamer/track_path.hpp>

namespace hyper
{
    auto track_path::manager::find_zone(const vector2* position, zone::type type, const zone* prev) -> zone*
    {
        if (position == nullptr)
        {
            return nullptr;
        }

        return call_function<zone*(__thiscall*)(manager*, const vector2*, zone::type, const zone*)>(0x0079DC60)(this, position, type, prev);
    }
}
