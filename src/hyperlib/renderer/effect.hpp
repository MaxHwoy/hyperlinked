#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/pca.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/collections/string.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/lighting.hpp>

namespace hyper
{
    enum class shader_type : std::uint32_t
    {
        WorldShader,
        WorldReflectShader,
        WorldBoneShader,
        WorldNormalMap,
        CarShader,
        CARNORMALMAP,
        WorldMinShader,
        FEShader,
        FEMaskShader,
        FilterShader,
        ScreenFilterShader,
        RainDropShader,
        VisualTreatmentShader,
        WorldPrelitShader,
        ParticlesShader,
        skyshader,
        shadow_map_mesh,
        CarShadowMapShader,
        WorldDepthShader,
        shadow_map_mesh_depth,
        NormalMapNoFog,
        InstanceMesh,
        ScreenEffectShader,
        HDRShader,
        UCAP,
        GLASS_REFLECT,
        WATER,
        RVMPIP,
        GHOSTCAR,
        count,
        first = 0,
    };

    enum class vertex_type : std::uint32_t
    {
        float1,
        float2,
        float3,
        float4,
        color,
        ubyte4,
        short4n,
        uint1,
        none,
        count,
    };

    enum class vertex_component : std::uint32_t
    {
        position,
        normal,
        color,
        uv,
        uv1,
        uv2,
        uv3,
        weight,
        index,
        tangent,
        pca_component1,
        pca_component2,
        pca_component3,
        pca_component4,
        pca_component5,
        pca_component6,
        pca_component7,
        pca_component8,
        uv4,
        uv5,
        uv6,
        uv7,
        count,
    };

    struct rendering_model;

    class effect
    {
    public:
        enum class flags : std::uint32_t
        {
            none                 = 0u,
            wants_alpha_sort     = 1u << 0,
            wants_pass_preflight = 1u << 1,
            wants_draw_preflight = 1u << 2,
            wants_current_end    = 1u << 3,
            wants_custom_draw    = 1u << 4,
            param_block_start    = 1u << 8,
        };

        struct parameter
        {
            char name[0x20];
            std::uint32_t key;
            ::D3DXHANDLE handle;
        };

        struct technique
        {
            technique();
            technique(const technique& other);
            technique(technique&& other);
            technique(const char* name, std::uint32_t index, std::uint32_t level);
            auto operator=(const technique& other) -> technique&;
            auto operator=(technique&& other) -> technique&;

            vector_string name;
            std::uint32_t technique_index;
            std::uint32_t detail_level;
        };

        struct table : public eastl::vector<technique>
        {
        };

        struct param_index_pair
        {
            std::uint32_t key;
            std::uint32_t index;
        };

        enum class parameter_type : std::uint32_t
        {
            AmbientCoeff,
            AmbientColour,
            BlendState,
            BaseAddressU,
            BaseAddressV,
            BaseMagTextureFilter,
            BaseMinTextureFilter,
            BaseMipTextureFilter,
            BaseTextureFilterParam,
            camBlendMatrices,
            cavLightColours,
            cavLightDirections,
            cavLightPositions,
            cbDepthOfFieldEnabled,
            cbDrawDepthOfField,
            cbUVESOverCliffEnabled,
            cfBloomScale,
            cfBrightPassThreshold,
            cfBrightness,
            cvBaseAlphaRef,
            cvPowers,
            cvClampAndScales,
            cvFlakes,
            cvVinylScales,
            cvTextureOffset,
            cfVisualEffectBrightness,
            cfVisualEffectVignette,
            cmPrevWorldViewProj,
            cmWorldMat,
            cmWorldView,
            cavPcaWeightsNX,
            cavPcaWeightsNY,
            cavPcaWeightsNZ,
            ColourCubeBlendFactor,
            ColorWriteMode,
            CLOUDSCROLL,
            Cull_Mode,
            CURVE_COEFFS_0,
            DIFFUSEMAP_TEXTURE,
            cbHDREnabled,
            cfVignetteScale,
            cvBlurParams,
            cvVisualEffectFadeColour,
            cfTimeTicker,
            cvDepthOfFieldParams,
            cvDiffuseMin,
            cvDiffuseRange,
            cvHeadlight1Colour,
            cvHeadlight2Colour,
            DIFFUSETEX,
            DEPTHBUFFER_TEXTURE,
            DISPLACEMAP,
            cvEnvmapRange,
            cvEnvmapMin,
            cfEnvmapPower,
            cfEnvmapPullAmount,
            ENVIROMAP_TEXTURE,
            FFSKYBLENDFACTOR,
            FFSKYCOLOR,
            cavFeatureHeights,
            FECOLORWRITEFLAG,
            FEMaskMap,
            FilterBlend,
            FilterTexture0,
            FilterTexture1,
            FilterTexture2,
            FilterTexture3,
            FilterWeights,
            FocalRange,
            FogColor,
            cfFogEnable,
            cvFogColour,
            cfSkyFogFalloff,
            cvFogValue,
            GAINMAP,
            cavHarmonicCoeff,
            HEADLIGHT_CLIP_TEXTURE,
            cmHeadlight1WVP,
            HEIGHTMAP_TEXTURE,
            INSTANCEMATRICES,
            INSTANCECOLORS,
            INVSHADOWSTRENGTH,
            cvLocalEyePos,
            cvLocalLightVec,
            cmLocalColourMatrix,
            cmLocalDirectionMatrix,
            cmLocalPositionMatrix,
            cfMetallicScale,
            cfMiddleGray,
            MISCMAP1_TEXTURE,
            MISCMAP2_TEXTURE,
            MISCMAP3_TEXTURE,
            MISCMAP4_TEXTURE,
            MISCMAP5_TEXTURE,
            MISCMAP6_TEXTURE,
            MOTIONBLUR_TEXTURE,
            NORMALMAP_TEXTURE,
            OPACITYMAP_TEXTURE,
            OVERBRIGHTGREYSCALE,
            OVERBRIGHTOFFSET,
            cavPcaWeightsR,
            cavPcaWeightsG,
            cavPcaWeightsB,
            cavPcaWeightsX,
            cavPcaWeightsY,
            cavPcaWeightsZ,
            RAINDROPOFFSET,
            RAINDROPALPHA,
            cfRainIntensity,
            cavSampleOffsets,
            cavSampleWeights,
            SCREENOFFSET,
            SHADOWCOLORMAP,
            SHADOWCURRENTGEN_COLOR,
            SHADOWMAP,
            SHADOWMAPSCALE,
            SHADOWTRANSFORM,
            cfSpecularHotSpot,
            SPECULARMAP_TEXTURE,
            cvSpecularMin,
            cfSpecularPower,
            cvSpecularRange,
            SPLINE,
            SKYRGBA,
            SKY_TEXTURE_MISC_1,
            SKY_TEXTURE_MISC_2,
            SKY_TEXTURE_MISC_3,
            SKY_DIFFUSESCALE,
            cfSurfaceReflection,
            TextureOffsetMatrix,
            TintColor,
            VISUAL_EFFECT_BRIGHTNESS,
            VOLUMEMAP_TEXTURE,
            WindowReflection,
            WorldView,
            WorldViewProj,
            HEADLIGHT_TEXTURE,
            PCA_COMPONENTS0_TEXTURE,
            PCA_COMPONENTS1_TEXTURE,
            cvVertexPowerBrightness,
            cavWorldHeadlightDirection,
            cavWorldHeadlightUpDirection,
            cavWorldHeadlightPosition,
            HEADLIGHT_TEXTURE_OLD,
            BLENDVOLUMEMAP_TEXTURE,
            cvVisualTreatmentParams,
            cmWorldMatTranspose,
            count,
            first = 0,
        };
        
        struct stream_channel
        {
            vertex_type type;
            vertex_component component;
        };

        struct input
        {
            const char* effect_name;
            stream_channel channels[0x10];
            const char* filename;
            const char* resource;
            std::uint32_t unknown;
        };

    private:
        struct linked_technique : public linked_node<linked_technique>
        {
            technique tech;
        };

    private:
        void store_param_by_key(::LPCSTR name, ::D3DXHANDLE handle);

    public:
        virtual ~effect();
        virtual void handle_material_data(const light_material::instance* material, draw_flags flags);
        virtual void set_transforms(const matrix4x4& local_to_world, const struct render_view& view, bool use_nonjittered);
        virtual void draw_full_screen_quad(::IDirect3DTexture9* texture, bool invert);
        virtual void start();
        virtual void end();
        virtual void load_global_textures();

        effect(shader_type type, effect::flags flags, const effect::param_index_pair* indices, const effect::input* input);

        void initialize(const effect::input* input);

        void connect_parameters();

        void reset_filter_params();

        void reset_lighting_params();

        void load_effect_from_buffer(const effect::input* input);

        void recompute_techniques_by_detail(std::uint32_t detail_level);

        auto find_techique(const char* name) -> technique*;

        void set_technique(const char* name);

        void set_pca_blend_data(const pca::blend_data& data);

        void set_blend_matrices(const matrix4x4* blend_matrices, const geometry::mesh_entry& entry);

        void set_light_context(const lighting::dynamic_context* context, const matrix4x4* local_to_world);

        void set_diffuse_map(::IDirect3DTexture9* texture, const rendering_model& model);

        inline auto id() const -> shader_type
        {
            return this->id_;
        }

        inline bool active() const
        {
            return this->active_;
        }

        inline bool has_low_lod_technique() const
        {
            return this->low_lod_technique_number_ > 0u;
        }

        inline void lose_device()
        {
            this->effect_->OnLostDevice();
        }

        inline void release_effect()
        {
            if (this->effect_ != nullptr)
            {
                this->effect_->Release();

                this->effect_ = nullptr;
            }
        }

        inline void end_effect_pass()
        {
            this->effect_->EndPass();
        }

        inline void end_effect()
        {
            this->effect_->End();
        }

        inline bool has_parameter(parameter_type type)
        {
            return this->params_[static_cast<std::uint32_t>(type)].handle != nullptr;
        }

        inline void set_int(parameter_type type, std::int32_t value)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetInt(handle, value);
            }
        }

        inline void set_int_unchecked(parameter_type type, std::int32_t value)
        {
            this->effect_->SetInt(this->params_[static_cast<std::uint32_t>(type)].handle, value);
        }

        inline void set_float(parameter_type type, float value)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetFloat(handle, value);
            }
        }

        inline void set_float_unchecked(parameter_type type, float value)
        {
            this->effect_->SetFloat(this->params_[static_cast<std::uint32_t>(type)].handle, value);
        }

        inline void set_vector(parameter_type type, const vector4& vector)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetVector(handle, reinterpret_cast<const ::D3DXVECTOR4*>(&vector));
            }
        }

        inline void set_vector_unchecked(parameter_type type, const vector4& vector)
        {
            this->effect_->SetVector(this->params_[static_cast<std::uint32_t>(type)].handle, reinterpret_cast<const ::D3DXVECTOR4*>(&vector));
        }

        inline void set_matrix(parameter_type type, const matrix4x4& matrix)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetMatrix(handle, reinterpret_cast<const ::D3DXMATRIX*>(&matrix));
            }
        }

        inline void set_matrix_unchecked(parameter_type type, const matrix4x4& matrix)
        {
            this->effect_->SetMatrix(this->params_[static_cast<std::uint32_t>(type)].handle, reinterpret_cast<const ::D3DXMATRIX*>(&matrix));
        }

        inline void set_vector_array(parameter_type type, const vector4* array, std::uint32_t count)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetVectorArray(handle, reinterpret_cast<const ::D3DXVECTOR4*>(array), count);
            }
        }

        inline void set_vector_array_unchecked(parameter_type type, const vector4* array, std::uint32_t count)
        {
            this->effect_->SetVectorArray(this->params_[static_cast<std::uint32_t>(type)].handle, reinterpret_cast<const ::D3DXVECTOR4*>(array), count);
        }

        inline void set_matrix_array(parameter_type type, const matrix4x4* array, std::uint32_t count)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetMatrixArray(handle, reinterpret_cast<const ::D3DXMATRIX*>(array), count);
            }
        }

        inline void set_matrix_array_unchecked(parameter_type type, const matrix4x4* array, std::uint32_t count)
        {
            this->effect_->SetMatrixArray(this->params_[static_cast<std::uint32_t>(type)].handle, reinterpret_cast<const ::D3DXMATRIX*>(array), count);
        }

        inline void set_texture(parameter_type type, IDirect3DBaseTexture9* texture)
        {
            if (::D3DXHANDLE handle = this->params_[static_cast<std::uint32_t>(type)].handle)
            {
                this->effect_->SetTexture(handle, texture);
            }
        }

        constexpr static inline auto get_parameter_name(parameter_type type) -> const char*
        {
            return effect::parameter_names_[static_cast<std::uint32_t>(type)];
        }

    private:
        shader_type id_;
        std::uint32_t stride_;
        std::uint32_t main_technique_number_;
        ::D3DXHANDLE main_technique_handle_;
        std::uint32_t pass_count_;
        std::int32_t __3__;
        std::int32_t __4__;
        std::int32_t __5__;
        std::int32_t __6__;
        std::int32_t __7__;
        std::int32_t __8__;
        std::int32_t __9__;
        std::int32_t __10__;
        std::int32_t __11__;
        std::int32_t __14__;
        bool active_;

    protected:
        ::ID3DXEffect* effect_;

    private:
        ::IDirect3DVertexDeclaration9* vertex_decl_;
        parameter params_[static_cast<std::uint32_t>(parameter_type::count)];
        table unsupported_table_;
        table supported_table_;
        const param_index_pair* index_pairs_;
        const char* name_;
        flags flags_;
        bool has_main_technique_;
        std::int32_t low_lod_technique_number_;
        std::int32_t has_zero_offset_scale_;
        std::int32_t has_fog_disabled_;
        const light_material::instance* last_used_light_material_;
        const lighting::dynamic_context* last_used_light_context_;

    private:
        static inline const effect*& last_submitted_effect_ = *reinterpret_cast<const effect**>(0x00B1F2E4);

        static inline const matrix4x4* last_submitted_matrix_ = *reinterpret_cast<const matrix4x4**>(0x00B1F2E8);

        static inline view_id& last_submitted_view_id_ = *reinterpret_cast<view_id*>(0x00A651A4);

        static inline const char* parameter_names_[static_cast<size_t>(parameter_type::count)] = {
            "AmbientCoeff",
            "AmbientColour",
            "BlendState",
            "BaseAddressU",
            "BaseAddressV",
            "BaseMagTextureFilter",
            "BaseMinTextureFilter",
            "BaseMipTextureFilter",
            "BaseTextureFilterParam",
            "camBlendMatrices",
            "cavLightColours",
            "cavLightDirections",
            "cavLightPositions",
            "cbDepthOfFieldEnabled",
            "cbDrawDepthOfField",
            "cbUVESOverCliffEnabled",
            "cfBloomScale",
            "cfBrightPassThreshold",
            "cfBrightness",
            "cvBaseAlphaRef",
            "cvPowers",
            "cvClampAndScales",
            "cvFlakes",
            "cvVinylScales",
            "cvTextureOffset",
            "cfVisualEffectBrightness",
            "cfVisualEffectVignette",
            "cmPrevWorldViewProj",
            "cmWorldMat",
            "cmWorldView",
            "cavPcaWeightsNX",
            "cavPcaWeightsNY",
            "cavPcaWeightsNZ",
            "ColourCubeBlendFactor",
            "ColorWriteMode",
            "CLOUDSCROLL",
            "Cull_Mode",
            "CURVE_COEFFS_0",
            "DIFFUSEMAP_TEXTURE",
            "cbHDREnabled",
            "cfVignetteScale",
            "cvBlurParams",
            "cvVisualEffectFadeColour",
            "cfTimeTicker",
            "cvDepthOfFieldParams",
            "cvDiffuseMin",
            "cvDiffuseRange",
            "cvHeadlight1Colour",
            "cvHeadlight2Colour",
            "DIFFUSETEX",
            "DEPTHBUFFER_TEXTURE",
            "DISPLACEMAP",
            "cvEnvmapRange",
            "cvEnvmapMin",
            "cfEnvmapPower",
            "cfEnvmapPullAmount",
            "ENVIROMAP_TEXTURE",
            "FFSKYBLENDFACTOR",
            "FFSKYCOLOR",
            "cavFeatureHeights",
            "FECOLORWRITEFLAG",
            "FEMaskMap",
            "FilterBlend",
            "FilterTexture0",
            "FilterTexture1",
            "FilterTexture2",
            "FilterTexture3",
            "FilterWeights",
            "FocalRange",
            "FogColor",
            "cfFogEnable",
            "cvFogColour",
            "cfSkyFogFalloff",
            "cvFogValue",
            "GAINMAP",
            "cavHarmonicCoeff",
            "HEADLIGHT_CLIP_TEXTURE",
            "cmHeadlight1WVP",
            "HEIGHTMAP_TEXTURE",
            "INSTANCEMATRICES",
            "INSTANCECOLORS",
            "INVSHADOWSTRENGTH",
            "cvLocalEyePos",
            "cvLocalLightVec",
            "cmLocalColourMatrix",
            "cmLocalDirectionMatrix",
            "cmLocalPositionMatrix",
            "cfMetallicScale",
            "cfMiddleGray",
            "MISCMAP1_TEXTURE",
            "MISCMAP2_TEXTURE",
            "MISCMAP3_TEXTURE",
            "MISCMAP4_TEXTURE",
            "MISCMAP5_TEXTURE",
            "MISCMAP6_TEXTURE",
            "MOTIONBLUR_TEXTURE",
            "NORMALMAP_TEXTURE",
            "OPACITYMAP_TEXTURE",
            "OVERBRIGHTGREYSCALE",
            "OVERBRIGHTOFFSET",
            "cavPcaWeightsR",
            "cavPcaWeightsG",
            "cavPcaWeightsB",
            "cavPcaWeightsX",
            "cavPcaWeightsY",
            "cavPcaWeightsZ",
            "RAINDROPOFFSET",
            "RAINDROPALPHA",
            "cfRainIntensity",
            "cavSampleOffsets",
            "cavSampleWeights",
            "SCREENOFFSET",
            "SHADOWCOLORMAP",
            "SHADOWCURRENTGEN_COLOR",
            "SHADOWMAP",
            "SHADOWMAPSCALE",
            "SHADOWTRANSFORM",
            "cfSpecularHotSpot",
            "SPECULARMAP_TEXTURE",
            "cvSpecularMin",
            "cfSpecularPower",
            "cvSpecularRange",
            "SPLINE",
            "SKYRGBA",
            "SKY_TEXTURE_MISC_1",
            "SKY_TEXTURE_MISC_2",
            "SKY_TEXTURE_MISC_3",
            "SKY_DIFFUSESCALE",
            "cfSurfaceReflection",
            "TextureOffsetMatrix",
            "TintColor",
            "VISUAL_EFFECT_BRIGHTNESS",
            "VOLUMEMAP_TEXTURE",
            "WindowReflection",
            "WorldView",
            "WorldViewProj",
            "HEADLIGHT_TEXTURE",
            "PCA_COMPONENTS0_TEXTURE",
            "PCA_COMPONENTS1_TEXTURE",
            "cvVertexPowerBrightness",
            "cavWorldHeadlightDirection",
            "cavWorldHeadlightUpDirection",
            "cavWorldHeadlightPosition",
            "HEADLIGHT_TEXTURE_OLD",
            "BLENDVOLUMEMAP_TEXTURE",
            "cvVisualTreatmentParams",
            "cmWorldMatTranspose",
        };
    };

    class effect_world : public effect
    {
    public:
        effect_world();

    public:
        constexpr static inline shader_type type = shader_type::WorldShader;

        constexpr static inline const char* name = "WorldShader";

        static inline effect_world*& instance = *reinterpret_cast<effect_world**>(0x00B42FB8);
    };

    class effect_world_reflect : public effect
    {
    public:
        effect_world_reflect();

    public:
        void start() override;
        void load_global_textures() override;

    private:
        ::IDirect3DTexture9* rain_splash_[16];
        std::uint32_t curr_splash_frame_;
        float last_time_updated_;
        float splash_fps_;

    public:
        constexpr static inline shader_type type = shader_type::WorldReflectShader;

        constexpr static inline const char* name = "WorldReflectShader";

        static inline effect_world_reflect*& instance = *reinterpret_cast<effect_world_reflect**>(0x00B42FBC);
    };

    class effect_world_bone : public effect
    {
    public:
        effect_world_bone();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::WorldBoneShader;

        constexpr static inline const char* name = "WorldBoneShader";

        static inline effect_world_bone*& instance = *reinterpret_cast<effect_world_bone**>(0x00B42FC0);
    };

    class effect_world_normal_map : public effect
    {
    public:
        effect_world_normal_map();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::WorldNormalMap;

        constexpr static inline const char* name = "WorldNormalMap";

        static inline effect_world_normal_map*& instance = *reinterpret_cast<effect_world_normal_map**>(0x00B42FC4);
    };

    class effect_car : public effect
    {
    public:
        effect_car();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::CarShader;

        constexpr static inline const char* name = "CarShader";

        static inline effect_car*& instance = *reinterpret_cast<effect_car**>(0x00B42FC8);
    };

    class effect_car_normal_map : public effect
    {
    public:
        effect_car_normal_map();

    public:
        void start() override;
        void load_global_textures() override;

    private:
        ::IDirect3DTexture9* normal_2dnoise_;

    public:
        constexpr static inline shader_type type = shader_type::CARNORMALMAP;

        constexpr static inline const char* name = "CARNORMALMAP";

        static inline effect_car_normal_map*& instance = *reinterpret_cast<effect_car_normal_map**>(0x00B42FCC);
    };

    class effect_world_min : public effect
    {
    public:
        effect_world_min();

    public:
        constexpr static inline shader_type type = shader_type::WorldMinShader;

        constexpr static inline const char* name = "WorldMinShader";

        static inline effect_world_min*& instance = *reinterpret_cast<effect_world_min**>(0x00B42FD0);
    };

    class effect_fe : public effect
    {
    public:
        effect_fe();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::FEShader;

        constexpr static inline const char* name = "FEShader";

        static inline effect_fe*& instance = *reinterpret_cast<effect_fe**>(0x00B42FD4);
    };

    class effect_fe_mask : public effect
    {
    public:
        effect_fe_mask();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::FEMaskShader;

        constexpr static inline const char* name = "FEMaskShader";

        static inline effect_fe_mask*& instance = *reinterpret_cast<effect_fe_mask**>(0x00B42FD8);
    };

    class effect_filter : public effect
    {
    public:
        effect_filter();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::FilterShader;

        constexpr static inline const char* name = "FilterShader";

        static inline effect_filter*& instance = *reinterpret_cast<effect_filter**>(0x00B42FDC);
    };

    class effect_screen_filter : public effect
    {
    public:
        effect_screen_filter();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::ScreenFilterShader;

        constexpr static inline const char* name = "ScreenFilterShader";

        static inline effect_screen_filter*& instance = *reinterpret_cast<effect_screen_filter**>(0x00B42FE0);
    };

    class effect_raindrop : public effect
    {
    public:
        effect_raindrop();

    public:
        constexpr static inline shader_type type = shader_type::RainDropShader;

        constexpr static inline const char* name = "RainDropShader";

        static inline effect_raindrop*& instance = *reinterpret_cast<effect_raindrop**>(0x00B42FE4);
    };

    class effect_visual_treatment : public effect
    {
    public:
        effect_visual_treatment();

    public:
        void draw_full_screen_quad(::IDirect3DTexture9* texture, bool invert) override;
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::VisualTreatmentShader;

        constexpr static inline const char* name = "VisualTreatmentShader";

        static inline effect_visual_treatment*& instance = *reinterpret_cast<effect_visual_treatment**>(0x00B42FE8);
    };

    class effect_world_prelit : public effect
    {
    public:
        effect_world_prelit();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::WorldPrelitShader;

        constexpr static inline const char* name = "WorldPrelitShader";

        static inline effect_world_prelit*& instance = *reinterpret_cast<effect_world_prelit**>(0x00B42FEC);
    };

    class effect_particles : public effect
    {
    public:
        effect_particles();

    public:
        constexpr static inline shader_type type = shader_type::ParticlesShader;

        constexpr static inline const char* name = "ParticlesShader";

        static inline effect_particles*& instance = *reinterpret_cast<effect_particles**>(0x00B42FF0);
    };

    class effect_sky : public effect
    {
    public:
        effect_sky();

    public:
        void start() override;
        void load_global_textures() override;

    private:
        ::IDirect3DTexture9* sky_texture_;

    public:
        constexpr static inline shader_type type = shader_type::skyshader;

        constexpr static inline const char* name = "skyshader";

        static inline effect_sky*& instance = *reinterpret_cast<effect_sky**>(0x00B42FF4);

    private:
        static inline float& sky_time_ticker_ = *reinterpret_cast<float*>(0x00B4CFB0);

        static inline std::int32_t& last_frame_updated_ = *reinterpret_cast<std::int32_t*>(0x00B4CFB4);
    };

    class effect_shadow_map_mesh : public effect
    {
    public:
        effect_shadow_map_mesh();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::shadow_map_mesh;

        constexpr static inline const char* name = "shadow_map_mesh";

        static inline effect_shadow_map_mesh*& instance = *reinterpret_cast<effect_shadow_map_mesh**>(0x00B42FF8);
    };

    class effect_car_shadow_map : public effect
    {
    public:
        effect_car_shadow_map();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::CarShadowMapShader;

        constexpr static inline const char* name = "CarShadowMapShader";

        static inline effect_car_shadow_map*& instance = *reinterpret_cast<effect_car_shadow_map**>(0x00B42FFC);
    };

    class effect_world_depth : public effect
    {
    public:
        effect_world_depth();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::WorldDepthShader;

        constexpr static inline const char* name = "WorldDepthShader";

        static inline effect_world_depth*& instance = *reinterpret_cast<effect_world_depth**>(0x00B43000);
    };

    class effect_shadow_map_mesh_depth : public effect
    {
    public:
        effect_shadow_map_mesh_depth();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::shadow_map_mesh_depth;

        constexpr static inline const char* name = "shadow_map_mesh_depth";

        static inline effect_shadow_map_mesh_depth*& instance = *reinterpret_cast<effect_shadow_map_mesh_depth**>(0x00B43004);
    };

    class effect_normal_map_no_fog : public effect
    {
    public:
        effect_normal_map_no_fog();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::NormalMapNoFog;

        constexpr static inline const char* name = "NormalMapNoFog";

        static inline effect_normal_map_no_fog*& instance = *reinterpret_cast<effect_normal_map_no_fog**>(0x00B43008);
    };

    class effect_instance_mesh : public effect
    {
    public:
        effect_instance_mesh();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::InstanceMesh;

        constexpr static inline const char* name = "InstanceMesh";

        static inline effect_instance_mesh*& instance = *reinterpret_cast<effect_instance_mesh**>(0x00B4300C);
    };

    class effect_screen_effect : public effect
    {
    public:
        effect_screen_effect();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::ScreenEffectShader;

        constexpr static inline const char* name = "ScreenEffectShader";

        static inline effect_screen_effect*& instance = *reinterpret_cast<effect_screen_effect**>(0x00B43010);
    };

    class effect_hdr : public effect
    {
    public:
        effect_hdr();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::HDRShader;

        constexpr static inline const char* name = "HDRShader";

        static inline effect_hdr*& instance = *reinterpret_cast<effect_hdr**>(0x00B43014);
    };

    class effect_ucap : public effect
    {
    public:
        effect_ucap();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::UCAP;

        constexpr static inline const char* name = "UCAP";

        static inline effect_ucap*& instance = *reinterpret_cast<effect_ucap**>(0x00B43018);
    };

    class effect_glass_reflect : public effect
    {
    public:
        effect_glass_reflect();

    public:
        void start() override;
        void load_global_textures() override;

    private:
        ::IDirect3DTexture9* reflection_texture_;

    public:
        constexpr static inline shader_type type = shader_type::GLASS_REFLECT;

        constexpr static inline const char* name = "GLASS_REFLECT";

        static inline effect_glass_reflect*& instance = *reinterpret_cast<effect_glass_reflect**>(0x00B4301C);
    };

    class effect_water : public effect
    {
    public:
        effect_water();

    public:
        void start() override;
        void load_global_textures() override;

    private:
        texture::info* pca_water_textures_[2];
        pca::blend_data blend_data_;

    public:
        constexpr static inline shader_type type = shader_type::WATER;

        constexpr static inline const char* name = "WATER";

        static inline effect_water*& instance = *reinterpret_cast<effect_water**>(0x00B43020);

    private:
        static inline float& fps_ = *reinterpret_cast<float*>(0x00A71258);

        static inline float& water_time_ticker_ = *reinterpret_cast<float*>(0x00B4CFA8);

        static inline std::int32_t& last_frame_updated_ = *reinterpret_cast<std::int32_t*>(0x00B4CFAC);
    };

    class effect_rvmpip : public effect
    {
    public:
        effect_rvmpip();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::RVMPIP;

        constexpr static inline const char* name = "RVMPIP";

        static inline effect_rvmpip*& instance = *reinterpret_cast<effect_rvmpip**>(0x00B43024);
    };

    class effect_ghost_car : public effect
    {
    public:
        effect_ghost_car();

    public:
        void start() override;

    public:
        constexpr static inline shader_type type = shader_type::GHOSTCAR;

        constexpr static inline const char* name = "GHOSTCAR";

        static inline effect_ghost_car*& instance = *reinterpret_cast<effect_ghost_car**>(0x00B43028);
    };


    class shader_lib final
    {
    private:
        static void bind_pca_channels(pca::channel_info* channels, size_t count);

    public:
        static void init();

        static void close();

        static auto find_input(const char* name) -> const effect::input*;

        static auto find_param_index(std::uint32_t key) -> const effect::param_index_pair*;

        static void lose_device();

        static void end_effect(effect& eff);

        static void recompute_techniques_by_detail(std::uint32_t detail_level);

        static void bind_pca_weights(pca::weights& weights);

        static void bind_ucap_weights(pca::ucap_frame_weights& weights);

        inline static auto get_shader_name(shader_type type) -> const char*
        {
            return shader_lib::type_names_[static_cast<std::uint32_t>(type)];
        }

    public:
        static inline auto effect_param_list = array<effect::param_index_pair, static_cast<size_t>(effect::parameter_type::count)>(0x00B43150);
        
        static inline effect*& current_effect = *reinterpret_cast<effect**>(0x00AB0BA4);

        static inline auto vertex_decl_type_map = array<::D3DDECLTYPE, static_cast<size_t>(vertex_type::count)>(0x00A650EC);

        static inline auto vertex_decl_usage_map = array<::D3DDECLUSAGE, static_cast<size_t>(vertex_component::count)>(0x00A65110);

        static inline auto vertex_usage_index_map = array<::BYTE, static_cast<size_t>(vertex_component::count)>(0x00A65168);

        static inline auto vertex_type_size_map = array<std::uint32_t, static_cast<size_t>(vertex_type::none)>(0x00A65180);

        static inline ::ID3DXEffectPool*& effect_pool = *reinterpret_cast<::ID3DXEffectPool**>(0x00AB0B84);

    private:
        static inline bool& initilaized_ = *reinterpret_cast<bool*>(0x00B4302C);

        static inline auto effects_ = array<effect*, static_cast<size_t>(shader_type::count)>(0x00B1F660);

        static inline auto inputs_ = array<effect::input, static_cast<size_t>(shader_type::count)>(0x00A64008);

        static inline const char* type_names_[static_cast<std::uint32_t>(shader_type::count)] = {
            effect_world::name,
            effect_world_reflect::name,
            effect_world_bone::name,
            effect_world_normal_map::name,
            effect_car::name,
            effect_car_normal_map::name,
            effect_world_min::name,
            effect_fe::name,
            effect_fe_mask::name,
            effect_filter::name,
            effect_screen_filter::name,
            effect_raindrop::name,
            effect_visual_treatment::name,
            effect_world_prelit::name,
            effect_particles::name,
            effect_sky::name,
            effect_shadow_map_mesh::name,
            effect_car_shadow_map::name,
            effect_world_depth::name,
            effect_shadow_map_mesh_depth::name,
            effect_normal_map_no_fog::name,
            effect_instance_mesh::name,
            effect_screen_effect::name,
            effect_hdr::name,
            effect_ucap::name,
            effect_glass_reflect::name,
            effect_water::name,
            effect_rvmpip::name,
            effect_ghost_car::name
        };
    };

    CREATE_ENUM_EXPR_OPERATORS(shader_type);
    CREATE_ENUM_EXPR_OPERATORS(effect::parameter_type);

    ASSERT_SIZE(effect::parameter, 0x28);
    ASSERT_SIZE(effect::technique, 0x18);
    ASSERT_SIZE(effect::table, 0x10);
    ASSERT_SIZE(effect::param_index_pair, 0x08);
    ASSERT_SIZE(effect::stream_channel, 0x08);
    ASSERT_SIZE(effect::input, 0x90);
    ASSERT_SIZE(effect, 0x1788);
}
