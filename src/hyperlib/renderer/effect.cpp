#include <hyperlib/utils/utils.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    effect::effect(shader_type type, effect::flags flags, effect::param_index_pair* indices, const effect::input* input) : id_(type), index_pairs_(indices), table_{}
    {
        for (size_t i = 0u; i < std::size(this->params_); ++i)
        {
            parameter& param = this->params_[i];

            param.name[0] = 0;
            param.key = 0u;
            param.handle = nullptr;
        }

        this->vector_[0] = nullptr;
        this->vector_[1] = nullptr;
        this->vector_[2] = nullptr;
        this->vector_[3] = nullptr;

        this->name_ = input->effect_name;
        this->flags_ = flags;
        this->last_used_light_material_ = nullptr;
        this->last_used_light_context_ = nullptr;
        this->active_ = false;
        this->effect_ = nullptr;

        this->low_lod_technique_number_ = -1;
        this->has_zero_offset_scale_ = -1;
        this->has_fog_disabled_ = -1;
        this->__3__ = -1;
        this->__4__ = -1;
        this->__5__ = -1;
        this->__6__ = -1;
        this->__7__ = -1;
        this->__8__ = -1;
        this->__9__ = -1;
        this->__10__ = -1;
        this->__11__ = -1;

        this->initialize(input);
        this->connect_parameters();
    }

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

    void effect::initialize(const effect::input* input)
    {
        if (this->effect_ != nullptr)
        {
            this->effect_->Release();

            this->effect_ = nullptr;
        }

        ::D3DVERTEXELEMENT9 elements[0x10];

        ::WORD stride = 0u;

        std::uint32_t index = 0u;

        while (input->channels[index].type != vertex_type::none)
        {
            const stream_channel& channel = input->channels[index];

            ::D3DVERTEXELEMENT9& element = elements[index++];

            element.Stream = 0u;
            element.Offset = stride;
            element.Type = static_cast<::BYTE>(shader_lib::vertex_decl_type_map[channel.type]);
            element.Method = 0u;
            element.Usage = static_cast<::BYTE>(shader_lib::vertex_decl_usage_map[channel.component]);
            element.UsageIndex = shader_lib::vertex_usage_index_map[channel.component];

            stride += static_cast<::WORD>(shader_lib::vertex_type_size_map[channel.type]);
        }

        elements[index++] = D3DDECL_END();

        directx::device()->CreateVertexDeclaration(elements, &this->vertex_decl_);

        this->stride_ = stride;


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

    void effect::reset_filter_params()
    {
        this->set_int(parameter_type::Cull_Mode, ::D3DCULL_NONE);
        this->set_int(parameter_type::BaseMagTextureFilter, directx::get_mag_filter());
        this->set_int(parameter_type::BaseMinTextureFilter, directx::get_min_filter());
        this->set_int(parameter_type::BaseMipTextureFilter, directx::get_mip_filter());
        this->set_int(parameter_type::BaseTextureFilterParam, directx::get_max_anisotropy());
        this->set_int(parameter_type::BaseAddressU, ::D3DTADDRESS_WRAP);
        this->set_int(parameter_type::BaseAddressV, ::D3DTADDRESS_WRAP);
        this->set_int(parameter_type::FECOLORWRITEFLAG, D3DCOLORWRITEENABLE_ALL);
        
        if (this->id_ == shader_type::FilterShader) // truly a blackbox moment
        {
            this->set_int(parameter_type::ColorWriteMode, D3DCOLORWRITEENABLE_ALL);
        }
    }

    void effect::load_effect_from_buffer(const effect::input* input)
    {
        this->has_zero_offset_scale_ = -1;
        this->has_fog_disabled_ = -1;
        this->last_used_light_material_ = nullptr;
        this->last_used_light_context_ = nullptr;

        if (this->effect_ != nullptr)
        {
            this->effect_->OnLostDevice();
            this->effect_->Release();
            this->effect_ = nullptr;
        }

        ::LPD3DXBUFFER errors;

        ::D3DXCreateEffectFromResourceA(directx::device(), nullptr, input->resource, nullptr, nullptr, 0u, shader_lib::effect_pool, &this->effect_, &errors);

        this->connect_parameters();

        this->reset_filter_params();
    }

    auto effect::find_techique(const char* name) -> technique*
    {
        if (name != nullptr)
        {
            size_t length = string::length(name);

            for (technique* i = this->table_.begin(); i != this->table_.end(); ++i)
            {
                if (length == i->size() && ::memcmp(name, i->begin(), length))
                {
                    return i;
                }
            }
        }

        return nullptr;
    }

    void effect::set_technique(const char* name)
    {
        technique* tech = this->find_techique(name);

        if (tech != nullptr)
        {
            this->effect_->SetTechnique(this->effect_->GetTechnique(tech->technique_index));
        }
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

    void shader_lib::lose_device()
    {
        for (size_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            shader_lib::effects_[i]->lose_device();
        }
    }

    void shader_lib::end_effect(effect& eff)
    {
        eff.end_effect_pass();
        eff.preflight_draw();
        eff.end_effect();

        shader_lib::current_effect = nullptr;
    }
}
