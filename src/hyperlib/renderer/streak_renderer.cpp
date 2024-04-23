#include <hyperlib/options.hpp>
#include <hyperlib/renderer/streak_renderer.hpp>

namespace hyper
{
    void streak_renderer::render(light_streak& streak, const view::instance& view)
    {
        call_function<void(__thiscall*)(light_streak*, const view::instance&)>(0x0074E540)(&streak, view);
    }

    void streak_renderer::render(const view::instance& view)
    {
        if (!options::car_guys_camera)
        {
            for (bpnode<light_streak>* i = light_streak::list.head.begin(); i != light_streak::list.head.end(); i = i->next())
            {
                streak_renderer::render(*i->object, view);
            }
        }
    }
}
