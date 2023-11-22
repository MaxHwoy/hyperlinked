#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/targets.hpp>

namespace hyper
{
    void render_target::ctor(bool act, render_target_id tid, hyper::view_id vid, std::uint32_t x, std::uint32_t y, ::IDirect3DSurface9* target, ::IDirect3DSurface9* depth_stencil)
    {
        this->active = act;
        this->target_id = tid;
        this->view_id = vid;
        this->resolution_x = x;
        this->resolution_y = y;
        this->d3d_target = target;
        this->d3d_depth_stencil = depth_stencil;
    }

    void player_render_target::open()
    {
        player_render_target::d3d_texture = nullptr;

        directx::device()->GetBackBuffer(0u, 0u, ::D3DBACKBUFFER_TYPE_MONO, &player_render_target::render_target_surface_postprocess);

        directx::device()->GetDepthStencilSurface(&player_render_target::depth_stencil_surface);

        if (directx::visual_treatment)
        {
            ::D3DMULTISAMPLE_TYPE multisample = static_cast<::D3DMULTISAMPLE_TYPE>(directx::antialias_level);

            directx::device()->CreateRenderTarget(directx::resolution_x, directx::resolution_y, ::D3DFMT_A8R8G8B8, multisample, directx::antialias_level, false, &player_render_target::render_target_surface_standalone, nullptr);
        }
    }

    void rvm_render_target::open()
    {
        std::uint32_t x = rvm_render_target::resolution_x = math::floor_pow_2(rvm_render_target::resolution_x);
        std::uint32_t y = rvm_render_target::resolution_y = math::floor_pow_2(rvm_render_target::resolution_y);

        directx::device()->CreateDepthStencilSurface(x, y, static_cast<::D3DFORMAT>(72u), ::D3DMULTISAMPLE_NONE, 0u, false, &rvm_render_target::depth_stencil_surface, nullptr);

        directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &rvm_render_target::d3d_texture, nullptr);

        rvm_render_target::d3d_texture->GetSurfaceLevel(0u, &rvm_render_target::render_target_surface);
    }

    void shadowmap_render_target::open()
    {
        std::uint32_t x = shadowmap_render_target::resolution_x;
        std::uint32_t y = shadowmap_render_target::resolution_y;

        if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &shadowmap_render_target::render_target_texture, nullptr)))
        {
            return;
        }

        if (shadowmap_render_target::shadow_target_type == target_type::stencil_d24s8)
        {
            if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_DEPTHSTENCIL, ::D3DFMT_D24S8, ::D3DPOOL_DEFAULT, &shadowmap_render_target::depth_stencil_texture, nullptr)))
            {
                return;
            }
        }
        else if (shadowmap_render_target::shadow_target_type == target_type::stencil_df16)
        {
            ::D3DFORMAT format = static_cast<::D3DFORMAT>(MAKEFOURCC('D', 'F', '1', '6'));

            if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_DEPTHSTENCIL, format, ::D3DPOOL_DEFAULT, &shadowmap_render_target::depth_stencil_texture, nullptr)))
            {
                return;
            }
        }

        shadowmap_render_target::render_target_texture->GetSurfaceLevel(0u, &shadowmap_render_target::render_target_surface);
        shadowmap_render_target::depth_stencil_texture->GetSurfaceLevel(0u, &shadowmap_render_target::depth_stencil_surface);
    }

    void motion_blur_render_target::open()
    {
        if (motion_blur_render_target::d3d_texture == nullptr)
        {
            std::uint32_t x = math::floor_pow_2(directx::resolution_x >> 1);
            std::uint32_t y = math::floor_pow_2(directx::resolution_y >> 1);

            directx::device()->CreateTexture(x, y, 2u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &motion_blur_render_target::d3d_texture, nullptr);
        }


    }

    void player_render_target::close()
    {
        if (player_render_target::depth_stencil_surface != nullptr)
        {
            player_render_target::depth_stencil_surface->Release();

            player_render_target::depth_stencil_surface = nullptr;
        }

        if (player_render_target::render_target_surface_standalone != nullptr)
        {
            player_render_target::render_target_surface_standalone->Release();

            player_render_target::render_target_surface_standalone = nullptr;
        }

        if (player_render_target::render_target_surface_postprocess != nullptr)
        {
            player_render_target::render_target_surface_postprocess->Release();

            player_render_target::render_target_surface_postprocess = nullptr;
        }

        if (player_render_target::d3d_texture != nullptr)
        {
            player_render_target::d3d_texture->Release();

            player_render_target::d3d_texture = nullptr;
        }
    }

    void reflection_render_target::close()
    {
        if (reflection_render_target::depth_stencil_surface != nullptr)
        {
            reflection_render_target::depth_stencil_surface->Release();

            reflection_render_target::depth_stencil_surface = nullptr;
        }

        if (reflection_render_target::render_target_surface != nullptr)
        {
            reflection_render_target::render_target_surface->Release();

            reflection_render_target::render_target_surface = nullptr;
        }

        if (reflection_render_target::d3d_texture != nullptr)
        {
            reflection_render_target::d3d_texture->Release();

            reflection_render_target::d3d_texture = nullptr;
        }
    }

    void rvm_render_target::close()
    {
        if (rvm_render_target::depth_stencil_surface != nullptr)
        {
            rvm_render_target::depth_stencil_surface->Release();

            rvm_render_target::depth_stencil_surface = nullptr;
        }

        if (rvm_render_target::render_target_surface != nullptr)
        {
            rvm_render_target::render_target_surface->Release();

            rvm_render_target::render_target_surface = nullptr;
        }

        if (rvm_render_target::d3d_texture != nullptr)
        {
            rvm_render_target::d3d_texture->Release();

            rvm_render_target::d3d_texture = nullptr;
        }
    }

    void shadowmap_render_target::close()
    {
        if (shadowmap_render_target::depth_stencil_texture != nullptr)
        {
            shadowmap_render_target::depth_stencil_texture->Release();

            shadowmap_render_target::depth_stencil_texture = nullptr;
        }

        if (shadowmap_render_target::render_target_texture != nullptr)
        {
            shadowmap_render_target::render_target_texture->Release();

            shadowmap_render_target::render_target_texture = nullptr;
        }

        if (shadowmap_render_target::depth_stencil_surface != nullptr)
        {
            shadowmap_render_target::depth_stencil_surface->Release();

            shadowmap_render_target::depth_stencil_surface = nullptr;
        }

        if (shadowmap_render_target::render_target_surface != nullptr)
        {
            shadowmap_render_target::render_target_surface->Release();

            shadowmap_render_target::render_target_surface = nullptr;
        }
    }

    void pip_render_target::close()
    {
        if (pip_render_target::depth_stencil_surface != nullptr)
        {
            pip_render_target::depth_stencil_surface->Release();

            pip_render_target::depth_stencil_surface = nullptr;
        }

        if (pip_render_target::render_target_surface != nullptr)
        {
            pip_render_target::render_target_surface->Release();

            pip_render_target::render_target_surface = nullptr;
        }

        if (pip_render_target::cube_texture != nullptr)
        {
            pip_render_target::cube_texture->Release();

            pip_render_target::cube_texture = nullptr;
        }
    }

    void motion_blur_render_target::close()
    {
        if (motion_blur_render_target::depth_stencil_surface != nullptr)
        {
            motion_blur_render_target::depth_stencil_surface->Release();

            motion_blur_render_target::depth_stencil_surface = nullptr;
        }

        if (motion_blur_render_target::render_target_surface != nullptr)
        {
            motion_blur_render_target::render_target_surface->Release();

            motion_blur_render_target::render_target_surface = nullptr;
        }

        if (motion_blur_render_target::fuzzz_texture != nullptr)
        {
            motion_blur_render_target::fuzzz_texture->Release();

            motion_blur_render_target::fuzzz_texture = nullptr;
        }

        if (motion_blur_render_target::d3d_texture != nullptr)
        {
            motion_blur_render_target::d3d_texture->Release();

            motion_blur_render_target::d3d_texture = nullptr;
        }
    }

    void env_map_render_target::close()
    {
        if (env_map_render_target::cube_texture != nullptr)
        {
            env_map_render_target::cube_texture->Release();

            env_map_render_target::cube_texture = nullptr;
        }
    }

    void env_x_pos_render_target::close()
    {
        if (env_x_pos_render_target::depth_stencil_surface != nullptr)
        {
            env_x_pos_render_target::depth_stencil_surface->Release();

            env_x_pos_render_target::depth_stencil_surface = nullptr;
        }

        if (env_x_pos_render_target::render_target_surface != nullptr)
        {
            env_x_pos_render_target::render_target_surface->Release();

            env_x_pos_render_target::render_target_surface = nullptr;
        }
    }

    void env_x_neg_render_target::close()
    {
        if (env_x_neg_render_target::depth_stencil_surface != nullptr)
        {
            env_x_neg_render_target::depth_stencil_surface->Release();

            env_x_neg_render_target::depth_stencil_surface = nullptr;
        }

        if (env_x_neg_render_target::render_target_surface != nullptr)
        {
            env_x_neg_render_target::render_target_surface->Release();

            env_x_neg_render_target::render_target_surface = nullptr;
        }
    }

    void env_y_pos_render_target::close()
    {
        if (env_y_pos_render_target::depth_stencil_surface != nullptr)
        {
            env_y_pos_render_target::depth_stencil_surface->Release();

            env_y_pos_render_target::depth_stencil_surface = nullptr;
        }

        if (env_y_pos_render_target::render_target_surface != nullptr)
        {
            env_y_pos_render_target::render_target_surface->Release();

            env_y_pos_render_target::render_target_surface = nullptr;
        }
    }

    void env_y_neg_render_target::close()
    {
        if (env_y_neg_render_target::depth_stencil_surface != nullptr)
        {
            env_y_neg_render_target::depth_stencil_surface->Release();

            env_y_neg_render_target::depth_stencil_surface = nullptr;
        }

        if (env_y_neg_render_target::render_target_surface != nullptr)
        {
            env_y_neg_render_target::render_target_surface->Release();

            env_y_neg_render_target::render_target_surface = nullptr;
        }
    }

    void env_z_pos_render_target::close()
    {
        if (env_z_pos_render_target::depth_stencil_surface != nullptr)
        {
            env_z_pos_render_target::depth_stencil_surface->Release();

            env_z_pos_render_target::depth_stencil_surface = nullptr;
        }

        if (env_z_pos_render_target::render_target_surface != nullptr)
        {
            env_z_pos_render_target::render_target_surface->Release();

            env_z_pos_render_target::render_target_surface = nullptr;
        }
    }

    void env_z_neg_render_target::close()
    {
        if (env_z_neg_render_target::depth_stencil_surface != nullptr)
        {
            env_z_neg_render_target::depth_stencil_surface->Release();

            env_z_neg_render_target::depth_stencil_surface = nullptr;
        }

        if (env_z_neg_render_target::render_target_surface != nullptr)
        {
            env_z_neg_render_target::render_target_surface->Release();

            env_z_neg_render_target::render_target_surface = nullptr;
        }
    }

    void render_view::update(const view::instance& view)
    {
        const camera* camera = view.camera;

        if (camera != nullptr)
        {
            this->has_camera = true;

            camera->get_forward_vector(this->camera_forward);
            camera->get_up_vector(this->camera_up);

            this->camera_position = camera->current_key.position.as_vector3();
            this->camera_prev_forward = camera->previous_key.direction.as_vector3();
            this->camera_velocity = camera->velocity_key.position.as_vector3();
            this->camera_view_matrix = camera->current_key.view_matrix;
            this->camera_focal_distance = camera->current_key.focal_distance;
            this->camera_depth_of_field = camera->current_key.depth_of_field;
            this->camera_dof_falloff = camera->current_key.dof_falloff;
            this->camera_dof_max_intensity = camera->current_key.dof_max_intensity;
        }
        else
        {
            this->has_camera = false;
        }

        if (view.camera_mover_list.empty())
        {
            this->has_camera_mover = false;
        }
        else
        {
            const camera_mover* mover = view.camera_mover_list.begin()->vtable_fix();

            this->has_camera_mover = true;
            this->camera_mover_type = mover->type;
            this->outside_view_type = mover->outside_pov();
        }

        if (view.pinfo != nullptr)
        {
            this->view_matrix = view.pinfo->view_matrix;
            this->projection_matrix = view.pinfo->projection_matrix;
            this->view_projection_matrix = view.pinfo->view_projection_matrix;
            this->non_jittered_view_projection_matrix = view.pinfo->non_jittered_view_projection_matrix;
        }
    }
}
