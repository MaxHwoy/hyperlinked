#include <hyperlib/utils/utils.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    void effect::store_param_by_key(::LPCSTR name, ::D3DXHANDLE handle)
    {
        const param_index_pair* pair = shader_lib::find_param_index(hashing::bin(name));

        if (pair != nullptr)
        {
            parameter& param = this->params_[pair->index];

            param.key = pair->key;
            param.handle = handle;
            
            string::copy_s(param.name, name);
        }
    }

    void effect::connect_parameters()
    {
        ::D3DXEFFECT_DESC effect_desc;
        ::D3DXPARAMETER_DESC param_desc;

        this->effect_->GetDesc(&effect_desc);

        for (std::uint32_t i = 0u; i < effect_desc.Parameters; ++i)
        {
            ::D3DXHANDLE handle = this->effect_->GetParameter(nullptr, i);

            this->effect_->GetParameterDesc(handle, &param_desc);

            if (param_desc.Name != nullptr && param_desc.Type != ::D3DXPT_SAMPLER)
            {
                this->store_param_by_key(param_desc.Name, handle);
            }
        }
    }

    void effect::free_effect()
    {
        this->effect_->OnLostDevice();
    }

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

    auto shader_lib::find_param_index(std::uint32_t key) -> const effect::param_index_pair*
    {
        const auto& list = shader_lib::effect_param_list;

        void* entry = utils::scan_hash_table_key32(key, list.begin(), list.length(), offsetof(effect::param_index_pair, key), sizeof(effect::param_index_pair));

        return reinterpret_cast<const effect::param_index_pair*>(entry);
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

    void shader_lib::free_effects()
    {
        for (size_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            shader_lib::effects_[i]->free_effect();
        }
    }
}
