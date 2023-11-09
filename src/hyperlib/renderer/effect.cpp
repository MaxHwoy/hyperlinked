#include <hyperlib/utils/utils.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    effect::technique::technique() : name{}, technique_index(-1), detail_level(0u)
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

    effect::effect(shader_type type, effect::flags flags, effect::param_index_pair* indices, const effect::input* input) : 
        id_(type), 
        index_pairs_(indices), 
        unsupported_table_{}, 
        supported_table_{}
    {
        for (size_t i = 0u; i < std::size(this->params_); ++i)
        {
            parameter& param = this->params_[i];

            param.name[0] = 0;
            param.key = 0u;
            param.handle = nullptr;
        }

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

    void effect::handle_material_data(const light_material::instance* material, draw_flags flags)
    {
        if (material != nullptr && this->has_parameter(parameter_type::cvDiffuseMin) && material != this->last_used_light_material_)
        {
            this->last_used_light_material_ = material;

            const light_material::data& data = material->material;

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
        if (&local_to_world != effect::last_submitted_matrix_ || shader_lib::current_effect != effect::last_submitted_effect_ || view.id != effect::last_submitted_view_id_)
        {
            const matrix4x4* view_matrix = &view.view_matrix;
            const matrix4x4* proj_matrix = &view.projection_matrix;
            const matrix4x4* view_proj_matrix = use_nonjittered
                ? &view.non_jittered_view_projection_matrix
                : &view.view_projection_matrix;

            if (view.id == effect::last_submitted_view_id_)
            {
                if (shader_lib::current_effect != effect::last_submitted_effect_ && !this->active_)
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

            effect::last_submitted_effect_ = shader_lib::current_effect;
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
        vertices[0].uv[0] = uv00;
        vertices[0].uv[1] = uv01;

        vertices[1].position.x = +1.0f;
        vertices[1].position.y = +1.0f;
        vertices[1].position.z = +0.0f;
        vertices[1].color = color32::white();
        vertices[1].uv[0] = uv10;
        vertices[1].uv[1] = uv11;

        vertices[2].position.x = +1.0f;
        vertices[2].position.y = -1.0f;
        vertices[2].position.z = +0.0f;
        vertices[2].color = color32::white();
        vertices[2].uv[0] = uv20;
        vertices[2].uv[1] = uv21;

        vertices[3].position.x = -1.0f;
        vertices[3].position.y = -1.0f;
        vertices[3].position.z = +0.0f;
        vertices[3].color = color32::white();
        vertices[3].uv[0] = uv30;
        vertices[3].uv[1] = uv31;

        this->set_texture(parameter_type::DIFFUSEMAP_TEXTURE, texture);

        this->effect_->CommitChanges();
        
        directx::device()->DrawPrimitiveUP(::D3DPT_TRIANGLEFAN, 2u, vertices, sizeof(vertices[0]));
    }

    void effect::start()
    {
        this->set_float(parameter_type::cfSpecularPower, lighting::default_spec_power);

        if (this->has_parameter(parameter_type::cvVertexPowerBrightness))
        {
            lighting::ingame_light_params.y = lighting::default_ingame_light_y;
            lighting::ingame_light_params.w = lighting::default_ingame_light_w;

            const auto in_frontend = game_flow::manager::instance.current_state == game_flow::state::in_frontend;

#if defined(ABOMINATOR)
            // copy the vector.
            auto vector = (in_frontend) ? lighting::frontend_light_params : lighting::ingame_light_params;
#else
            const auto& vector = (in_frontend) ? lighting::frontend_light_params : lighting::ingame_light_params;
#endif

#if defined(ABOMINATOR)
            const auto render_target = render_target::current;

            if (render_target->view_id >= view_id::env_z_pos)
            {
                if (in_frontend)
                {
                    vector.x += 2.5f;
                    vector.y -= 1.0f;
                }
                else
                {
                    vector.y -= 0.4f;
                }
            }
#endif
            this->set_vector_unchecked(parameter_type::cvVertexPowerBrightness, vector);
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
                    this->low_lod_technique_number_ = i;
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
                if (length == i->name.length() && ::memcmp(name, i->name, length))
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

    void effect_world_reflect::start()
    {
        float rain_intensity = 0.0f;

        const view::instance& view = view::instance::views[render_target::current->view_id];

        if (view.rain != nullptr)
        {
            rain_intensity = view.rain->rain_intensity;
        }

        if (renderer::use_lowlod_pass)
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

        if (renderer::shadow_detail > 0 && view.id == view_id::player1)
        {
            if (shadowmap_render_target::shadow_target_type == shadowmap_render_target::target_type::render_target)
            {
                this->set_texture(parameter_type::SHADOWMAP, shadowmap_render_target::render_target_cubemap);
            }
            else
            {
                this->set_texture(parameter_type::SHADOWMAP, shadowmap_render_target::depth_stencil_cubemap);
            }

            this->set_float(parameter_type::SHADOWMAPSCALE, 1.0f / static_cast<float>(shadowmap_render_target::resolution_x));
        }
    }

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
        eff.end();
        eff.end_effect();

        shader_lib::current_effect = nullptr;
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
}
