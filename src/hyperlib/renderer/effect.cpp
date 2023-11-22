#include <hyperlib/global_vars.hpp>
#include <hyperlib/utils/utils.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/lighting.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>

#pragma warning (disable : 26813)

namespace hyper
{
    /// *************************************************************************************************************
    /// TECHNIQUE
    /// *************************************************************************************************************

    effect::technique::technique() : name{}, technique_index(std::numeric_limits<std::uint32_t>::max()), detail_level(0u)
    {
    }

    effect::technique::technique(const technique& other) : name(other.name), technique_index(other.technique_index), detail_level(other.detail_level)
    {
    }

    effect::technique::technique(technique&& other) : name(std::move(other.name)), technique_index(other.technique_index), detail_level(other.detail_level)
    {
    }

    effect::technique::technique(const char* name, std::uint32_t index, std::uint32_t level) : name(name), technique_index(index), detail_level(level)
    {
    }

    auto effect::technique::operator=(const technique& other) -> effect::technique&
    {
        if (this != &other)
        {
            this->name = other.name;
            this->technique_index = other.technique_index;
            this->detail_level = other.detail_level;
        }

        return *this;
    }

    auto effect::technique::operator=(technique&& other) -> effect::technique&
    {
        if (this != &other)
        {
            this->name = std::move(other.name);
            this->technique_index = other.technique_index;
            this->detail_level = other.detail_level;
        }

        return *this;
    }

    /// *************************************************************************************************************
    /// EFFECT
    /// *************************************************************************************************************

    effect::effect(shader_type type, effect::flags flags, const effect::param_index_pair* indices, const effect::input* input) : 
        name_(input->effect_name),
        id_(type), 
        flags_(flags),
        index_pairs_(indices), 
        unsupported_table_{}, 
        supported_table_{},
        effect_(nullptr),
        vertex_decl_(nullptr)
    {
        this->reinitialize();
    }

    effect::~effect()
    {
        if (this->effect_ != nullptr)
        {
            this->effect_->OnLostDevice();
            this->effect_->Release();
            this->effect_ = nullptr;
        }

        if (this->vertex_decl_ != nullptr)
        {
            this->vertex_decl_->Release();
            this->vertex_decl_ = nullptr;
        }
    }

    void effect::handle_material_data(const light_material::instance& material, draw_flags flags)
    {
        if (this->has_parameter(parameter_type::cvDiffuseMin) && &material != this->last_used_light_material_)
        {
            this->last_used_light_material_ = &material;

            const light_material::data& data = material.material;

            float min_clamp = light_material::min_clamp;
            float spec_scale = light_material::spec_scale;
            float envmap_scale = light_material::envmap_scale;

            color diffuse_min =
            {
                data.diffuse_min_r * data.diffuse_min_scale,
                data.diffuse_min_g * data.diffuse_min_scale,
                data.diffuse_min_b * data.diffuse_min_scale,
                data.diffuse_min_a
            };

            color diffuse_range =
            {
                data.diffuse_max_r * data.diffuse_max_scale - diffuse_min.r,
                data.diffuse_max_g * data.diffuse_max_scale - diffuse_min.g,
                data.diffuse_max_b * data.diffuse_max_scale - diffuse_min.b,
                data.diffuse_max_a - diffuse_min.a
            };

            color specular_min =
            {
                data.specular_min_r * data.specular_min_scale * spec_scale,
                data.specular_min_g * data.specular_min_scale * spec_scale,
                data.specular_min_b * data.specular_min_scale * spec_scale,
                0.0f
            };

            color specular_range =
            {
                data.specular_max_r * data.specular_max_scale * spec_scale - specular_min.r,
                data.specular_max_g * data.specular_max_scale * spec_scale - specular_min.g,
                data.specular_max_b * data.specular_max_scale * spec_scale - specular_min.b,
                0.0f
            };

            color envmap_min =
            {
                data.envmap_min_r * data.envmap_min_scale * envmap_scale,
                data.envmap_min_g * data.envmap_min_scale * envmap_scale,
                data.envmap_min_b * data.envmap_min_scale * envmap_scale,
                0.0f
            };

            color envmap_range =
            {
                data.envmap_max_r * data.envmap_max_scale * envmap_scale - envmap_min.r,
                data.envmap_max_g * data.envmap_max_scale * envmap_scale - envmap_min.g,
                data.envmap_max_b * data.envmap_max_scale * envmap_scale - envmap_min.b,
                0.0f
            };

            vector4 powers =
            {
                data.diffuse_power,
                data.specular_power,
                data.envmap_power,
                0.0f
            };

            vector4 clamps =
            {
                data.diffuse_clamp,
                data.envmap_clamp,
                1.0f / math::max(data.diffuse_clamp, min_clamp),
                1.0f / math::max(data.envmap_clamp, min_clamp)
            };

            vector4 flakes =
            {
                data.diffuse_flakes,
                data.specular_flakes,
                data.vinyl_luminance_min_scale,
                data.vinyl_luminance_max_scale - data.vinyl_luminance_min_scale
            };

            vector4 vinyls =
            {
                data.diffuse_vinyl_scale,
                data.specular_vinyl_scale,
                data.envmap_vinyl_scale,
                0.0f
            };

            this->set_vector(parameter_type::cvDiffuseMin, diffuse_min.as_vector4());
            this->set_vector(parameter_type::cvDiffuseRange, diffuse_range.as_vector4());
            this->set_vector(parameter_type::cvSpecularMin, specular_min.as_vector4());
            this->set_vector(parameter_type::cvSpecularRange, specular_range.as_vector4());
            this->set_vector(parameter_type::cvEnvmapMin, envmap_min.as_vector4());
            this->set_vector(parameter_type::cvEnvmapRange, envmap_range.as_vector4());
            this->set_vector(parameter_type::cvPowers, powers);
            this->set_vector(parameter_type::cvClampAndScales, clamps);
            this->set_vector(parameter_type::cvFlakes, flakes);
            this->set_vector(parameter_type::cvVinylScales, vinyls);
            this->set_float(parameter_type::cfSpecularPower, data.specular_power);
        }
    }

    void effect::set_transforms(const matrix4x4& local_to_world, const render_view& view, bool use_nonjittered)
    {
        if (&local_to_world != effect::last_submitted_matrix_ || effect::current_effect_ != effect::last_submitted_effect_ || view.id != effect::last_submitted_view_id_)
        {
            const matrix4x4* view_matrix = &view.view_matrix;
            const matrix4x4* proj_matrix = &view.projection_matrix;
            const matrix4x4* view_proj_matrix = use_nonjittered
                ? &view.non_jittered_view_projection_matrix
                : &view.view_projection_matrix;

            if (view.id == effect::last_submitted_view_id_)
            {
                if (effect::current_effect_ != effect::last_submitted_effect_ && !this->active_)
                {
                    directx::device()->SetTransform(::D3DTS_VIEW, reinterpret_cast<const ::D3DMATRIX*>(view_matrix));
                    directx::device()->SetTransform(::D3DTS_PROJECTION, reinterpret_cast<const ::D3DMATRIX*>(proj_matrix));
                }
            }
            else
            {
                if (!this->active_)
                {
                    directx::device()->SetTransform(::D3DTS_VIEW, reinterpret_cast<const ::D3DMATRIX*>(view_matrix));
                    directx::device()->SetTransform(::D3DTS_PROJECTION, reinterpret_cast<const ::D3DMATRIX*>(proj_matrix));
                }

                effect::last_submitted_view_id_ = view.id;
            }

            effect::last_submitted_effect_ = effect::current_effect_;
            effect::last_submitted_matrix_ = &local_to_world;

            if (this->has_parameter(parameter_type::cmWorldMatTranspose))
            {
                matrix4x4 transpose;

                transpose.row(0u).as_vector3() = local_to_world.row(0u).as_vector3().normalized();
                transpose.row(1u).as_vector3() = local_to_world.row(1u).as_vector3().normalized();
                transpose.row(2u).as_vector3() = vector3::cross(transpose.row(0u).as_vector3(), transpose.row(1u).as_vector3());

                transpose.m14 = 0.0f;
                transpose.m24 = 0.0f;
                transpose.m34 = 0.0f;
                transpose.m41 = 0.0f;
                transpose.m42 = 0.0f;
                transpose.m43 = 0.0f;
                transpose.m44 = 0.0f;

                math::transpose_matrix(transpose, transpose);

                this->set_matrix_unchecked(parameter_type::cmWorldMatTranspose, transpose);
            }

            if (this->has_parameter(parameter_type::cmWorldView))
            {
                matrix4x4 world_view;

                math::multiply_matrix(local_to_world, *view_matrix, world_view);

                this->set_matrix_unchecked(parameter_type::cmWorldView, world_view);
            }

            this->set_matrix(parameter_type::cmWorldMat, local_to_world);

            if (this->active_)
            {
                matrix4x4 world_view_proj;

                math::multiply_matrix(local_to_world, *view_proj_matrix, world_view_proj);

                this->set_matrix(parameter_type::WorldViewProj, world_view_proj);
            }
            else
            {
                directx::device()->SetTransform(D3DTS_WORLDMATRIX(0), reinterpret_cast<const ::D3DMATRIX*>(&local_to_world));
            }

            if (this->has_parameter(parameter_type::cvLocalEyePos) || this->has_parameter(parameter_type::cvLocalLightVec))
            {
                matrix4x4 inverted;

                math::invert_transform(local_to_world, inverted);

                if (this->has_parameter(parameter_type::cvLocalEyePos))
                {
                    vector4 sun_position(view.camera_position);

                    math::transform_point(inverted, sun_position);

                    this->set_vector_unchecked(parameter_type::cvLocalEyePos, sun_position);
                }

                if (this->has_parameter(parameter_type::cvLocalLightVec))
                {
                    vector4 sun_direction(lighting::time_of_day::instance->sun_direction);

                    math::transform_point(inverted, sun_direction);

                    this->set_vector_unchecked(parameter_type::cvLocalLightVec, sun_direction.normalized());
                }
            }

            if (this->has_parameter(parameter_type::cmHeadlight1WVP))
            {
                if (this->supported_table_.front().detail_level == 1)
                {
                    const render_view& headlight = render_view::views[view_id::player1_headlight];

                    matrix4x4 headlight_wvp;
                    matrix4x4 headlight_trs =
                    {
                        0.5f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.5f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.5f, 0.0f,
                        0.5f, 0.5f, 0.5f, 1.0f
                    };

                    math::multiply_matrix(local_to_world, headlight.view_projection_matrix, headlight_wvp);
                    math::multiply_matrix(headlight_wvp, headlight_trs, headlight_wvp);

                    this->set_matrix_unchecked(parameter_type::cmHeadlight1WVP, headlight_wvp);
                }
            }
        }
    }

    void effect::draw_full_screen_quad(::IDirect3DTexture9* texture, bool invert)
    {
        struct {
            vector3 position;
            color32 color;
            vector2 uv;
        } vertices[4];

        directx::device()->SetRenderState(::D3DRS_CULLMODE, ::D3DCULL_NONE);

        ::D3DSURFACE_DESC desc;

        texture->GetLevelDesc(0u, &desc);

        float w = static_cast<float>(desc.Width);
        float h = static_cast<float>(desc.Height);

        if ((desc.Width & 0x80000000) != 0)
        {
            w += 4.2949673e9f;
        }

        if ((desc.Height & 0x80000000) != 0)
        {
            h += 4.2949673e9f;
        }

        float inv_w = 0.5f / w;
        float inv_h = 0.5f / h;

        float uv00 = inv_w;
        float uv10 = inv_w + 1.0f;
        float uv20 = inv_w + 1.0f;
        float uv30 = inv_w;

        float uv11;
        float uv31;
        float uv21;
        float uv01;

        if (invert)
        {
            uv01 = inv_h + 1.0f;
            uv11 = inv_h + 1.0f;
            uv21 = inv_h;
            uv31 = inv_h;
        }
        else
        {
            uv01 = inv_h;
            uv11 = inv_h;
            uv21 = inv_h + 1.0f;
            uv31 = inv_h + 1.0f;
        }

        vertices[0].position.x = -1.0f;
        vertices[0].position.y = +1.0f;
        vertices[0].position.z = +0.0f;
        vertices[0].color = color32::white();
        vertices[0].uv.x = uv00;
        vertices[0].uv.y = uv01;

        vertices[1].position.x = +1.0f;
        vertices[1].position.y = +1.0f;
        vertices[1].position.z = +0.0f;
        vertices[1].color = color32::white();
        vertices[1].uv.x = uv10;
        vertices[1].uv.y = uv11;

        vertices[2].position.x = +1.0f;
        vertices[2].position.y = -1.0f;
        vertices[2].position.z = +0.0f;
        vertices[2].color = color32::white();
        vertices[2].uv.x = uv20;
        vertices[2].uv.y = uv21;

        vertices[3].position.x = -1.0f;
        vertices[3].position.y = -1.0f;
        vertices[3].position.z = +0.0f;
        vertices[3].color = color32::white();
        vertices[3].uv.x = uv30;
        vertices[3].uv.y = uv31;

        this->set_texture(parameter_type::DIFFUSEMAP_TEXTURE, texture);

        this->effect_->CommitChanges();
        
        directx::device()->DrawPrimitiveUP(::D3DPT_TRIANGLEFAN, 2u, vertices, sizeof(vertices[0]));
    }

    void effect::start()
    {
        this->set_float(parameter_type::cfSpecularPower, lighting::default_spec_power);

        if (this->has_parameter(parameter_type::cvVertexPowerBrightness))
        {
            this->reset_lighting_params();
        }
    }

    void effect::end()
    {
        // nothing
    }

    void effect::load_global_textures()
    {
        // nothing
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

        this->load_effect_from_input(input);

        std::uint32_t max_detail = 3u; // #TODO move this as part of settings

        if (directx::adapter.VendorId == 4318u) // nvidia
        {
            if (directx::device_caps.PixelShaderVersion < 0xFFFF0300u) // less than ps_3_0
            {
                if (directx::adapter.DeviceId < 816u || directx::adapter.DeviceId > 820u)
                {
                    max_detail = 1u;
                }
            }
        }

        ::D3DXEFFECT_DESC eff_desc;

        this->effect_->GetDesc(&eff_desc);

        this->unsupported_table_.clear();
        this->supported_table_.clear();
        this->supported_table_.reserve(eff_desc.Techniques);

        constexpr const char* lowlod = "lowlod";
        constexpr size_t lowlod_size = string::length(lowlod);

        for (std::uint32_t i = 0u; i < eff_desc.Techniques; ++i)
        {
            ::D3DXHANDLE tech = this->effect_->GetTechnique(i);

            if (SUCCEEDED(this->effect_->ValidateTechnique(tech)))
            {
                ::D3DXTECHNIQUE_DESC tech_desc;

                this->effect_->GetTechniqueDesc(tech, &tech_desc);

                if (string::length(tech_desc.Name) == lowlod_size && !::memcmp(tech_desc.Name, lowlod, lowlod_size))
                {
                    this->low_lod_technique_number_ = static_cast<std::int32_t>(i);
                }
                else
                {
                    if (::D3DXHANDLE annotation = this->effect_->GetAnnotationByName(tech, "shader"))
                    {
                        std::uint32_t shader_detail = std::numeric_limits<std::uint32_t>::max();

                        this->effect_->GetInt(annotation, reinterpret_cast<::INT*>(&shader_detail));

                        if (shader_detail <= max_detail)
                        {
                            this->supported_table_.emplace_back(tech_desc.Name, i, shader_detail);
                        }
                    }
                }
            }
        }

        for (std::uint32_t i = 0u; i < eff_desc.Techniques; ++i)
        {
            ::D3DXHANDLE tech = this->effect_->GetTechnique(i);

            if (SUCCEEDED(this->effect_->ValidateTechnique(tech)))
            {
                ::D3DXTECHNIQUE_DESC tech_desc;

                this->main_technique_number_ = static_cast<std::int32_t>(i);
                this->main_technique_handle_ = tech;

                this->effect_->GetTechniqueDesc(tech, &tech_desc);

                this->pass_count_ = tech_desc.Passes;

                break;
            }
        }

        this->active_ = this->effect_ != nullptr;
        this->has_main_technique_ = this->main_technique_handle_ != nullptr;
    }

    void effect::reinitialize()
    {
        ::memset(this->params_, 0, sizeof(this->params_));

        this->last_used_light_material_ = nullptr;
        this->last_used_light_context_ = nullptr;
        this->active_ = false;
        this->stride_ = 0u;

        this->low_lod_technique_number_ = -1;
        this->has_zero_offset_scale_ = -1;
        this->has_fog_disabled_ = -1;

        this->main_technique_number_ = -1;
        this->main_technique_handle_ = nullptr;
        this->has_main_technique_ = false;
        this->pass_count_ = 0u;
        
        this->__3__ = -1;
        this->__4__ = -1;
        this->__5__ = -1;
        this->__6__ = -1;
        this->__7__ = -1;
        this->__8__ = -1;
        this->__9__ = -1;
        this->__10__ = -1;
        this->__11__ = 0;
        this->__14__ = 0;

        this->supported_table_.clear();
        this->unsupported_table_.clear();

        this->release_effect();
        this->release_vertdecl();

        this->initialize(shader_lib::find_input(this->id_));
        this->connect_parameters();
    }

    void effect::reset()
    {
        this->has_zero_offset_scale_ = -1;
        this->has_fog_disabled_ = -1;
        this->last_used_light_material_ = nullptr;
        this->last_used_light_context_ = nullptr;
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

    void effect::reset_lighting_params()
    {
        lighting::ingame_light_params.y = lighting::default_ingame_light_y;
        lighting::ingame_light_params.w = lighting::default_ingame_light_w;

        if (game_flow::manager::instance.current_state == game_flow::state::racing)
        {
            this->set_vector(parameter_type::cvVertexPowerBrightness, lighting::ingame_light_params);
        }
        else
        {
            this->set_vector(parameter_type::cvVertexPowerBrightness, lighting::frontend_light_params);
        }
    }

    void effect::load_effect()
    {
        this->load_effect_from_input(shader_lib::find_input(this->id_));
    }

    void effect::load_effect_from_input(const effect::input* input)
    {
        this->reset();

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

    void effect::recompute_techniques_by_detail(std::uint32_t detail_level)
    {
        eastl::vector<technique> all_techniques;

        all_techniques.reserve(this->unsupported_table_.size() + this->supported_table_.size());

        for (technique* i = this->unsupported_table_.begin(); i != this->unsupported_table_.end(); ++i)
        {
            all_techniques.push_back(std::move(*i));
        }

        for (technique* i = this->supported_table_.begin(); i != this->supported_table_.end(); ++i)
        {
            all_techniques.push_back(std::move(*i));
        }

        this->unsupported_table_.clear();
        this->supported_table_.clear();

        for (size_t i = 0u; i < all_techniques.size(); ++i)
        {
            technique& tech = all_techniques[i];

            if (tech.detail_level <= detail_level)
            {
                this->supported_table_.push_back(std::move(tech));
            }
            else
            {
                this->unsupported_table_.push_back(std::move(tech));
            }
        }

        std::stable_sort(this->supported_table_.begin(), this->supported_table_.end(), [](const technique& lhs, const technique& rhs) -> bool
        {
            return lhs.detail_level > rhs.detail_level;
        });
    }

    auto effect::find_techique(const char* name) -> technique*
    {
        if (name != nullptr)
        {
            size_t length = string::length(name);

            for (technique* i = this->supported_table_.begin(); i != this->supported_table_.end(); ++i)
            {
                if (length == i->name.length() && !::memcmp(name, i->name, length))
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

    void effect::set_current_pass(std::uint32_t pass, const char* technique, bool use_low_lod)
    {
        std::int32_t index = -1;

        if (technique != nullptr)
        {
            if (effect::technique* tech = this->find_techique(technique))
            {
                index = static_cast<std::int32_t>(tech->technique_index);
            }
        }
        else if ((world_renderer::use_lowlod_pass || use_low_lod) && this->low_lod_technique_number_ >= 0)
        {
            index = this->low_lod_technique_number_;
        }
        else
        {
            index = static_cast<std::int32_t>(this->supported_table_.front().technique_index);
        }

        if (index < 0)
        {
            PRINT_FATAL("Unable to find requested technique or lowlod/main technique is missing");
        }

        this->effect_->SetTechnique(this->effect_->GetTechnique(index));

        effect::current_effect_ = this;

        this->start();

        directx::device()->SetVertexDeclaration(this->vertex_decl_);

        this->effect_->Begin(&this->pass_count_, 0u);

        this->effect_->BeginPass(pass);
    }

    void effect::set_pca_blend_data(const pca::blend_data& data)
    {
        if (data.ucap_weight != nullptr)
        {
            const pca::ucap_frame_weights* weights = data.ucap_weight;

            parameter_type feature_type = static_cast<parameter_type>(weights->feature_heights_param_handle);

            this->set_vector(feature_type, *weights->feature_heights);

            for (std::uint32_t i = 0u; i < weights->channel_count; ++i)
            {
                const pca::channel_info& info = weights->channel_infos[i];

                parameter_type type = static_cast<parameter_type>(info.param_handle);

                std::uint32_t vector_count = info.weight_count >> 2;

                const vector4* vectors = reinterpret_cast<const vector4*>(pca::blend_ucap(*weights, info.type, data.curr_frame, data.next_frame, data.blend));

                this->set_vector_array(type, vectors, vector_count);
            }
        }
        else
        {
            const pca::weights* weights = data.pca_weight;

            for (std::uint32_t i = 0u; i < weights->channel_count; ++i)
            {
                const pca::channel_info& info = weights->channel_infos[i];

                parameter_type type = static_cast<parameter_type>(info.param_handle);

                std::uint32_t vector_count = info.weight_count >> 2;

                std::uint32_t offset = data.curr_frame * weights->weights_per_frame_count + info.weight_offset;

                const vector4* vectors = reinterpret_cast<const vector4*>(weights->mean + offset);

                this->set_vector_array(type, vectors, vector_count);
            }
        }
    }

    void effect::set_blend_matrices(const matrix4x4* blend_matrices, const geometry::mesh_entry& entry)
    {
        if (this->has_parameter(parameter_type::camBlendMatrices))
        {
            matrix4x4 matrix_array[geometry::mesh_entry::blend_matrix_count];

            for (std::uint32_t i = 0u; i < geometry::mesh_entry::blend_matrix_count; ++i)
            {
                matrix_array[i] = blend_matrices[entry.blend_matrix_indices[i]];
            }

            this->set_matrix_array_unchecked(parameter_type::camBlendMatrices, matrix_array, geometry::mesh_entry::blend_matrix_count);
        }
    }

    void effect::set_light_context(const lighting::dynamic_context& context, const matrix4x4& local_to_world)
    {
        if (context.type == 0 && &context != this->last_used_light_context_)
        {
            this->last_used_light_context_ = &context;

            this->set_vector_array(parameter_type::cavHarmonicCoeff, context.harmonics, std::size(context.harmonics));

            this->set_matrix(parameter_type::cmLocalColourMatrix, context.local_color);

            this->set_matrix(parameter_type::cmLocalDirectionMatrix, context.local_direction);

            this->set_matrix(parameter_type::cmLocalPositionMatrix, local_to_world);
        }
    }

    void effect::set_texture_maps(rendering_model& model, draw_flags flags)
    {
        uintptr_t state_change = 0u;

        state_change |= static_cast<uintptr_t>(static_cast<std::uint32_t>(effect::last_submitted_draw_flags_) - static_cast<std::uint32_t>(flags));
        state_change |= static_cast<uintptr_t>(static_cast<std::uint32_t>(effect::last_submitted_render_state_) - static_cast<std::uint32_t>(model.render_bits));
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_effect_) - reinterpret_cast<uintptr_t>(this);
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_diffuse_map_) - reinterpret_cast<uintptr_t>(model.d3d9_diffuse_texture);
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_normal_map_) - reinterpret_cast<uintptr_t>(model.d3d9_normal_texture);
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_height_map_) - reinterpret_cast<uintptr_t>(model.d3d9_height_texture);
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_specular_map_) - reinterpret_cast<uintptr_t>(model.d3d9_specular_texture);
        state_change |= reinterpret_cast<uintptr_t>(effect::last_submitted_opacity_map_) - reinterpret_cast<uintptr_t>(model.d3d9_opacity_texture);

        if (state_change != 0u)
        {
            effect::last_submitted_effect_ = this;
            effect::last_submitted_draw_flags_ = flags;
            effect::last_submitted_render_state_ = model.render_bits;
            effect::last_submitted_diffuse_map_ = model.d3d9_diffuse_texture;
            effect::last_submitted_normal_map_ = model.d3d9_normal_texture;
            effect::last_submitted_height_map_ = model.d3d9_height_texture;
            effect::last_submitted_specular_map_ = model.d3d9_specular_texture;
            effect::last_submitted_opacity_map_ = model.d3d9_opacity_texture;

            std::int32_t cull_mode;

            if (model.render_bits.is_backface_culled && renderer::cull_backfaces)
            {
                cull_mode = ::D3DCULL_CCW - ((renderer::current_cull_mode == ::D3DCULL_CW) != ((flags & draw_flags::inverted_culling) != 0));
            }
            else
            {
                cull_mode = ::D3DCULL_NONE;
            }

            directx::device()->SetRenderState(::D3DRS_CULLMODE, static_cast<::DWORD>(cull_mode));

            this->set_diffuse_map(model);

            if (model.render_bits.wants_auxiliary_textures)
            {
                this->set_auxiliary_maps(model);
            }

            this->set_texture_animation(*model.base_texture_info);

            if (this->has_fog_disabled_ != static_cast<std::int32_t>(model.render_bits.is_additive_blend))
            {
                this->has_fog_disabled_ = model.render_bits.is_additive_blend;

                if (this->has_parameter(parameter_type::cfFogEnable))
                {
                    float enable_fog = 0.0f;

                    if (!model.render_bits.is_additive_blend)
                    {
                        enable_fog = 1.0f;
                    }

                    this->set_float_unchecked(parameter_type::cfFogEnable, enable_fog);
                }
            }
        }
    }

    void effect::set_diffuse_map(rendering_model& model)
    {
        this->set_texture(parameter_type::DIFFUSEMAP_TEXTURE, model.d3d9_diffuse_texture);

        texture::render_state state = model.render_bits;

        directx::device()->SetSamplerState(0u, ::D3DSAMP_ADDRESSU, state.texture_address_u);
        directx::device()->SetSamplerState(0u, ::D3DSAMP_ADDRESSV, state.texture_address_v);
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, state.alpha_test_enabled);
        directx::device()->SetRenderState(::D3DRS_ALPHAREF, state.alpha_test_ref << 4);
        directx::device()->SetRenderState(::D3DRS_ALPHAFUNC, ::D3DCMP_GREATER);
        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, state.alpha_blend_enabled);
        directx::device()->SetRenderState(::D3DRS_SRCBLEND, state.alpha_blend_src);
        directx::device()->SetRenderState(::D3DRS_DESTBLEND, state.alpha_blend_dest);
        directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, state.z_write_enabled);
        directx::device()->SetRenderState(::D3DRS_COLORWRITEENABLE, state.colour_write_alpha ? D3DCOLORWRITEENABLE_ALL : D3DCOLORWRITEENABLE_RGB);
    }

    void effect::set_auxiliary_maps(rendering_model& model)
    {
        if (model.d3d9_normal_texture != nullptr)
        {
            this->set_texture(parameter_type::NORMALMAP_TEXTURE, model.d3d9_normal_texture);
        }

        if (model.d3d9_height_texture != nullptr)
        {
            this->set_texture(parameter_type::HEIGHTMAP_TEXTURE, model.d3d9_height_texture);
        }

        if (model.d3d9_specular_texture != nullptr)
        {
            this->set_texture(parameter_type::SPECULARMAP_TEXTURE, model.d3d9_specular_texture);
        }

        if (model.d3d9_opacity_texture != nullptr)
        {
            this->set_texture(parameter_type::OPACITYMAP_TEXTURE, model.d3d9_opacity_texture);
        }
    }

    void effect::set_texture_animation(const texture::info& info)
    {
        if (this->has_parameter(parameter_type::cvTextureOffset))
        {
            vector4 scroll{};

            if (info.scroll == texture::scroll_type::none)
            {
                if (this->has_zero_offset_scale_ != 1)
                {
                    this->set_vector_unchecked(parameter_type::cvTextureOffset, scroll);
                }

                this->has_zero_offset_scale_ = true;
            }
            else
            {
                if (info.scroll == texture::scroll_type::offset_scale)
                {
                    scroll.x = static_cast<float>(info.offset_s) * -0.0009765625f; // 1 / -1024.0f
                    scroll.y = static_cast<float>(info.offset_t) * -0.0009765625f; // 1 / -1024.0f
                    scroll.z = static_cast<float>(info.scale_s) * 0.00390625f; // 1 / 256.0f
                    scroll.w = static_cast<float>(info.scale_t) * 0.00390625f; // 1 / 256.0f
                }
                else
                {
                    float delta_time = utils::get_render_time();

                    scroll.x = texture::get_scroll_s(info, delta_time);
                    scroll.y = texture::get_scroll_t(info, delta_time);
                }

                this->set_vector_unchecked(parameter_type::cvTextureOffset, scroll);

                this->has_zero_offset_scale_ = false;
            }
        }
    }

    void effect::commit_and_draw_indexed(std::uint32_t vertex_count, std::uint32_t index_start, std::uint32_t index_count, const rendering_model& model)
    {
        if (model.render_bits.multi_pass_blend)
        {
            if (world_renderer::world_detail == 3u)
            {
                directx::set_alpha_render_state(true, model.render_bits.alpha_test_ref << 4, ::D3DCMP_GREATEREQUAL);

                directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, true);

                this->effect_->CommitChanges();

                directx::device()->DrawIndexedPrimitive(::D3DPT_TRIANGLELIST, 0, 0u, vertex_count, index_start, index_count);

                directx::set_alpha_render_state(true, model.render_bits.alpha_test_ref << 4, ::D3DCMP_LESS);

                directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, true);

                directx::device()->SetRenderState(::D3DRS_SRCBLEND, ::D3DBLEND_SRCALPHA);

                directx::device()->SetRenderState(::D3DRS_DESTBLEND, ::D3DBLEND_INVSRCALPHA);

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, false);

                this->effect_->CommitChanges();

                directx::device()->DrawIndexedPrimitive(::D3DPT_TRIANGLELIST, 0, 0u, vertex_count, index_start, index_count);
            }
            else
            {
                directx::set_alpha_render_state(false, 0u, ::D3DCMP_GREATER);

                directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, true);

                directx::device()->SetRenderState(::D3DRS_SRCBLEND, ::D3DBLEND_SRCALPHA);

                directx::device()->SetRenderState(::D3DRS_DESTBLEND, ::D3DBLEND_INVSRCALPHA);

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, false);

                this->effect_->CommitChanges();

                directx::device()->DrawIndexedPrimitive(::D3DPT_TRIANGLELIST, 0, 0u, vertex_count, index_start, index_count);
            }
        }
        else
        {
            this->effect_->CommitChanges();

            directx::device()->DrawIndexedPrimitive(::D3DPT_TRIANGLELIST, 0, 0u, vertex_count, index_start, index_count);
        }
    }

    /// *************************************************************************************************************
    /// CTOR
    /// *************************************************************************************************************

    effect_world::effect_world() : 
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_world_reflect::effect_world_reflect() : 
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name)), 
        curr_splash_frame_(0u), 
        splash_fps_(20.0f)
    {
    }

    effect_world_bone::effect_world_bone() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_world_normal_map::effect_world_normal_map() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_car::effect_car() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_car_normal_map::effect_car_normal_map() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name)),
        normal_2dnoise_(nullptr)
    {
    }

    effect_world_min::effect_world_min() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_fe::effect_fe() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_fe_mask::effect_fe_mask() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_filter::effect_filter() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_screen_filter::effect_screen_filter() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_raindrop::effect_raindrop() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_visual_treatment::effect_visual_treatment() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_world_prelit::effect_world_prelit() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_particles::effect_particles() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_sky::effect_sky() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_shadow_map_mesh::effect_shadow_map_mesh() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_car_shadow_map::effect_car_shadow_map() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_world_depth::effect_world_depth() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_shadow_map_mesh_depth::effect_shadow_map_mesh_depth() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_normal_map_no_fog::effect_normal_map_no_fog() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_instance_mesh::effect_instance_mesh() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_screen_effect::effect_screen_effect() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_hdr::effect_hdr() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_ucap::effect_ucap() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_glass_reflect::effect_glass_reflect() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_water::effect_water() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_rvmpip::effect_rvmpip() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    effect_ghost_car::effect_ghost_car() :
        effect(type, effect::flags::none, shader_lib::effect_param_list.pointer(), shader_lib::find_input(name))
    {
    }

    /// *************************************************************************************************************
    /// DRAW_FULL_SCREEN_QUAD
    /// *************************************************************************************************************

    void effect_visual_treatment::draw_full_screen_quad(::IDirect3DTexture9* texture, bool invert)
    {
        struct {
            vector4 position;
            vector2 uv[8];
        } vertices[4];

        directx::device()->SetRenderState(::D3DRS_CULLMODE, ::D3DCULL_NONE);

        ::D3DSURFACE_DESC desc;

        texture->GetLevelDesc(0u, &desc);

        float w = static_cast<float>(desc.Width);
        float h = static_cast<float>(desc.Height);

        if ((desc.Width & 0x80000000) != 0)
        {
            w += 4.2949673e9f;
        }

        if ((desc.Height & 0x80000000) != 0)
        {
            h += 4.2949673e9f;
        }

        float inv_w = 0.5f / w;
        float inv_h = 0.5f / h;

        float uv00 = inv_w;
        float uv10 = inv_w + 1.0f;
        float uv20 = inv_w + 1.0f;
        float uv30 = inv_w;

        float uv11;
        float uv31;
        float uv21;
        float uv01;

        if (invert)
        {
            uv01 = inv_h + 1.0f;
            uv11 = inv_h + 1.0f;
            uv21 = inv_h;
            uv31 = inv_h;
        }
        else
        {
            uv01 = inv_h;
            uv11 = inv_h;
            uv21 = inv_h + 1.0f;
            uv31 = inv_h + 1.0f;
        }

        vertices[0].position.x = -1.0;
        vertices[0].position.y = +1.0;
        vertices[0].position.z = +0.0;
        vertices[0].position.w = +1.0;
        vertices[0].uv[0].x = uv00;
        vertices[0].uv[0].y = uv01;
        vertices[0].uv[1].x = uv00;
        vertices[0].uv[1].y = uv01;
        vertices[0].uv[2].x = uv00;
        vertices[0].uv[2].y = uv01;
        vertices[0].uv[3].x = uv00;
        vertices[0].uv[3].y = uv01;
        vertices[0].uv[4].x = uv00;
        vertices[0].uv[4].y = uv01;
        vertices[0].uv[5].x = uv00;
        vertices[0].uv[5].y = uv01;
        vertices[0].uv[6].x = uv00;
        vertices[0].uv[6].y = uv01;
        vertices[0].uv[7].x = uv00;
        vertices[0].uv[7].y = uv01;

        vertices[1].position.x = +1.0;
        vertices[1].position.y = +1.0;
        vertices[1].position.z = +0.0;
        vertices[1].position.w = +1.0;
        vertices[1].uv[0].x = uv10;
        vertices[1].uv[0].y = uv11;
        vertices[1].uv[1].x = uv10;
        vertices[1].uv[1].y = uv11;
        vertices[1].uv[2].x = uv10;
        vertices[1].uv[2].y = uv11;
        vertices[1].uv[3].x = uv10;
        vertices[1].uv[3].y = uv11;
        vertices[1].uv[4].x = uv10;
        vertices[1].uv[4].y = uv11;
        vertices[1].uv[5].x = uv10;
        vertices[1].uv[5].y = uv11;
        vertices[1].uv[6].x = uv10;
        vertices[1].uv[6].y = uv11;
        vertices[1].uv[7].x = uv10;
        vertices[1].uv[7].y = uv11;

        vertices[2].position.x = +1.0;
        vertices[2].position.y = -1.0;
        vertices[2].position.z = +0.0;
        vertices[2].position.w = +1.0;
        vertices[2].uv[0].x = uv20;
        vertices[2].uv[0].y = uv21;
        vertices[2].uv[1].x = uv20;
        vertices[2].uv[1].y = uv21;
        vertices[2].uv[2].x = uv20;
        vertices[2].uv[2].y = uv21;
        vertices[2].uv[3].x = uv20;
        vertices[2].uv[3].y = uv21;
        vertices[2].uv[4].x = uv20;
        vertices[2].uv[4].y = uv21;
        vertices[2].uv[5].x = uv20;
        vertices[2].uv[5].y = uv21;
        vertices[2].uv[6].x = uv20;
        vertices[2].uv[6].y = uv21;
        vertices[2].uv[7].x = uv20;
        vertices[2].uv[7].y = uv21;

        vertices[3].position.x = -1.0;
        vertices[3].position.y = -1.0;
        vertices[3].position.z = +0.0;
        vertices[3].position.w = +1.0;
        vertices[3].uv[0].x = uv30;
        vertices[3].uv[0].y = uv31;
        vertices[3].uv[1].x = uv30;
        vertices[3].uv[1].y = uv31;
        vertices[3].uv[2].x = uv30;
        vertices[3].uv[2].y = uv31;
        vertices[3].uv[3].x = uv30;
        vertices[3].uv[3].y = uv31;
        vertices[3].uv[4].x = uv30;
        vertices[3].uv[4].y = uv31;
        vertices[3].uv[5].x = uv30;
        vertices[3].uv[5].y = uv31;
        vertices[3].uv[6].x = uv30;
        vertices[3].uv[6].y = uv31;
        vertices[3].uv[7].x = uv30;
        vertices[3].uv[7].y = uv31;

        this->set_texture(parameter_type::DIFFUSEMAP_TEXTURE, texture);

        this->effect_->CommitChanges();

        directx::device()->DrawPrimitiveUP(::D3DPT_TRIANGLEFAN, 2u, vertices, sizeof(vertices[0]));
    }

    /// *************************************************************************************************************
    /// START
    /// *************************************************************************************************************

    void effect_world_reflect::start()
    {
        float rain_intensity = 0.0f;

        const view::instance& view = view::instance::views[render_target::current->view_id];

        if (view.rain != nullptr)
        {
            rain_intensity = view.rain->rain_intensity;
        }

        if (world_renderer::use_lowlod_pass)
        {
            this->set_technique("lowlod");
        }
        else if (rain_intensity <= 0.0f)
        {
            this->set_technique("dryroad");
        }
        else
        {
            float frame_count = static_cast<float>(std::size(this->rain_splash_));

            this->curr_splash_frame_ = static_cast<std::uint32_t>(::fmodf(this->splash_fps_ * utils::get_world_time(), frame_count));

            this->set_texture(parameter_type::MISCMAP2_TEXTURE, this->rain_splash_[this->curr_splash_frame_]);

            this->set_technique("raining_on_road");
        }

        if (view.rain != nullptr)
        {
            this->set_float(parameter_type::cfSurfaceReflection, view.rain->road_dampness);
            this->set_float(parameter_type::cfRainIntensity, view.rain->rain_intensity);
        }

        if (view.id == view_id::player1_rvm)
        {
            this->set_texture(parameter_type::MISCMAP1_TEXTURE, nullptr);
        }
        else
        {
            this->set_texture(parameter_type::MISCMAP1_TEXTURE, reflection_render_target::d3d_texture);
        }

        effect::start();

        if (world_renderer::shadow_detail > 0 && view.id == view_id::player1)
        {
            if (shadowmap_render_target::shadow_target_type == shadowmap_render_target::target_type::render_target)
            {
                this->set_texture(parameter_type::SHADOWMAP, shadowmap_render_target::render_target_texture);
            }
            else
            {
                this->set_texture(parameter_type::SHADOWMAP, shadowmap_render_target::depth_stencil_texture);
            }

            this->set_float(parameter_type::SHADOWMAPSCALE, 1.0f / static_cast<float>(shadowmap_render_target::resolution_x));
        }
    }

    void effect_world_bone::start()
    {
        this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::cube_texture);
    }

    void effect_world_normal_map::start()
    {
        this->reset_lighting_params();
    }

    void effect_car::start()
    {
        if (env_map_render_target::car_volume != nullptr)
        {
            this->set_texture(parameter_type::VOLUMEMAP_TEXTURE, env_map_render_target::car_volume);
        }

        if (game_flow::manager::instance.current_state == game_flow::state::racing)
        {
            this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::cube_texture);
            this->set_float(parameter_type::cfEnvmapPullAmount, lighting::ingame_envmap_pull_amount);
        }
        else if (game_flow::manager::instance.current_state == game_flow::state::in_frontend)
        {
            this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::fe_texture);
            this->set_float(parameter_type::cfEnvmapPullAmount, lighting::frontend_envmap_pull_amount);
        }
    }

    void effect_car_normal_map::start()
    {
        if (env_map_render_target::car_volume != nullptr)
        {
            this->set_texture(parameter_type::VOLUMEMAP_TEXTURE, env_map_render_target::car_volume);
        }

        if (game_flow::manager::instance.current_state == game_flow::state::racing)
        {
            this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::cube_texture);
            this->set_float(parameter_type::cfEnvmapPullAmount, lighting::ingame_envmap_pull_amount);
        }
        else if (game_flow::manager::instance.current_state == game_flow::state::in_frontend)
        {
            this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::fe_texture);
            this->set_float(parameter_type::cfEnvmapPullAmount, lighting::frontend_envmap_pull_amount);
        }
    }

    void effect_fe::start()
    {
    }

    void effect_fe_mask::start()
    {
    }

    void effect_filter::start()
    {
    }

    void effect_screen_filter::start()
    {
    }

    void effect_visual_treatment::start()
    {
    }

    void effect_world_prelit::start()
    {
    }

    void effect_sky::start()
    {
        this->set_texture(parameter_type::MISCMAP1_TEXTURE, this->sky_texture_);

        if (effect_sky::last_frame_updated_ != global::world_time_frames)
        {
            float update_rate = lighting::time_of_day::instance->update_rate;

            effect_sky::last_frame_updated_ = global::world_time_frames;

            effect_sky::sky_time_ticker_ += update_rate * 0.5f * global::world_time_elapsed * 0.04f;
        }

        this->set_float(parameter_type::cfTimeTicker, effect_sky::sky_time_ticker_);

        this->set_vector(parameter_type::cfSkyFogFalloff, lighting::fog_shader_params::instance->sky_fog_falloff);

        this->reset_lighting_params();
    }

    void effect_shadow_map_mesh::start()
    {
    }

    void effect_car_shadow_map::start()
    {
    }

    void effect_world_depth::start()
    {
    }

    void effect_shadow_map_mesh_depth::start()
    {
    }

    void effect_normal_map_no_fog::start()
    {
    }

    void effect_instance_mesh::start()
    {
    }

    void effect_screen_effect::start()
    {
    }

    void effect_hdr::start()
    {
    }

    void effect_ucap::start()
    {
    }

    void effect_glass_reflect::start()
    {
        ::IDirect3DCubeTexture9* refl_texture = texture::refl_cube_world_indices[0] >= texture::refl_cube_texture_count
            ? texture::refl_cube_textures[0]
            : texture::refl_cube_textures[texture::refl_cube_world_indices[0]];

        this->set_texture(parameter_type::ENVIROMAP_TEXTURE, refl_texture);

        this->set_texture(parameter_type::HEADLIGHT_TEXTURE, texture::headlights_xenon_texture->pinfo->texture);

        this->reset_lighting_params();
    }

    void effect_water::start()
    {
        this->blend_data_.curr_frame = static_cast<std::uint32_t>(::fmodf(effect_water::fps_ * utils::get_world_time(), this->blend_data_.pca_weight->frame_count));

        if (effect_water::last_frame_updated_ != global::world_time_frames)
        {
            float update_rate = lighting::time_of_day::instance->update_rate;

            effect_water::last_frame_updated_ = global::world_time_frames;

            effect_water::water_time_ticker_ += update_rate * 0.5f * global::world_time_elapsed * 0.04f;
        }

        this->set_float(parameter_type::cfTimeTicker, effect_water::water_time_ticker_);

        this->set_texture(parameter_type::MISCMAP1_TEXTURE, reflection_render_target::d3d_texture);
        this->set_texture(parameter_type::PCA_COMPONENTS0_TEXTURE, this->pca_water_textures_[0]->pinfo->texture);
        this->set_texture(parameter_type::PCA_COMPONENTS1_TEXTURE, this->pca_water_textures_[1]->pinfo->texture);

        this->set_pca_blend_data(this->blend_data_);
    }

    void effect_rvmpip::start()
    {
    }

    void effect_ghost_car::start()
    {
        this->set_texture(parameter_type::ENVIROMAP_TEXTURE, env_map_render_target::cube_texture);
    }

    /// *************************************************************************************************************
    /// LOAD_GLOBAL_TEXTURES
    /// *************************************************************************************************************

    void effect_world_reflect::load_global_textures()
    {
        char buffer[32];

        for (std::uint32_t i = 0u; i < std::size(this->rain_splash_); ++i)
        {
            ::sprintf(buffer, "RAINSPLASH%02u", i);

            std::uint32_t key = hashing::bin(buffer);

            this->rain_splash_[i] = texture::get_texture_info(key, true, false)->pinfo->texture;
        }
    }

    void effect_car_normal_map::load_global_textures()
    {
        this->normal_2dnoise_ = texture::get_texture_info(hashing::bin_const("2DNOISE_NORMAL"), true, false)->pinfo->texture;
    }

    void effect_sky::load_global_textures()
    {
        this->sky_texture_ = texture::get_texture_info(hashing::bin_const("SKY_NIGHT_CLOUDS_02"), true, false)->pinfo->texture;
    }

    void effect_glass_reflect::load_global_textures()
    {
        this->reflection_texture_ = texture::get_texture_info(hashing::bin_const("WINDOWREFLECTION"), true, false)->pinfo->texture;
    }

    void effect_water::load_global_textures()
    {
        this->pca_water_textures_[0] = texture::get_texture_info(hashing::bin_const("PCAWATER0"), true, false);
        this->pca_water_textures_[1] = texture::get_texture_info(hashing::bin_const("PCAWATER1"), true, false);
        this->blend_data_.curr_frame = 0u;
        this->blend_data_.blend = 0.0f;
        this->blend_data_.ucap_weight = nullptr;
        this->blend_data_.pca_weight = this->pca_water_textures_[0]->weights;
    }

    /// *************************************************************************************************************
    /// SHADER_LIB
    /// *************************************************************************************************************

    void shader_lib::bind_pca_channels(pca::channel_info* channels, size_t count)
    {
        for (size_t i = 0u; i < count; ++i)
        {
            pca::channel_info& info = channels[i];

            switch (info.type)
            {
                case 0:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsR);
                    break;

                case 1:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsG);
                    break;

                case 2:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsB);
                    break;

                case 3:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsX);
                    break;

                case 4:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsY);
                    break;

                case 5:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsZ);
                    break;

                case 6:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsNX);
                    break;

                case 7:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsNY);
                    break;

                case 8:
                    info.param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavPcaWeightsNZ);
                    break;

                default:
                    break;
            }
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

        shader_lib::effects_[shader_type::WorldShader] = effect_world::instance = memory::allocate<effect_world>();
        shader_lib::effects_[shader_type::WorldReflectShader] = effect_world_reflect::instance = memory::allocate<effect_world_reflect>();
        shader_lib::effects_[shader_type::WorldBoneShader] = effect_world_bone::instance = memory::allocate<effect_world_bone>();
        shader_lib::effects_[shader_type::WorldNormalMap] = effect_world_normal_map::instance = memory::allocate<effect_world_normal_map>();
        shader_lib::effects_[shader_type::CarShader] = effect_car::instance = memory::allocate<effect_car>();
        shader_lib::effects_[shader_type::CARNORMALMAP] = effect_car_normal_map::instance = memory::allocate<effect_car_normal_map>();
        shader_lib::effects_[shader_type::WorldMinShader] = effect_world_min::instance = memory::allocate<effect_world_min>();
        shader_lib::effects_[shader_type::FEShader] = effect_fe::instance = memory::allocate<effect_fe>();
        shader_lib::effects_[shader_type::FEMaskShader] = effect_fe_mask::instance = memory::allocate<effect_fe_mask>();
        shader_lib::effects_[shader_type::FilterShader] = effect_filter::instance = memory::allocate<effect_filter>();
        shader_lib::effects_[shader_type::ScreenFilterShader] = effect_screen_filter::instance = memory::allocate<effect_screen_filter>();
        shader_lib::effects_[shader_type::RainDropShader] = effect_raindrop::instance = memory::allocate<effect_raindrop>();
        shader_lib::effects_[shader_type::VisualTreatmentShader] = effect_visual_treatment::instance = memory::allocate<effect_visual_treatment>();
        shader_lib::effects_[shader_type::WorldPrelitShader] = effect_world_prelit::instance = memory::allocate<effect_world_prelit>();
        shader_lib::effects_[shader_type::ParticlesShader] = effect_particles::instance = memory::allocate<effect_particles>();
        shader_lib::effects_[shader_type::skyshader] = effect_sky::instance = memory::allocate<effect_sky>();
        shader_lib::effects_[shader_type::shadow_map_mesh] = effect_shadow_map_mesh::instance = memory::allocate<effect_shadow_map_mesh>();
        shader_lib::effects_[shader_type::CarShadowMapShader] = effect_car_shadow_map::instance = memory::allocate<effect_car_shadow_map>();
        shader_lib::effects_[shader_type::WorldDepthShader] = effect_world_depth::instance = memory::allocate<effect_world_depth>();
        shader_lib::effects_[shader_type::shadow_map_mesh_depth] = effect_shadow_map_mesh_depth::instance = memory::allocate<effect_shadow_map_mesh_depth>();
        shader_lib::effects_[shader_type::NormalMapNoFog] = effect_normal_map_no_fog::instance = memory::allocate<effect_normal_map_no_fog>();
        shader_lib::effects_[shader_type::InstanceMesh] = effect_instance_mesh::instance = memory::allocate<effect_instance_mesh>();
        shader_lib::effects_[shader_type::ScreenEffectShader] = effect_screen_effect::instance = memory::allocate<effect_screen_effect>();
        shader_lib::effects_[shader_type::HDRShader] = effect_hdr::instance = memory::allocate<effect_hdr>();
        shader_lib::effects_[shader_type::UCAP] = effect_ucap::instance = memory::allocate<effect_ucap>();
        shader_lib::effects_[shader_type::GLASS_REFLECT] = effect_glass_reflect::instance = memory::allocate<effect_glass_reflect>();
        shader_lib::effects_[shader_type::WATER] = effect_water::instance = memory::allocate<effect_water>();
        shader_lib::effects_[shader_type::RVMPIP] = effect_rvmpip::instance = memory::allocate<effect_rvmpip>();
        shader_lib::effects_[shader_type::GHOSTCAR] = effect_ghost_car::instance = memory::allocate<effect_ghost_car>();

        pca::ucap_bind = shader_lib::bind_ucap_weights;

        shader_lib::initilaized_ = true;
    }

    void shader_lib::close()
    {
        for (std::uint32_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            effect* ptr = shader_lib::effects_[i];

            if (ptr != nullptr)
            {
                memory::destroy(ptr);
            }
        }
    }

    void shader_lib::reset()
    {
        for (std::uint32_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            if (effect* ptr = shader_lib::effects_[i])
            {
                ptr->reset();
            }
        }
    }

    void shader_lib::reinit()
    {
        for (std::uint32_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            if (effect* ptr = shader_lib::effects_[i])
            {
                ptr->reinitialize();
            }
        }

        shader_lib::recompute_techniques_by_detail(directx::shader_detail);
    }

    auto shader_lib::find_param_index(std::uint32_t key) -> const effect::param_index_pair*
    {
        const auto& list = shader_lib::effect_param_list;

        void* entry = utils::scan_hash_table_key32(key, list.begin(), list.length(), offsetof(effect::param_index_pair, key), sizeof(effect::param_index_pair));

        return reinterpret_cast<const effect::param_index_pair*>(entry);
    }

    auto shader_lib::find_input(shader_type type) -> const effect::input*
    {
        return &shader_lib::inputs_[type];
    }

    auto shader_lib::find_input(const char* name) -> const effect::input*
    {
        for (size_t i = 0u; i < shader_lib::inputs_.length(); ++i)
        {
            if (!::strcmp(name, shader_lib::inputs_[i].effect_name))
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
            if (effect* ptr = shader_lib::effects_[i])
            {
                ptr->lose_device();
            }
        }
    }

    void shader_lib::reconnect_device()
    {
        for (size_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            if (effect* ptr = shader_lib::effects_[i])
            {
                ptr->get_device();

                ptr->connect_parameters();
            }
        }
    }

    void shader_lib::release()
    {
        for (size_t i = 0u; i < shader_lib::effects_.length(); ++i)
        {
            if (effect* ptr = shader_lib::effects_[i])
            {
                ptr->release_effect();
            }
        }
    }

    void shader_lib::end_effect(effect& eff)
    {
        eff.end_effect_pass();
        eff.end();
        eff.end_effect();

        effect::set_current_effect(nullptr);
    }

    void shader_lib::recompute_techniques_by_detail(std::uint32_t detail_level)
    {
        for (shader_type type :
        {
            shader_type::WorldShader,
            shader_type::WorldReflectShader,
            shader_type::WorldNormalMap,
            shader_type::CarShader,
            shader_type::CARNORMALMAP,
            shader_type::ParticlesShader,
            shader_type::GLASS_REFLECT,
            shader_type::WATER,
            shader_type::GHOSTCAR,
        })
        {
            if (shader_lib::effects_[type] != nullptr)
            {
                shader_lib::effects_[type]->recompute_techniques_by_detail(detail_level);
            }
        }
    }

    void shader_lib::bind_pca_weights(pca::weights& weights)
    {
        shader_lib::bind_pca_channels(weights.channel_infos, weights.channel_count);
    }

    void shader_lib::bind_ucap_weights(pca::ucap_frame_weights& weights)
    {
        weights.feature_heights_param_handle = static_cast<std::uint32_t>(effect::parameter_type::cavFeatureHeights);

        shader_lib::bind_pca_channels(weights.channel_infos, weights.channel_count);
    }
}
