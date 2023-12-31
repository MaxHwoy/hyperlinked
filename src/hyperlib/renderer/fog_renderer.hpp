#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    class fog_renderer final
    {
    public:
        struct params
        {
        public:
            params();

        public:
            color color;
            float start;
            float end;
            float power;
            float exponent;
            float sky_falloff;
            float sky_offset;
            float enable;

        public:
            static inline params*& instance = *reinterpret_cast<params**>(0x00B4295C);
        };

        struct query
        {
        public:
            auto calculate_fog(const view::base& view, bool unknown) -> std::uint32_t;

        public:
            float falloff;
            float falloff_x;
            float falloff_y;
            float start;
            float end;
            float exponent;
            float sky_falloff;
            float sky_offset;
            float power;
            color32 color;

        public:
            static inline query& instance = *reinterpret_cast<query*>(0x00B74210);
        };

    public:
        static void initialize();
        
        static void assign_parameters(params* parameters, const view::base& view);

        static void apply_parameters(const params* parameters, effect* effect);
    };

    ASSERT_SIZE(fog_renderer::params, 0x2C);
}
