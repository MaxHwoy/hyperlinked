#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/depth_of_field.hpp>
#include <hyperlib/renderer/screen_effect.hpp>
#include <hyperlib/renderer/face_pixelation.hpp>
#include <hyperlib/renderer/post_process.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/visual_treatment.hpp>

namespace hyper
{
    post_process::post_process() : surfaces_{}, textures_{}, data_{}
    {
        std::uint32_t x = vt_render_target::resolution_x = directx::resolution_x;
        std::uint32_t y = vt_render_target::resolution_y = directx::resolution_y;

        bool enable = options::motion_blur_enabled || options::rain_enabled || options::visual_treatment;

        if (enable)
        {
            bool failed = false;

            for (std::uint32_t i = 0u; i < post_process::target_count; ++i)
            {
                if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_[i], nullptr)))
                {
                    failed = true;

                    break;
                }

                if (FAILED(this->textures_[i]->GetSurfaceLevel(0u, &this->surfaces_[i])))
                {
                    failed = true;

                    break;
                }
            }

            if (failed)
            {
                post_process::dtor(*this);

                options::rain_enabled = false;
                options::motion_blur_enabled = false;
                options::rain_supported = 0u;
                options::motion_blur_supported = 0u;
            }
        }

        if (!post_process::inited)
        {
            post_process::inited = true; // blackbox???
        }
    }

    post_process::~post_process()
    {
        for (std::uint32_t i = 0u; i < post_process::target_count; ++i)
        {
            if (this->surfaces_[i] != nullptr)
            {
                this->surfaces_[i]->Release();

                this->surfaces_[i] = nullptr;
            }

            if (this->textures_[i] != nullptr)
            {
                this->textures_[i]->Release();

                this->textures_[i] = nullptr;
            }
        }
    }

    bool post_process::can_do_noise(const post_process& process, const screen_effect& screen)
    {
        return process.textures_[0] != nullptr && screen.acquire_texture_2(screen_effect::downscale::d8x8);
    }

    void post_process::ctor(post_process& process)
    {
        new (&process) post_process();
    }

    void post_process::dtor(post_process& process)
    {
        process.~post_process();
    }

    void post_process::apply(post_process& process, const view::instance& view)
    {
        bool rain_enabled = game_flow::manager::instance.current_state == game_flow::state::racing && options::rain_enabled && options::particles_enabled;

        bool pixelate = options::bone_animations_enabled && face_pixelation::enabled && post_process::can_do_noise(process, screen_effect::instance);

        bool vt_enabled = options::visual_treatment > 0u;

        render_target& vt_target = vt_render_target::instance();

        render_target& main_target = player_render_target::instance();

        if (vt_enabled || pixelate || rain_enabled)
        {
            BENCHMARK();

            directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);
            directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);
            directx::device()->StretchRect(main_target.d3d_target, nullptr, post_process::get_current_surface(process), nullptr, ::D3DTEXF_NONE);

            if (rain_enabled)
            {
                directx::device()->StretchRect(main_target.d3d_target, nullptr, post_process::get_next_surface(process), nullptr, ::D3DTEXF_NONE);

                if (rain_renderer::instance.render(post_process::get_next_surface(process), post_process::get_current_texture(process)))
                {
                    post_process::rotate_current_surface();
                }
            }

            if (pixelate && view.face_pixelation != nullptr)
            {
                view.face_pixelation->update();

                view.face_pixelation->render(post_process::get_current_texture(process));
            }

            if (vt_enabled)
            {
                depth_of_field::instance.update(post_process::get_current_texture(process), false);

                visual_treatment::instance->final_render(view_id::player1, post_process::get_current_texture(process), motion_blur_render_target::d3d_texture);
            }
            else
            {
                directx::device()->StretchRect(post_process::get_current_surface(process), nullptr, vt_target.d3d_target, nullptr, ::D3DTEXF_NONE);
            }
        }
        else if (vt_target.d3d_target != main_target.d3d_target)
        {
            directx::device()->StretchRect(main_target.d3d_target, nullptr, vt_target.d3d_target, nullptr, ::D3DTEXF_NONE);
        }
    }

    void post_process::do_motion_blur(post_process& process, visual_treatment& vt, view_id id)
    {
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);

        const render_target* target = view::instance::views[id].attached_target;

        directx::device()->StretchRect(target->d3d_target, nullptr, process.surfaces_[0u], nullptr, ::D3DTEXF_NONE);

        vt.motion_blur(id);
    }

    void post_process::do_uves_over_cliff(post_process& process, visual_treatment& vt, view_id id)
    {
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, false);
        directx::device()->SetRenderState(::D3DRS_ALPHABLENDENABLE, false);

        const render_target* target = view::instance::views[id].attached_target;

        directx::device()->StretchRect(target->d3d_target, nullptr, process.surfaces_[0u], nullptr, ::D3DTEXF_NONE);

        vt.uves_over_cliff(id);
    }

    void post_process::do_uves_over_cliff_darken(post_process& process, visual_treatment& vt, view_id id)
    {
        const render_target* target = view::instance::views[id].attached_target;

        directx::device()->StretchRect(target->d3d_target, nullptr, process.surfaces_[0u], nullptr, ::D3DTEXF_NONE);

        vt.uves_over_cliff_darken(id);
    }

    auto post_process::get_current_texture(const post_process& process) -> ::IDirect3DTexture9*
    {
        return process.textures_[post_process::current_index_];
    }

    auto post_process::get_current_surface(const post_process& process) -> ::IDirect3DSurface9*
    {
        return process.surfaces_[post_process::current_index_];
    }

    auto post_process::get_next_texture(const post_process& process) -> ::IDirect3DTexture9*
    {
        if constexpr (math::is_pow_2(post_process::target_count))
        {
            return process.textures_[(post_process::current_index_ + 1) & (post_process::target_count - 1u)];
        }
        else
        {
            return process.textures_[(post_process::current_index_ + 1) % post_process::target_count];
        }
    }

    auto post_process::get_next_surface(const post_process& process) -> ::IDirect3DSurface9*
    {
        if constexpr (math::is_pow_2(post_process::target_count))
        {
            return process.surfaces_[(post_process::current_index_ + 1) & (post_process::target_count - 1u)];
        }
        else
        {
            return process.surfaces_[(post_process::current_index_ + 1) % post_process::target_count];
        }
    }

    void post_process::rotate_current_surface()
    {
        if constexpr (math::is_pow_2(post_process::target_count))
        {
            post_process::current_index_ = (post_process::current_index_ + 1) & (post_process::target_count - 1u);
        }
        else
        {
            post_process::current_index_ = (post_process::current_index_ + 1) % post_process::target_count;
        }
    }
}
