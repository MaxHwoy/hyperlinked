#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class fog_renderer final
    {
    public:
        struct params
        {
            color color;
            float start;
            float end;
            float power;
            float exponent;
            float sky_falloff;
            float sky_offset;
            float enable;

            static inline params*& instance = *reinterpret_cast<params**>(0x00B4295C);
        };
    };

    ASSERT_SIZE(fog_renderer::params, 0x2C);
}
