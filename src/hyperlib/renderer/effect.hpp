#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/geometry.hpp>
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
        half2,
        half4,
        dec4n,
        short4n,
        ubyte4,
        uint1,
        count,
    };

    enum class vertex_component : std::uint32_t
    {
        position,
        normal,
        diffuse,
        uv,
        uv_ambient,
        weight,
        index,
        tangent,
        light_index,
        pca_component0,
        pca_component1,
        pca_component2,
        pca_component3,
        pca_component4,
        pca_component5,
        pca_component6,
        pca_component7,
        count,
    };

    class effect
    {
    public:
        enum class flags : std::uint32_t
        {
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
            const char* name_begin;
            const char* name_end;
            const void* true_end;
            std::int32_t idk_0x0C;
            std::int32_t number;
            std::int32_t param_int;
        };

        struct table
        {
            technique* tech_begin;
            technique* tech_end;
            void* true_end;
            std::uint32_t pad;
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
            std::uint32_t stream;
            vertex_component component;
            vertex_type type;
            std::uint32_t usage_index;
        };

        struct input
        {
            const char* effect_name;
            stream_channel channels[8];
            const char* filename;
            const char* resource;
            std::uint32_t unknown;
        };

    private:
        void store_param_by_key(::LPCSTR name, ::D3DXHANDLE handle);

    public:
        virtual ~effect() = 0;
        virtual void handle_material_data(std::uint32_t*, std::uint32_t) = 0;
        virtual void set_transforms(const matrix4x4*, class render_target*, bool) = 0;
        virtual void start(::IDirect3DTexture9*, std::int32_t) = 0;
        virtual void end() = 0;
        virtual void preflight_draw() = 0;
        virtual void load_global_textures() = 0;

        effect(shader_type type, effect::flags flags, std::uint32_t last_param_key, const effect::input* input);

        void connect_parameters();

        void free_effect();

        inline auto id() const -> std::uint32_t
        {
            return this->id_;
        }

        inline bool has_low_lod_technique() const
        {
            return this->low_lod_technique_number_ > 0u;
        }

        inline void set_int(parameter_type type, std::int32_t value)
        {
            this->effect_->SetInt(this->params_[static_cast<std::uint32_t>(type)].handle, value);
        }

        inline void set_float(parameter_type type, float value)
        {
            this->effect_->SetFloat(this->params_[static_cast<std::uint32_t>(type)].handle, value);
        }

        constexpr static inline auto get_parameter_name(parameter_type type) -> const char*
        {
            return effect::parameter_names_[static_cast<std::uint32_t>(type)];
        }

    private:
        std::uint32_t id_;
        std::uint32_t stride_;
        std::uint32_t technique_count_;
        std::uint32_t technique_handle_;
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
        ::ID3DXEffect* effect_;
        ::IDirect3DVertexDeclaration9* vertex_decl_;
        parameter params_[static_cast<std::uint32_t>(parameter_type::count)];
        char data_36_[16];
        table table_;
        param_index_pair* index_pairs_;
        const char* name_;
        flags flags_;
        bool has_annotations_;
        std::int32_t low_lod_technique_number_;
        std::int32_t has_zero_offset_scale_;
        std::int32_t has_fog_disabled_;
        const light_material::instance* last_used_light_material_;
        const lighting::dynamic_context* last_used_light_context_;

    private:
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
        constexpr static inline shader_type type = shader_type::WorldShader;

        constexpr static inline const char* name = "WorldShader";

        static inline effect_world*& instance = *reinterpret_cast<effect_world**>(0x00B42FB8);
    };

    class effect_world_reflect : public effect
    {
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
        constexpr static inline shader_type type = shader_type::WorldBoneShader;

        constexpr static inline const char* name = "WorldBoneShader";

        static inline effect_world_bone*& instance = *reinterpret_cast<effect_world_bone**>(0x00B42FC0);
    };

    class effect_world_normal_map : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::WorldNormalMap;

        constexpr static inline const char* name = "WorldNormalMap";

        static inline effect_world_normal_map*& instance = *reinterpret_cast<effect_world_normal_map**>(0x00B42FC4);
    };

    class effect_car : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::CarShader;

        constexpr static inline const char* name = "CarShader";

        static inline effect_car*& instance = *reinterpret_cast<effect_car**>(0x00B42FC8);
    };

    class effect_car_normal_map : public effect
    {
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
        constexpr static inline shader_type type = shader_type::WorldMinShader;

        constexpr static inline const char* name = "WorldMinShader";

        static inline effect_world_min*& instance = *reinterpret_cast<effect_world_min**>(0x00B42FD0);
    };

    class effect_fe : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::FEShader;

        constexpr static inline const char* name = "FEShader";

        static inline effect_fe*& instance = *reinterpret_cast<effect_fe**>(0x00B42FD4);
    };

    class effect_fe_mask : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::FEMaskShader;

        constexpr static inline const char* name = "FEMaskShader";

        static inline effect_fe_mask*& instance = *reinterpret_cast<effect_fe_mask**>(0x00B42FD8);
    };

    class effect_filter : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::FilterShader;

        constexpr static inline const char* name = "FilterShader";

        static inline effect_filter*& instance = *reinterpret_cast<effect_filter**>(0x00B42FDC);
    };

    class effect_screen_filter : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::ScreenFilterShader;

        constexpr static inline const char* name = "ScreenFilterShader";

        static inline effect_screen_filter*& instance = *reinterpret_cast<effect_screen_filter**>(0x00B42FE0);
    };

    class effect_raindrop : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::RainDropShader;

        constexpr static inline const char* name = "RainDropShader";

        static inline effect_raindrop*& instance = *reinterpret_cast<effect_raindrop**>(0x00B42FE4);
    };

    class effect_visual_treatment : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::VisualTreatmentShader;

        constexpr static inline const char* name = "VisualTreatmentShader";

        static inline effect_visual_treatment*& instance = *reinterpret_cast<effect_visual_treatment**>(0x00B42FE8);
    };

    class effect_world_prelit : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::WorldPrelitShader;

        constexpr static inline const char* name = "WorldPrelitShader";

        static inline effect_world_prelit*& instance = *reinterpret_cast<effect_world_prelit**>(0x00B42FEC);
    };

    class effect_particles : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::ParticlesShader;

        constexpr static inline const char* name = "ParticlesShader";

        static inline effect_particles*& instance = *reinterpret_cast<effect_particles**>(0x00B42FF0);
    };

    class effect_sky : public effect
    {
    private:
        ::IDirect3DTexture9* texture_;

    public:
        constexpr static inline shader_type type = shader_type::skyshader;

        constexpr static inline const char* name = "skyshader";

        static inline effect_sky*& instance = *reinterpret_cast<effect_sky**>(0x00B42FF4);
    };

    class effect_shadow_map_mesh : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::shadow_map_mesh;

        constexpr static inline const char* name = "shadow_map_mesh";

        static inline effect_shadow_map_mesh*& instance = *reinterpret_cast<effect_shadow_map_mesh**>(0x00B42FF8);
    };

    class effect_car_shadow_map : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::CarShadowMapShader;

        constexpr static inline const char* name = "CarShadowMapShader";

        static inline effect_car_shadow_map*& instance = *reinterpret_cast<effect_car_shadow_map**>(0x00B42FFC);
    };

    class effect_world_depth : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::WorldDepthShader;

        constexpr static inline const char* name = "WorldDepthShader";

        static inline effect_world_depth*& instance = *reinterpret_cast<effect_world_depth**>(0x00B43000);
    };

    class effect_shadow_map_mesh_depth : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::shadow_map_mesh_depth;

        constexpr static inline const char* name = "shadow_map_mesh_depth";

        static inline effect_shadow_map_mesh_depth*& instance = *reinterpret_cast<effect_shadow_map_mesh_depth**>(0x00B43004);
    };

    class effect_normal_map_no_fog : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::NormalMapNoFog;

        constexpr static inline const char* name = "NormalMapNoFog";

        static inline effect_normal_map_no_fog*& instance = *reinterpret_cast<effect_normal_map_no_fog**>(0x00B43008);
    };

    class effect_instance_mesh : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::InstanceMesh;

        constexpr static inline const char* name = "InstanceMesh";

        static inline effect_instance_mesh*& instance = *reinterpret_cast<effect_instance_mesh**>(0x00B4300C);
    };

    class effect_screen_effect : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::ScreenEffectShader;

        constexpr static inline const char* name = "ScreenEffectShader";

        static inline effect_screen_effect*& instance = *reinterpret_cast<effect_screen_effect**>(0x00B43010);
    };

    class effect_hdr : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::HDRShader;

        constexpr static inline const char* name = "HDRShader";

        static inline effect_hdr*& instance = *reinterpret_cast<effect_hdr**>(0x00B43014);
    };

    class effect_ucap : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::UCAP;

        constexpr static inline const char* name = "UCAP";

        static inline effect_ucap*& instance = *reinterpret_cast<effect_ucap**>(0x00B43018);
    };

    class effect_glass_reflect : public effect
    {
    private:
        ::IDirect3DTexture9* reflection_texture_;

    public:
        constexpr static inline shader_type type = shader_type::GLASS_REFLECT;

        constexpr static inline const char* name = "GLASS_REFLECT";

        static inline effect_glass_reflect*& instance = *reinterpret_cast<effect_glass_reflect**>(0x00B4301C);
    };

    class effect_water : public effect
    {
    private:
        texture::info* pca_water_textures_[2];
        geometry::pca_blend_data blend_data_;
        char data_unknown[0x04];

    public:
        constexpr static inline shader_type type = shader_type::WATER;

        constexpr static inline const char* name = "WATER";

        static inline effect_water*& instance = *reinterpret_cast<effect_water**>(0x00B43020);
    };

    class effect_rvmpip : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::RVMPIP;

        constexpr static inline const char* name = "RVMPIP";

        static inline effect_rvmpip*& instance = *reinterpret_cast<effect_rvmpip**>(0x00B43024);
    };

    class effect_ghost_car : public effect
    {
    public:
        constexpr static inline shader_type type = shader_type::GHOSTCAR;

        constexpr static inline const char* name = "GHOSTCAR";

        static inline effect_ghost_car*& instance = *reinterpret_cast<effect_ghost_car**>(0x00B43028);
    };


    class shader_lib final
    {
    public:
        static void init();

        static auto find_input(const char* name) -> const effect::input*;

        static auto find_param_index(std::uint32_t key) -> const effect::param_index_pair*;

        static void free_effects();

        inline static auto get_shader_name(shader_type type) -> const char*
        {
            return shader_lib::type_names_[static_cast<std::uint32_t>(type)];
        }

    public:
        static inline auto effect_param_list = array<effect::param_index_pair, static_cast<size_t>(effect::parameter_type::count)>(0x00B43150);
        
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
    ASSERT_SIZE(effect::stream_channel, 0x10);
    ASSERT_SIZE(effect::input, 0x90);
    ASSERT_SIZE(effect, 0x1788);
}
