#include <hyperlib/world/collision.hpp>

namespace hyper
{
    bool collision::manager::get_world_height_at_point_rigorous(const vector3& point, float& height, vector3* normal)
    {
        return call_function<bool(__thiscall*)(collision::manager*, const vector3*, float*, vector3*)>(0x00816DF0)(this, &point, &height, normal);
    }
}
