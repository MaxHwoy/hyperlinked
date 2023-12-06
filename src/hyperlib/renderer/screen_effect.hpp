#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class effect;

    class screen_effect final
    {
    public:
        enum class downscale : std::uint32_t
        {
            d2x2 = 0,
            d4x4 = 1,
            d8x8 = 2,
        };

        enum class control : std::uint32_t
        {
            frame,
            boolean,
            function,
        };

        enum class type : std::uint32_t
        {
            tint,
            motion_blur,
            visual_sig,
            glare,
            fe_blur,
            count,
        };

        struct __declspec(align(0x04)) inf
        {
            control controller;
            std::uint32_t frame_num;
            bool active;
        };

        struct db;

        struct def
        {
            color color;
            float intensity;
            float data[14];
            void(__cdecl* update)(type, db*);
        };

        struct db
        {
            void* view;
            inf se_inf[5];
            def se_data[5];
            std::uint32_t num_type[5];
            float se_time;
        };

        const static inline std::uint32_t downscale_count = 3;

    public:
        screen_effect();

        ~screen_effect();

        inline auto acquire_texture_1(downscale scale) -> ::IDirect3DTexture9*
        {
            return this->textures_1_[static_cast<std::uint32_t>(scale)];
        }

        inline auto acquire_texture_2(downscale scale) -> ::IDirect3DTexture9*
        {
            return this->textures_2_[static_cast<std::uint32_t>(scale)];
        }

        inline auto acquire_texture_3(downscale scale) -> ::IDirect3DTexture9*
        {
            return this->textures_3_[static_cast<std::uint32_t>(scale)];
        }

        inline auto acquire_surface_1(downscale scale) -> ::IDirect3DSurface9*
        {
            return this->surfaces_1_[static_cast<std::uint32_t>(scale)];
        }

        inline auto acquire_surface_2(downscale scale) -> ::IDirect3DSurface9*
        {
            return this->surfaces_2_[static_cast<std::uint32_t>(scale)];
        }

        inline auto acquire_surface_3(downscale scale) -> ::IDirect3DSurface9*
        {
            return this->surfaces_3_[static_cast<std::uint32_t>(scale)];
        }

    private:
        static void get_downscale_4x4_offsets(std::uint32_t width, std::uint32_t height, vector4(&offsets)[16]);

        static void get_bloom_kernal_params(std::uint32_t resolution, vector4(&offsets)[16], vector4(&weights)[16], float deviation, float multiplier, bool x_direction);

    public:
        static void ctor(screen_effect& screen);

        static void dtor(screen_effect& screen);

        static void set_gaussian_kernals(float x_offset, float y_offset, float x_scale, float y_scale, float deviation);

        static void downsample_4x4_texture(effect& effect, ::IDirect3DTexture9* texture_src, ::IDirect3DSurface9* surface_dst, const char* technique);

        static void multipass_gauss_blur(effect& effect, ::IDirect3DTexture9* texture_src, bool bloom_across_width, ::IDirect3DSurface9* surface_dst, float multiplier);

        static void blend_textures(effect& effect, ::IDirect3DSurface9* surface_dst, ::IDirect3DTexture9* texture_src1, ::IDirect3DTexture9* texture_src2, float x, float y, const char* technique);

    private:
        ::IDirect3DSurface9* surfaces_1_[downscale_count];
        ::IDirect3DTexture9* textures_1_[downscale_count];
        ::IDirect3DTexture9* textures_2_[downscale_count];
        ::IDirect3DTexture9* textures_3_[downscale_count];
        ::IDirect3DSurface9* surfaces_2_[downscale_count];
        ::IDirect3DSurface9* surfaces_3_[downscale_count];

    public:
        static inline screen_effect& instance = *reinterpret_cast<screen_effect*>(0x00B42F3C);

    private:
        static inline float& kernal_x_offset_ = *reinterpret_cast<float*>(0x00B42F34);

        static inline float& kernal_y_offset_ = *reinterpret_cast<float*>(0x00B42F38);

        static inline float& kernal_x_scale_ = *reinterpret_cast<float*>(0x00A6C1E0);

        static inline float& kernal_y_scale_ = *reinterpret_cast<float*>(0x00A6C1E4);

        static inline float& kernal_deviation_ = *reinterpret_cast<float*>(0x00A6C1E8);
    };

    ASSERT_SIZE(screen_effect::inf, 0x0C);
    ASSERT_SIZE(screen_effect::def, 0x50);
    ASSERT_SIZE(screen_effect::db, 0x1E8);
}
