#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    void shader_lib::init()
    {
        for (effect::parameter_type i = effect::parameter_type::first; i < effect::parameter_type::count; ++i)
        {
            shader_lib::effect_param_list[static_cast<std::uint32_t>(i)].index = static_cast<std::uint32_t>(i);
            shader_lib::effect_param_list[static_cast<std::uint32_t>(i)].key = hashing::bin(effect::get_parameter_name(i));
        }

        std::sort(shader_lib::effect_param_list.begin(), shader_lib::effect_param_list.end(), [](const effect::param_index_pair& lhs, const effect::param_index_pair& rhs) -> bool
        {
            return lhs.key < rhs.key;
        });

        shader_lib::initilaized_ = true;

        // #TODO init
    }

    auto shader_lib::find_input(const char* name) -> const effect::input*
    {
        for (size_t i = 0u; i < shader_lib::inputs_.length(); ++i)
        {
            if (::strcmp(name, shader_lib::inputs_[i].effect_name))
            {
                return &shader_lib::inputs_[i];
            }
        }

        return nullptr;
    }
}
