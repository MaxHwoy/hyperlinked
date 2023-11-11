#include <hyperlib/streamer/track_path.hpp>

namespace hyper
{
    void track_path::manager::disable_all_barriers()
    {
        for (std::uint32_t i = 0u; i < this->barrier_count; ++i)
        {
            this->barriers[i].enabled = false;
        }
    }

    void track_path::manager::enable_barriers(const char* barrier_name)
    {
        call_function<void(__thiscall*)(track_path::manager*, const char*)>(0x007A2390)(this, barrier_name);
    }

    auto track_path::manager::find_zone(const vector2* position, zone::type type, const zone* prev) -> zone*
    {
        if (position == nullptr)
        {
            return nullptr;
        }

        return call_function<zone*(__thiscall*)(manager*, const vector2*, zone::type, const zone*)>(0x0079DC60)(this, position, type, prev);
    }
}
