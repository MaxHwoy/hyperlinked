#include <hyperlib/gameplay/g_race.hpp>
#include <hyperlib/gameplay/dal.hpp>

namespace hyper
{
    bool g_race::status::is_drift()
    {
        return call_function<bool(__cdecl*)()>(0x0047AA00)();
    }

    bool g_race::status::is_checkpoint()
    {
        return call_function<bool(__cdecl*)()>(0x0061AAF0)();
    }

    bool g_race::status::is_racing()
    {
        return g_race::status::instance != nullptr && g_race::status::instance->mode == g_race::status::play_mode::racing;
    }

    bool g_race::status::is_in_pursuit()
    {
        std::int32_t in_pursuit;

        dal::manager::instance.get_integer(0x2B02, in_pursuit, -1, -1, -1);

        return in_pursuit != 0;
    }
}
