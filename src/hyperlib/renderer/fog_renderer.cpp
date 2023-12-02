#include <hyperlib/renderer/fog_renderer.hpp>

namespace hyper
{
    fog_renderer::params::params() : enable(1.0f)
    {
    }

    void fog_renderer::initialize()
    {
        fog_renderer::params::instance = memory::allocate<fog_renderer::params>();
    }

    void fog_renderer::assign_parameters(params* parameters, const view::base& view)
    {
        call_function<void(__thiscall*)(params*, const view::base&)>(0x0073E990)(parameters, view);
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
