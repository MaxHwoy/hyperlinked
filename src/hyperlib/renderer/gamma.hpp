#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class gamma final
    {
    public:
        void set_brightness();

        void set_brightness_level(float brightness);

    private:
        bool change_level_enabled;
        ::D3DGAMMARAMP ramp;

    public:
        static inline gamma*& instance = *reinterpret_cast<gamma**>(0x00B42F2C);

        static inline float& sf_gamma = *reinterpret_cast<float*>(0x00B43078);
    };

    ASSERT_SIZE(gamma, 0x602);
}
