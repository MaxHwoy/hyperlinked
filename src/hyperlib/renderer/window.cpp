#include <hyperlib/renderer/window.hpp>

namespace hyper
{
    void window::get_resolution(std::uint32_t& x, std::uint32_t& y)
    {
        switch (window::racing_resolution)
        {
            case 0u:
                x = 640u;
                y = 480u;
                break;

            case 1:
                x = 800u;
                y = 600u;
                break;

            case 2:
                x = 1024u;
                y = 768u;
                break;

            case 3:
                x = 1280u;
                y = 960u;
                break;

            case 4:
                x = 1280u;
                y = 1024u;
                break;

            case 5:
                x = 1600u;
                y = 1200u;
                break;

            default:
                break;
        }
    }
}
