#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/screen_effect.hpp>
#include <hyperlib/renderer/depth_of_field.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    depth_of_field::depth_of_field()
    {
        if (this->texture_ == nullptr)
        {
            std::uint32_t x = math::floor_pow_2(directx::resolution_x >> 1);
            std::uint32_t y = math::floor_pow_2(directx::resolution_y >> 1);

            directx::device()->CreateTexture(x, y, 2u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->texture_, nullptr);
        }
    }

    depth_of_field::~depth_of_field()
    {
        if (this->texture_ != nullptr)
        {
            this->texture_->Release();

            this->texture_ = nullptr;
        }
    }

    bool depth_of_field::active(view_id id)
    {
        const render_view& view = render_view::views[id];

        if (view.get_has_camera())
        {
            float focal_distance = depth_of_field::override_focal_distance == 0.0f ? view.get_focal_distance() : depth_of_field::override_focal_distance;

            float dof_value = depth_of_field::override_dof_value == 0.0f ? view.get_camera_dof() : depth_of_field::override_dof_value;

            return ::fabs(focal_distance) >= 0.0001f || ::fabs(dof_value) >= 0.0001f;
        }

        return false;
    }

    void depth_of_field::update(::IDirect3DTexture9* back_buffer, bool split_screen)
    {
        if (this->active(view_id::player1))
        {
            directx::device()->SetDepthStencilSurface(nullptr);

            directx::device()->SetRenderState(::D3DRS_ZENABLE, false);

            effect_screen_effect& effect = *effect_screen_effect::instance;

            constexpr std::uint32_t downsample_count = 2u;

            for (std::uint32_t i = 0u; i < downsample_count; ++i)
            {
                ::IDirect3DTexture9* texture = back_buffer;

                if (i != 0u)
                {
                    texture = screen_effect::instance.acquire_texture_2(static_cast<screen_effect::downscale>(i - 1u));
                }

                screen_effect::downsample_4x4_texture(effect, texture, screen_effect::instance.acquire_surface_2(static_cast<screen_effect::downscale>(i)), nullptr);
                screen_effect::gauss_blur_5x5_texture(effect, screen_effect::instance.acquire_texture_2(static_cast<screen_effect::downscale>(i)), this->texture_, i);
            }

            directx::device()->SetRenderState(::D3DRS_ZENABLE, true);
        }
    }

    void depth_of_field::ctor(depth_of_field& dof)
    {
        new (&dof) depth_of_field();
    }

    void depth_of_field::dtor(depth_of_field& dof)
    {
        dof.~depth_of_field();
    }
}
