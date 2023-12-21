#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/screen_effect.hpp>
#include <hyperlib/renderer/model_renderer.hpp>

namespace hyper
{
    screen_effect::screen_effect()
    {
        for (std::uint32_t i = 0u; i < screen_effect::downscale_count; ++i)
        {
            std::uint32_t x = directx::resolution_x >> (1 + i);
            std::uint32_t y = directx::resolution_y >> (1 + i);

            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_1_[i], nullptr);
            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_2_[i], nullptr);
            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_3_[i], nullptr);

            this->textures_1_[i]->GetSurfaceLevel(0u, &this->surfaces_1_[i]);
            this->textures_2_[i]->GetSurfaceLevel(0u, &this->surfaces_2_[i]);
            this->textures_3_[i]->GetSurfaceLevel(0u, &this->surfaces_3_[i]);
        }
    }

    screen_effect::~screen_effect()
    {
        for (std::uint32_t i = 0u; i < screen_effect::downscale_count; ++i)
        {
            if (this->surfaces_1_[i] != nullptr)
            {
                this->surfaces_1_[i]->Release();

                this->surfaces_1_[i] = nullptr;
            }

            if (this->surfaces_2_[i] != nullptr)
            {
                this->surfaces_2_[i]->Release();

                this->surfaces_2_[i] = nullptr;
            }

            if (this->surfaces_3_[i] != nullptr)
            {
                this->surfaces_3_[i]->Release();

                this->surfaces_3_[i] = nullptr;
            }

            if (this->textures_1_[i] != nullptr)
            {
                this->textures_1_[i]->Release();

                this->textures_1_[i] = nullptr;
            }

            if (this->textures_2_[i] != nullptr)
            {
                this->textures_2_[i]->Release();

                this->textures_2_[i] = nullptr;
            }

            if (this->textures_3_[i] != nullptr)
            {
                this->textures_3_[i]->Release();

                this->textures_3_[i] = nullptr;
            }
        }
    }

    void screen_effect::get_downscale_4x4_offsets(std::uint32_t width, std::uint32_t height, vector4(&offsets)[16])
    {
        call_function<void(__cdecl*)(std::uint32_t, std::uint32_t, vector4*)>(0x0073CC10)(width, height, offsets);
    }

    void screen_effect::gauss_blur_kernal_5x5(std::uint32_t width, std::uint32_t height, vector4(&offsets)[16], vector4(&weights)[16], float deviation)
    {
        call_function<void(__cdecl*)(std::uint32_t, std::uint32_t, vector4*, vector4*, float)>(0x0073C670)(width, height, offsets, weights, deviation);
    }

    void screen_effect::ctor(screen_effect& screen)
    {
        new (&screen) screen_effect();
    }

    void screen_effect::dtor(screen_effect& screen)
    {
        screen.~screen_effect();
    }

    void screen_effect::set_gaussian_kernals(float x_offset, float y_offset, float x_scale, float y_scale, float deviation)
    {
        screen_effect::kernal_x_offset_ = x_offset;
        screen_effect::kernal_y_offset_ = y_offset;
        screen_effect::kernal_x_scale_ = x_scale;
        screen_effect::kernal_y_scale_ = y_scale;
        screen_effect::kernal_deviation_ = deviation;
    }

    void screen_effect::get_bloom_kernal_params(std::uint32_t resolution, vector4(&offsets)[16], vector4(&weights)[16], float deviation, float multiplier, bool x_direction)
    {
        call_function<void(__cdecl*)(std::uint32_t, vector4*, vector4*, float, float, bool)>(0x0073C960)(resolution, offsets, weights, deviation, multiplier, x_direction);
    }

    void screen_effect::downsample_4x4_texture(effect& effect, ::IDirect3DTexture9* texture_src, ::IDirect3DSurface9* surface_dst, const char* technique)
    {
        vector4 offsets[16];

        if (technique == nullptr)
        {
            technique = "DownScale4x4";
        }

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);
        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);

        ::D3DSURFACE_DESC desc;

        texture_src->GetLevelDesc(0u, &desc);

        screen_effect::get_downscale_4x4_offsets(desc.Width, desc.Height, offsets);

        effect.set_current_pass(0u, technique, false);

        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));

        effect.draw_full_screen_quad(texture_src, false);

        effect.finalize();
    }

    void screen_effect::gauss_blur_5x5_texture(effect& effect, ::IDirect3DTexture9* texture_src, ::IDirect3DTexture9* texture_dst, std::uint32_t level)
    {
        vector4 offsets[16];
        vector4 weights[16];

        ::IDirect3DSurface9* surface_dst;

        texture_dst->GetSurfaceLevel(level, &surface_dst);

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        ::D3DSURFACE_DESC desc;

        texture_src->GetLevelDesc(0u, &desc);

        screen_effect::gauss_blur_kernal_5x5(desc.Width, desc.Height, offsets, weights, 1.0f);

        effect.set_current_pass(0u, "GaussBlur5x5", false);

        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));

        effect.set_vector_array(effect::parameter_type::cavSampleWeights, weights, std::size(weights));

        effect.draw_full_screen_quad(texture_src, false);

        effect.finalize();

        surface_dst->Release();
    }

    void screen_effect::multipass_gauss_blur(effect& effect, ::IDirect3DTexture9* texture_src, bool bloom_across_width, ::IDirect3DSurface9* surface_dst, float multiplier)
    {
        vector4 offsets[16];
        vector4 weights[16];

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        ::D3DSURFACE_DESC desc;

        texture_src->GetLevelDesc(0u, &desc);

        std::uint32_t resolution = desc.Width;

        if (!bloom_across_width)
        {
            resolution = desc.Height;
        }

        screen_effect::get_bloom_kernal_params(resolution, offsets, weights, 3.0f, multiplier, bloom_across_width);

        effect.set_current_pass(0u, "TwoPassBlur", false);

        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));

        effect.set_vector_array(effect::parameter_type::cavSampleWeights, weights, std::size(weights));

        effect.draw_full_screen_quad(texture_src, false);

        effect.finalize();
    }

    void screen_effect::blend_textures(effect& effect, ::IDirect3DSurface9* surface_dst, ::IDirect3DTexture9* texture_src1, ::IDirect3DTexture9* texture_src2, float x, float y, const char* technique)
    {
        if (technique == nullptr)
        {
            technique = "BlendTextures";
        }

        vector4 weight(x, y, 0.0f, 0.0f);

        effect.set_current_pass(0u, technique, false);

        effect.set_texture(effect::parameter_type::MISCMAP1_TEXTURE, texture_src2);

        effect.set_vector_array(effect::parameter_type::cavSampleWeights, &weight, 1u);

        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);
        directx::device()->SetRenderState(::D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);
        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        effect.draw_full_screen_quad(texture_src1, false);

        effect.finalize();
    }
}
