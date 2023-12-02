#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class fog_renderer final
    {
    public:
        struct params
        {
            float float_0;
            float float_1;
            float float_2;
            float float_3;
            float float_4;
            float float_5;
            float float_6;
            float float_7;
            vector2 sky_fog_falloff;
            float fog_enable;

            static inline params*& instance = *reinterpret_cast<params**>(0x00B4295C);
        };
    };

    ASSERT_SIZE(fog_renderer::params, 0x2C);
}
