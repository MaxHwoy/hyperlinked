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

    void screen_effect::clear() const
    {
        for (std::uint32_t i = 0u; i < screen_effect::downscale_count; ++i)
        {
            if (::IDirect3DSurface9* surface = this->surfaces_1_[i])
            {
                directx::set_and_clear_target(surface);
            }

            if (::IDirect3DSurface9* surface = this->surfaces_2_[i])
            {
                directx::set_and_clear_target(surface);
            }

            if (::IDirect3DSurface9* surface = this->surfaces_3_[i])
            {
                directx::set_and_clear_target(surface);
            }
        }
    }

    void screen_effect::get_downscale_2x2_offsets(std::uint32_t width, std::uint32_t height, vector4(&offsets)[0x10])
    {
        float inverse_w = 1.0f / width;
        float inverse_h = 1.0f / height;

        float n_scaled_0_5_w = -0.5f * inverse_w;
        float p_scaled_0_5_w = +0.5f * inverse_w;

        for (std::uint32_t i = 0u, k = 0u; i < 2u; ++i, k += 2u)
        {
            float scaled_h = (i - 0.5f) * inverse_h;

            offsets[k + 0].x = n_scaled_0_5_w;
            offsets[k + 0].y = scaled_h;

            offsets[k + 1].x = p_scaled_0_5_w;
            offsets[k + 1].y = scaled_h;
        }
    }

    void screen_effect::get_downscale_4x4_offsets(std::uint32_t width, std::uint32_t height, vector4(&offsets)[0x10])
    {
        float inverse_w = 1.0f / width;
        float inverse_h = 1.0f / height;

        float n_scaled_1_5_w = -1.5f * inverse_w;
        float n_scaled_0_5_w = -0.5f * inverse_w;
        float p_scaled_0_5_w = +0.5f * inverse_w;
        float p_scaled_1_5_w = +1.5f * inverse_w;

        for (std::uint32_t i = 0u, k = 0u; i < 4u; ++i, k += 4u)
        {
            float scaled_h = (i - 1.5f) * inverse_h;

            offsets[k + 0].x = n_scaled_1_5_w;
            offsets[k + 0].y = scaled_h;

            offsets[k + 1].x = n_scaled_0_5_w;
            offsets[k + 1].y = scaled_h;

            offsets[k + 2].x = p_scaled_0_5_w;
            offsets[k + 2].y = scaled_h;

            offsets[k + 3].x = p_scaled_1_5_w;
            offsets[k + 3].y = scaled_h;
        }
    }

    void screen_effect::get_gauss_blur_kernal_5x5(std::uint32_t width, std::uint32_t height, vector4(&offsets)[0x10], vector4(&weights)[0x10], float scale)
    {
        constexpr float deviation = 1.0f;

        float sum = 0.0f;
        float sig = deviation * deviation;
        float mpi = 1.0f / (::sqrtf(6.283185482025146f * sig));
        float dev = -0.5f / sig;

        std::uint32_t index = 0u;

        for (std::int32_t i = -2; i <= 2; ++i)
        {
            for (std::int32_t j = -2; j <= 2; ++j)
            {
                float x = static_cast<float>(i);
                float y = static_cast<float>(j);

                if (::fabs(x) + ::fabs(y) <= 2.0f)
                {
                    float gauss = ::expf((x * x + y * y) * dev) * mpi;

                    offsets[index].x = x / static_cast<float>(width);
                    offsets[index].y = y / static_cast<float>(height);
                    offsets[index].z = 0.0f;
                    offsets[index].w = 0.0f;

                    weights[index].x = gauss;
                    weights[index].y = gauss;
                    weights[index].z = gauss;
                    weights[index].w = gauss;

                    sum += gauss;

                    index++;
                }
            }
        }

        scale /= sum;

        for (std::uint32_t i = 0u; i < index; ++i)
        {
            weights[i] *= scale;
        }
    }

    void screen_effect::get_bloom_kernal_params(std::uint32_t resolution, vector4(&offsets)[0x10], vector4(&weights)[0x10], float deviation, float scale, bool x_direction)
    {
        float sum = 0.0f;
        float sig = deviation * deviation;
        float mpi = 1.0f / (::sqrtf(6.283185482025146f * sig));
        float dev = -0.5f / sig;
        float res = 1.0f / static_cast<float>(resolution);

        std::uint32_t index = 0u;

        if (x_direction)
        {
            for (std::int32_t i = -7; i <= 8; ++i)
            {
                float radii = (screen_effect::kernal_x_offset_ + static_cast<float>(i)) * screen_effect::kernal_x_scale_;
                float gauss = ::expf((radii * radii) * dev) * mpi;

                offsets[index].x = radii * res;
                offsets[index].y = 0.0f;
                offsets[index].z = 0.0f;
                offsets[index].w = 0.0f;

                weights[index].x = gauss;
                weights[index].y = gauss;
                weights[index].z = gauss;
                weights[index].w = 1.0f;

                sum += gauss;

                index++;
            }
        }
        else
        {
            for (std::int32_t i = -7; i <= 8; ++i)
            {
                float radii = (screen_effect::kernal_y_offset_ + static_cast<float>(i)) * screen_effect::kernal_y_scale_;
                float gauss = ::expf((radii * radii) * dev) * mpi;

                offsets[index].x = 0.0f;
                offsets[index].y = radii * res;
                offsets[index].z = 0.0f;
                offsets[index].w = 0.0f;

                weights[index].x = gauss;
                weights[index].y = gauss;
                weights[index].z = gauss;
                weights[index].w = 1.0f;

                sum += gauss;

                index++;
            }
        }

        scale /= sum;

        for (std::uint32_t i = 0u; i < index; ++i)
        {
            weights[i] *= scale;
        }
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

    void screen_effect::downsample_2x2_texture(effect& effect, ::IDirect3DTexture9* texture_src, ::IDirect3DSurface9* surface_dst, const char* technique)
    {
        vector4 offsets[0x10];

        if (technique == nullptr)
        {
            technique = "DownScale2x2";
        }

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);
        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);

        std::uint32_t w = directx::resolution_x;
        std::uint32_t h = directx::resolution_y;

        screen_effect::get_downscale_2x2_offsets(w, h, offsets); // #FORCARBON get resolution from level desc (like in 4x4)

        effect.set_current_pass(0u, technique, false);
        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));
        effect.draw_full_screen_quad(texture_src, false);
        effect.finalize();
    }

    void screen_effect::downsample_4x4_texture(effect& effect, ::IDirect3DTexture9* texture_src, ::IDirect3DSurface9* surface_dst, const char* technique)
    {
        vector4 offsets[0x10];

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
        vector4 offsets[0x10];
        vector4 weights[0x10];

        ::IDirect3DSurface9* surface_dst;

        texture_dst->GetSurfaceLevel(level, &surface_dst);

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        ::D3DSURFACE_DESC desc;

        texture_src->GetLevelDesc(0u, &desc);

        screen_effect::get_gauss_blur_kernal_5x5(desc.Width, desc.Height, offsets, weights, 1.0f);

        effect.set_current_pass(0u, "GaussBlur5x5", false);
        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));
        effect.set_vector_array(effect::parameter_type::cavSampleWeights, weights, std::size(weights));
        effect.draw_full_screen_quad(texture_src, false);
        effect.finalize();

        surface_dst->Release();
    }

    void screen_effect::multipass_gauss_blur(effect& effect, ::IDirect3DTexture9* texture_src, bool bloom_across_width, ::IDirect3DSurface9* surface_dst, float scale)
    {
        vector4 offsets[0x10];
        vector4 weights[0x10];

        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->SetDepthStencilSurface(nullptr);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        ::D3DSURFACE_DESC desc;

        texture_src->GetLevelDesc(0u, &desc);

        std::uint32_t resolution = bloom_across_width ? desc.Width : desc.Height;

        screen_effect::get_bloom_kernal_params(resolution, offsets, weights, 3.0f, scale, bloom_across_width);

        effect.set_current_pass(0u, "TwoPassBlur", false);
        effect.set_vector_array(effect::parameter_type::cavSampleOffsets, offsets, std::size(offsets));
        effect.set_vector_array(effect::parameter_type::cavSampleWeights, weights, std::size(weights));
        effect.draw_full_screen_quad(texture_src, false);
        effect.finalize();
    }

    void screen_effect::blend_textures(effect& effect, ::IDirect3DSurface9* surface_dst, ::IDirect3DTexture9* texture_src1, ::IDirect3DTexture9* texture_src2, float x, float y, const char* technique)
    {
        vector4 weights[0x01];

        if (technique == nullptr)
        {
            technique = "BlendTextures";
        }

        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);
        directx::device()->SetRenderState(::D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);
        directx::device()->SetRenderTarget(0u, surface_dst);
        directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, color32::clear(), 1.0f, 0u);

        weights[0] = { x, y, 0.0f, 0.0f };

        effect.set_current_pass(0u, technique, false);
        effect.set_texture(effect::parameter_type::MISCMAP1_TEXTURE, texture_src2);
        effect.set_vector_array(effect::parameter_type::cavSampleWeights, weights, std::size(weights));
        effect.draw_full_screen_quad(texture_src1, false);
        effect.finalize();
    }
}
