#include <hyperlib/renderer/fog_renderer.hpp>

namespace hyper
{
    fog_renderer::params::params() : enable(1.0f)
    {
    }

    auto fog_renderer::query::calculate_fog(const view::base& view, bool unknown) -> std::uint32_t
    {
        return call_function<std::uint32_t(__thiscall*)(fog_renderer::query*, const view::base&, bool)>(0x007D1F50)(this, view, unknown);
    }

    void fog_renderer::initialize()
    {
        fog_renderer::params::instance = memory::allocate<fog_renderer::params>();
    }

    void fog_renderer::assign_parameters(params* parameters, const view::base& view)
    {
        fog_renderer::query::instance.calculate_fog(view, false);

        parameters->start = fog_renderer::query::instance.start;
        parameters->end = fog_renderer::query::instance.end;
        parameters->power = fog_renderer::query::instance.power;
        parameters->exponent = fog_renderer::query::instance.exponent;
        parameters->sky_falloff = fog_renderer::query::instance.sky_falloff;
        parameters->sky_offset = fog_renderer::query::instance.sky_offset;
        parameters->color.r = fog_renderer::query::instance.color.r * 0.0039215689f;
        parameters->color.g = fog_renderer::query::instance.color.g * 0.0039215689f;
        parameters->color.b = fog_renderer::query::instance.color.b * 0.0039215689f;
        parameters->color.a = 1.0f;
    }

    void fog_renderer::apply_parameters(const params* parameters, effect* effect)
    {
        if (parameters != nullptr)
        {
            if (effect->has_parameter(effect::parameter_type::cvFogValue))
            {
                vector4 value =
                {
                    parameters->end,
                    1.0f / (parameters->end - parameters->start),
                    parameters->power,
                    parameters->exponent
                };

                effect->set_float_unchecked(effect::parameter_type::cfFogEnable, parameters->enable);

                effect->set_vector_unchecked(effect::parameter_type::cvFogValue, value);

                effect->set_vector_unchecked(effect::parameter_type::cvFogColour, parameters->color.as_vector4());
            }

            if (effect->has_parameter(effect::parameter_type::FogColor))
            {
                color32 color =
                {
                    static_cast<std::uint8_t>(parameters->color.b * 255.0f),
                    static_cast<std::uint8_t>(parameters->color.g * 255.0f),
                    static_cast<std::uint8_t>(parameters->color.r * 255.0f),
                    std::numeric_limits<std::uint8_t>::max()
                };

                effect->set_int_unchecked(effect::parameter_type::FogColor, color);
            }
        }
    }
}
