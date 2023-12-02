#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/time_of_day.hpp>

namespace hyper
{
    time_of_day::time_of_day() :
        sunny(hashing::vlt_const("sunny")),
        dusk_sunny(hashing::vlt_const("dusk_sunny")),
        overcast(hashing::vlt_const("overcast")),
        dusk_overcast(hashing::vlt_const("dusk_overcast")),
        _0xA01A3942(0xA01A3942u),
        _0x0E1C797C(0x0E1C797Cu),
        _0x94ABDFF0(0x94ABDFF0u),
        _0x919B6689(0x919B6689u),
        update_rate(1.0f),
        current_time_of_day(0.45f),
        sun_azimuth(3.28f),
        latitude(0.5f),
        update_direction(-1)
    {
        this->current.diffuse_color = { 0.61f, 0.49f, 0.23f, 1.0f };
        this->current.ambient_color = { 0.14f, 0.16f, 0.22f, 4.0f };
        this->current.fog_sky_color = { 0.27f, 0.46f, 0.96f, 1.0f };
        this->current.specular_color = { 0.58f, 0.53f, 0.42f, 1.0f };
        this->current.fog_haze_color = { 0.19f, 0.25f, 0.38f, 1.0f };
        this->current.car_spec_scale = 1.0f;
        this->current.env_sky_brightness = 2.5f;
        this->current.fog_sky_color_scale = 2.0f;
        this->current.fog_haze_color_scale = 0.0f;
        this->current.fog_in_light_scatter = 8.0f;
        this->current.fog_sun_falloff = -0.16f;

        this->update_time();
        this->update(0.1f);

        this->some_color = { 0.61f, 0.49f, 0.23f, 1.0f };
    }

    void time_of_day::update_time()
    {
        call_function<void(__thiscall*)(time_of_day*)>(0x007F1080)(this);
    }

    void time_of_day::update(float intensity)
    {
        call_function<void(__thiscall*)(time_of_day*, float)>(0x007F12A0)(this, intensity);
    }

    void time_of_day::init()
    {
        time_of_day::instance = memory::allocate<time_of_day>();
    }
}
