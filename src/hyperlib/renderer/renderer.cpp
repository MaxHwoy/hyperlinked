#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/window.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/fe_renderer.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/flare_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>

namespace hyper
{
    bool renderer::reinit_window_params()
    {
        directx::reset_parameters(); // #TODO do we REALLY need a double call here?

        if (FAILED(directx::reset_parameters()))
        {
            if (directx::antialias_level > 0)
            {
                directx::antialias_level = 0;
            }

            std::uint32_t x;
            std::uint32_t y;
            std::uint32_t f = 0u;

            window::get_resolution(x, y);

            directx::create_d3d_present_params(x, y);

            while (FAILED(directx::reset_parameters()))
            {
                if (++f >= 3)
                {
                    while (true)
                    {
                        if (window::racing_resolution == 0u)
                        {
                            PRINT_FATAL("Unable to properly reset DirectX device!");

                            return false;
                        }

                        window::racing_resolution--;

                        window::get_resolution(x, y);

                        directx::create_d3d_present_params(x, y);

                        if (SUCCEEDED(directx::reset_parameters()))
                        {
                            break;
                        }
                    }
                }
            }
        }

        return true;
    }

    void renderer::init_render_targets()
    {
        vt_render_target::init();
        player_render_target::init();
        reflection_render_target::init();
        flailer_render_target::init();
        rvm_render_target::init();
        shadowmap_render_target::init();
        pip_render_target::init();
        motion_blur_render_target::init();
        env_x_pos_render_target::init();
        env_x_neg_render_target::init();
        env_y_pos_render_target::init();
        env_y_neg_render_target::init();
        env_z_pos_render_target::init();
        env_z_neg_render_target::init();
    }

    void renderer::close_render_targets()
    {
        player_render_target::close();
        reflection_render_target::close();
        rvm_render_target::close();
        shadowmap_render_target::close();
        pip_render_target::close();
        motion_blur_render_target::close();
        env_x_pos_render_target::close();
        env_x_neg_render_target::close();
        env_y_pos_render_target::close();
        env_y_neg_render_target::close();
        env_z_pos_render_target::close();
        env_z_neg_render_target::close();
        env_map_render_target::close();
    }

    void renderer::set_render_target(render_target& target, bool clear, ::D3DCOLOR clear_color)
    {
        world_renderer::render();

        directx::device()->SetRenderTarget(0u, target.d3d_target);
        directx::device()->SetDepthStencilSurface(target.d3d_depth_stencil);

        if (clear)
        {
            if (target.target_id == render_target_id::flailer)
            {
                directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET, clear_color, 1.0f, 0u);
            }
            else
            {
                directx::device()->Clear(0u, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, clear_color, 1.0f, 0u);
            }
        }

        ::D3DVIEWPORT9 viewport;

        viewport.Width = target.resolution_x;
        viewport.Height = target.resolution_y;
        viewport.X = 0u;
        viewport.Y = 0u;
        viewport.MinZ = 0.0f;
        viewport.MaxZ = 1.0f;

        directx::device()->SetViewport(&viewport);

        render_target::current = &target;
    }

    void renderer::update_render_views()
    {
        for (view_id i = view_id::first; i < view_id::count; ++i)
        {
            render_view::views[i].update(view::instance::views[i]);
        }
    }

    void renderer::reset()
    {
        call_function<void(__cdecl*)()>(0x00729A90)();

        shader_lib::lose_device();
        
        renderer::close_render_targets();
        
        call_function<void(__cdecl*)()>(0x0073C410)();
        
        fe_renderer::dtor(fe_renderer::instance);
        
        flare_renderer::dtor(flare_renderer::instance);
        
        prelit_pool::dtor(prelit_pool::instance);
        
        call_function<void(__cdecl*)()>(0x00712E90);
        
        directx::release_query();
        
        rain_renderer::dtor(rain_renderer::instance);

        if (!renderer::reinit_window_params())
        {
            return;
        }
        
        player_render_target::open();
        rvm_render_target::open();
        motion_blur_render_target::open();
        
        if (world_renderer::shadow_detail > 0u)
        {
            shadowmap_render_target::open();
        }

        rain_renderer::ctor(rain_renderer::instance);

        directx::reset_rendering_states();
        
        directx::create_query(::D3DQUERYTYPE_EVENT);
        
        directx::query()->Issue(D3DISSUE_END);
        
        shader_lib::reconnect_device();
        
        prelit_pool::ctor(prelit_pool::instance);
        
        fe_renderer::ctor(fe_renderer::instance);
        
        flare_renderer::ctor(flare_renderer::instance);
    }

    void renderer::render()
    {

    }
}
