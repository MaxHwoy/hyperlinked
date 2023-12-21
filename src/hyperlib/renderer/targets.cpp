#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/renderer.hpp>

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

    bool player_render_target::open()
    {
        player_render_target::d3d_texture = nullptr;

        if (FAILED(directx::device()->GetBackBuffer(0u, 0u, ::D3DBACKBUFFER_TYPE_MONO, &player_render_target::render_target_surface_postprocess)))
        {
            return ::assert_return(false);
        }

        if (FAILED(directx::device()->GetDepthStencilSurface(&player_render_target::depth_stencil_surface)))
        {
            return ::assert_return(false);
        }

        if (options::visual_treatment)
        {
            ::D3DMULTISAMPLE_TYPE multisample = static_cast<::D3DMULTISAMPLE_TYPE>(options::antialias_level > 0);

            ::HRESULT result;

            if (FAILED(result = directx::device()->CreateRenderTarget(directx::resolution_x, directx::resolution_y, ::D3DFMT_A8R8G8B8, multisample, options::antialias_level, false, &player_render_target::render_target_surface_standalone, nullptr)))
            {
                return ::assert_return(false);
            }
        }

        return true;
    }

    bool reflection_render_target::open()
    {
        reflection_render_target::close();

        if (options::road_reflection_detail)
        {
            std::uint32_t x = reflection_render_target::resolution_x = math::floor_pow_2(directx::resolution_x >> 1); // originally directx::resolution_x / 3
            std::uint32_t y = reflection_render_target::resolution_y = math::floor_pow_2(directx::resolution_y >> 1); // originally directx::resolution_y / 3

            if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &reflection_render_target::d3d_texture, nullptr)))
            {
                return options::road_reflection_detail = assert_return(false);
            }

            if (FAILED(directx::device()->CreateDepthStencilSurface(x, y, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &reflection_render_target::depth_stencil_surface, nullptr)))
            {
                return options::road_reflection_detail = assert_return(false);
            }

            if (FAILED(reflection_render_target::d3d_texture->GetSurfaceLevel(0u, &reflection_render_target::render_target_surface)))
            {
                return options::road_reflection_detail = assert_return(false);
            }
        }
        else
        {
            std::uint32_t x = reflection_render_target::resolution_x = 1u;
            std::uint32_t y = reflection_render_target::resolution_y = 1u;

            if (FAILED(directx::device()->CreateTexture(x, y, 1u, 0u, ::D3DFMT_A8R8G8B8, ::D3DPOOL_MANAGED, &reflection_render_target::d3d_texture, nullptr)))
            {
                return assert_return(false);
            }

            directx::fill_with_color(reflection_render_target::d3d_texture, 0x00u);
        }

        return true;
    }

    bool rvm_render_target::open()
    {
        std::uint32_t x = rvm_render_target::resolution_x = math::floor_pow_2(directx::resolution_x >> 1); // originally directx::resolution_x / 3
        std::uint32_t y = rvm_render_target::resolution_y = math::floor_pow_2(directx::resolution_y >> 1); // originally directx::resolution_y / 3

        if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &rvm_render_target::d3d_texture, nullptr)))
        {
            return ::assert_return(false);
        }

        if (FAILED(directx::device()->CreateDepthStencilSurface(x, y, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &rvm_render_target::depth_stencil_surface, nullptr)))
        {
            return ::assert_return(false);
        }

        if (FAILED(rvm_render_target::d3d_texture->GetSurfaceLevel(0u, &rvm_render_target::render_target_surface)))
        {
            return ::assert_return(false);
        }

        return true;
    }

    bool shadowmap_render_target::open()
    {
        if (options::shadow_detail > 0u)
        {
            std::uint32_t x = shadowmap_render_target::resolution_x;
            std::uint32_t y = shadowmap_render_target::resolution_y;

            if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &shadowmap_render_target::render_target_texture, nullptr)))
            {
                return ::assert_return(false);
            }

            ::HRESULT result = S_OK;

            if (shadowmap_render_target::shadow_target_type == target_type::stencil_d24s8)
            {
                result = directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_DEPTHSTENCIL, ::D3DFMT_D24S8, ::D3DPOOL_DEFAULT, &shadowmap_render_target::depth_stencil_texture, nullptr);
            }
            else if (shadowmap_render_target::shadow_target_type == target_type::stencil_df16)
            {
                result = directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_DEPTHSTENCIL, static_cast<::D3DFORMAT>(MAKEFOURCC('D', 'F', '1', '6')), ::D3DPOOL_DEFAULT, &shadowmap_render_target::depth_stencil_texture, nullptr);
            }

            if (FAILED(result))
            {
                return ::assert_return(false);
            }

            if (FAILED(shadowmap_render_target::render_target_texture->GetSurfaceLevel(0u, &shadowmap_render_target::render_target_surface)))
            {
                return ::assert_return(false);
            }

            if (FAILED(shadowmap_render_target::depth_stencil_texture->GetSurfaceLevel(0u, &shadowmap_render_target::depth_stencil_surface)))
            {
                return ::assert_return(false);
            }
        }

        return true;
    }

    bool pip_render_target::open()
    {
        std::uint32_t x = pip_render_target::resolution_x;
        std::uint32_t y = pip_render_target::resolution_y;

        if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &pip_render_target::d3d_texture, nullptr)))
        {
            return ::assert_return(false);
        }

        if (FAILED(directx::device()->CreateDepthStencilSurface(x, y, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, true, &pip_render_target::depth_stencil_surface, nullptr)))
        {
            return ::assert_return(false);
        }

        if (FAILED(pip_render_target::d3d_texture->GetSurfaceLevel(0u, &pip_render_target::render_target_surface)))
        {
            return ::assert_return(false);
        }

        return true;
    }

    bool motion_blur_render_target::open()
    {
        if (options::motion_blur_enabled)
        {
            std::uint32_t x = directx::resolution_x;
            std::uint32_t y = directx::resolution_y;

            if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A16B16G16R16F, ::D3DPOOL_DEFAULT, &motion_blur_render_target::d3d_texture, nullptr)))
            {
                return ::assert_return(false);
            }

            if (FAILED(directx::device()->CreateDepthStencilSurface(x, y, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &motion_blur_render_target::depth_stencil_surface, nullptr)))
            {
                return ::assert_return(false);
            }

            if (FAILED(motion_blur_render_target::d3d_texture->GetSurfaceLevel(0u, &motion_blur_render_target::render_target_surface)))
            {
                return ::assert_return(false);
            }
        }

        return true;
    }

    bool env_map_render_target::open()
    {
        env_map_render_target::cube_texture = nullptr;
        env_map_render_target::fe_texture = nullptr;

        if (options::envmap_textures_enabled)
        {
            std::uint32_t res = env_map_render_target::resolution_cube = options::vehicle_reflection_rate != 0u ? 256u : 16u;
            
            bool failed = false;

            if (FAILED(directx::device()->CreateCubeTexture(res, 1u, 0u, ::D3DFMT_DXT1, ::D3DPOOL_MANAGED, &env_map_render_target::fe_texture, nullptr)) ||
                FAILED(directx::device()->CreateCubeTexture(res, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &env_map_render_target::cube_texture, nullptr)))
            {
                failed = true;
            }

            if (!failed)
            {
                if (!env_x_pos_render_target::open() ||
                    !env_x_neg_render_target::open() ||
                    !env_y_pos_render_target::open() ||
                    !env_y_neg_render_target::open() ||
                    !env_z_pos_render_target::open() ||
                    !env_z_neg_render_target::open())
                {
                    failed = true;
                }
            }

            if (failed)
            {
                options::envmap_textures_enabled = false;
                options::vehicle_reflection_rate = 0u;
                options::max_vehicle_reflection_rate = 0u;

                env_map_render_target::close();
            }
        }

        renderer::envmap_calibration = true;

        return true;
    }

    bool env_x_pos_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_x_pos_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_POSITIVE_X, 0u, &env_x_pos_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
    }

    bool env_x_neg_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_x_neg_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_NEGATIVE_X, 0u, &env_x_neg_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
    }

    bool env_y_pos_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_y_pos_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_POSITIVE_Y, 0u, &env_y_pos_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
    }

    bool env_y_neg_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_y_neg_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_NEGATIVE_Y, 0u, &env_y_neg_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
    }

    bool env_z_pos_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_z_pos_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_POSITIVE_Z, 0u, &env_z_pos_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
    }

    bool env_z_neg_render_target::open()
    {
        std::uint32_t res = env_map_render_target::resolution_cube;

        if (FAILED(directx::device()->CreateDepthStencilSurface(res, res, ::D3DFMT_D24S8, ::D3DMULTISAMPLE_NONE, 0u, false, &env_z_neg_render_target::depth_stencil_surface, nullptr)))
        {
            return assert_return(false);
        }

        if (FAILED(env_map_render_target::cube_texture->GetCubeMapSurface(::D3DCUBEMAP_FACE_NEGATIVE_Z, 0u, &env_z_neg_render_target::render_target_surface)))
        {
            return assert_return(false);
        }

        return true;
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

        if (pip_render_target::d3d_texture != nullptr)
        {
            pip_render_target::d3d_texture->Release();

            pip_render_target::d3d_texture = nullptr;
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
        env_x_pos_render_target::close();
        env_x_neg_render_target::close();
        env_y_pos_render_target::close();
        env_y_neg_render_target::close();
        env_z_pos_render_target::close();
        env_z_neg_render_target::close();

        if (env_map_render_target::fe_texture != nullptr)
        {
            env_map_render_target::fe_texture->Release();

            env_map_render_target::fe_texture = nullptr;
        }

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

    bool render_view::get_has_camera() const
    {
        return call_function<bool(__thiscall*)(const render_view*)>(0x0073EC60)(this);
    }

    auto render_view::get_focal_distance() const -> float
    {
        return call_function<float(__thiscall*)(const render_view*)>(0x0073ECA0)(this);
    }

    auto render_view::get_camera_dof() const -> float
    {
        return call_function<float(__thiscall*)(const render_view*)>(0x0073ECB0)(this);
    }

    auto render_view::get_camera_dof_falloff() const -> float
    {
        return call_function<float(__thiscall*)(const render_view*)>(0x0073ECC0)(this);
    }

    auto render_view::get_camera_dof_max_intensity() const -> float
    {
        return call_function<float(__thiscall*)(const render_view*)>(0x0073ECD0)(this);
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
            const camera_mover* mover = view.camera_mover_list.begin();

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
