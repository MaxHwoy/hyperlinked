#include <hyperlib/global_vars.hpp>
#include <hyperlib/memory/frame_pool.hpp>
#include <hyperlib/gameplay/movie.hpp>
#include <hyperlib/gameplay/photo.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/renderer.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/window.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/gamma.hpp>
#include <hyperlib/renderer/culling.hpp>
#include <hyperlib/renderer/time_of_day.hpp>
#include <hyperlib/renderer/post_process.hpp>
#include <hyperlib/renderer/fe_renderer.hpp>
#include <hyperlib/renderer/fog_renderer.hpp>
#include <hyperlib/renderer/blur_renderer.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>
#include <hyperlib/renderer/gauss_renderer.hpp>
#include <hyperlib/renderer/flare_renderer.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlib/renderer/per_view/rvm_renderer.hpp>
#include <hyperlib/renderer/per_view/pip_renderer.hpp>
#include <hyperlib/renderer/per_view/env_map_renderer.hpp>
#include <hyperlib/renderer/per_view/main_view_renderer.hpp>
#include <hyperlib/renderer/per_view/reflection_renderer.hpp>

namespace hyper
{
    bool renderer::reinit_window_params()
    {
        ::HRESULT result;

        if (FAILED(result = directx::reset_parameters()))
        {
            PRINT_FATAL("Unable to reset device, error code 0x%08X", result);

            if (options::antialias_level > 0)
            {
                options::antialias_level = 0;
            }

            std::uint32_t x;
            std::uint32_t y;
            std::uint32_t f = 0u;

            window::get_resolution(x, y);

            directx::create_d3d_present_params(x, y);

            while (FAILED(result = directx::reset_parameters()))
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

    void renderer::open_render_targets()
    {
        player_render_target::open();
        rvm_render_target::open();
        shadowmap_render_target::open();
        env_map_render_target::open();
        motion_blur_render_target::open();
        reflection_render_target::open();
        pip_render_target::open();
    }

    void renderer::close_render_targets()
    {
        player_render_target::close();
        reflection_render_target::close();
        rvm_render_target::close();
        shadowmap_render_target::close();
        pip_render_target::close();
        motion_blur_render_target::close();
        env_map_render_target::close();
    }

    void renderer::setup_car_env_map()
    {
        call_function<void(__cdecl*)()>(0x007D5C60)();
    }

    void renderer::update_view_instances()
    {
        for (size_t i = 0u; i < view::instance::views.length(); ++i)
        {
            view::instance& instance = view::instance::views[i];

            if (instance.active || instance.is_env_map())
            {
                instance.calculate_view_matrices(0.0f, 0.0f, true);
            }
        }
    }

    void renderer::update_render_views()
    {
        for (view_id i = view_id::first; i < view_id::count; ++i)
        {
            render_view::views[i].update(view::instance::views[i]);
        }
    }

    void renderer::update_headlight_views()
    {
        call_function<void(__cdecl*)()>(0x0074A4E0)();
    }

    void renderer::update_world_lights()
    {
        for (const view_id id :
        {
            view_id::player1,
            view_id::player2,
            view_id::player1_rvm,
        })
        {
            view::instance& instance = view::instance::views[id];

            if (instance.active)
            {
                instance.setup_world_light_context();
            }
        }
    }

    void renderer::update_animations()
    {
        texture::update_animations();

        call_function<void(__cdecl*)()>(0x007A1330)();

        flare_renderer::reset();

        global::fac_flush = 0u;
    }

    void renderer::update_sun_moon()
    {
        float d = 0.677f - 0.14f;
        float z = math::clamp(time_of_day::instance->sun_direction.z - 0.14f, 0.0f, 0.677f - 0.14f);

        renderer::moon_brightness.x = z / d * (1.0f - view::instance::views[view_id::player1].rain->cloud_intensity);
        renderer::moon_brightness.y = (0.9f - 0.1f) * renderer::moon_brightness.x + 0.1f;
    }

    void renderer::reset_renderer_state()
    {
        directx::reset_rendering_states();
    }

    bool renderer::wants_renderer_reset()
    {
        return directx::device()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
    }

    void renderer::begin_scene()
    {
        directx::device()->BeginScene();
    }

    void renderer::end_scene()
    {
        directx::device()->EndScene();
    }

    void renderer::pre_display()
    {
        call_function<void(__cdecl*)()>(0x0074E350)();
    }

    void renderer::render_world()
    {
        BENCHMARK();

        if (!options::bone_animations_supported)
        {
            options::nis_anim_scene_disabled = true;
        }

        renderer::low_quality_flags = static_cast<prepass_flags>(0u);

        if (renderer::world_lod_level_low)
        {
            renderer::low_quality_flags |= prepass_flags::exclude_high_quality;
        }

        renderer::no_shadows = options::shadow_detail == 0u;

        fog_renderer::assign_parameters(fog_renderer::params::instance, view::instance::views[view_id::player1]);

        for (const shader_type type :
        {
            shader_type::WorldShader,
            shader_type::CarShader,
            shader_type::WATER,
        })
        {
            fog_renderer::apply_parameters(fog_renderer::params::instance, shader_lib::get_effect(type));
        }

        effect::set_current_effect(nullptr);

        renderer::pre_display();

        time_of_day::instance->update_time();

        renderer::update_sun_moon();

        grand_scenery_cull_info culler;

        culler.setup_world_culling();

        if (options::road_reflection_detail)
        {
            reflection_renderer::render(view::instance::views[view_id::player1_reflection], view::instance::views[view_id::player1], culler);
            reflection_renderer::render(view::instance::views[view_id::player2_reflection], view::instance::views[view_id::player2], culler);
        }

        if (options::vehicle_reflection_rate)
        {
            renderer::envmap_calibration = true;

            renderer::current_cull_mode = ::D3DCULL_CCW;

            if (game_flow::manager::instance.current_state == game_flow::state::racing)
            {
                world_renderer::use_lowlod_pass = true; // #TODO move to env map renderer

                for (const view_id id :
                {
                    view_id::env_z_pos,
                    view_id::env_x_pos,
                    view_id::env_z_neg,
                    view_id::env_x_neg,
                    view_id::env_y_pos,
                    view_id::env_y_neg,
                })
                {
                    env_map_renderer::render(view::instance::views[id], culler);
                }

                world_renderer::use_lowlod_pass = false;
            }
        }
        else if (renderer::envmap_calibration)
        {
            for (const view_id id :
            {
                view_id::env_z_pos,
                view_id::env_x_pos,
                view_id::env_z_neg,
                view_id::env_x_neg,
                view_id::env_y_pos,
                view_id::env_y_neg,
            })
            {
                renderer::set_render_target(*view::instance::views[id].attached_target, true, color32::clear());
            }

            renderer::envmap_calibration = false;
        }

        world_renderer::render();

        renderer::current_cull_mode = ::D3DCULL_CW;

        directx::set_z_write_enable(true, true);

        rvm_renderer::render(view::instance::views[view_id::player1_rvm], culler);

        renderer::reset_renderer_state();

        if (options::motion_blur_enabled)
        {
            renderer::set_render_target(motion_blur_render_target::instance(), true, color32::white());

            blur_renderer::render(blur_renderer::instance, view::instance::views[view_id::player1], culler);
        }

        main_view_renderer::render(view::instance::views[view_id::player1], culler);

        main_view_renderer::render(view::instance::views[view_id::player2], culler);

        pip_renderer::render(view::instance::views[view_id::player1_pip], culler);

        post_process::apply(post_process::instance, view::instance::views[view_id::player1]);
    }

    void renderer::render_fe()
    {
        BENCHMARK();

        call_function<void(__cdecl*)()>(0x00730CB0)();
    }

    void renderer::wait_render()
    {
        BENCHMARK();

        char data[0x04];

        while (directx::query()->GetData(data, sizeof(data), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }

        directx::query()->Issue(D3DISSUE_END);

        if (directx::device()->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST &&
            directx::device()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        {
            renderer::reset();
        }
    }

    void renderer::make_screenshot(const char* name)
    {
        call_function<void(__cdecl*)(const char*)>(0x00710750)(name);
    }

    void renderer::handle_cursor()
    {
        call_function<void(__thiscall*)(void*)>(0x007316A0)(reinterpret_cast<void*>(0x00B1F634));
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

    bool renderer::reset()
    {
        BENCHMARK();

#if !defined(HYPER_USE_CUSTOM_RENDERER_RESET)
        return SUCCEEDED(call_function<::HRESULT(__cdecl*)()>(0x0072B370)());
#else
        call_function<void(__cdecl*)()>(0x00729A90)();
        
        shader_lib::lose_device();

        renderer::close_render_targets();

        gauss_renderer::dtor(gauss_renderer::instance);

        blur_renderer::dtor(blur_renderer::instance);

        fe_renderer::dtor(fe_renderer::instance);
        
        flare_renderer::dtor(flare_renderer::instance);
        
        prelit_pool::dtor(prelit_pool::instance);
        
        post_process::dtor(post_process::instance);
        
        rain_renderer::dtor(rain_renderer::instance);
        
        directx::release_query();

        bool result = renderer::reinit_window_params();

        if (!result)
        {
            return false;
        }
        
        renderer::open_render_targets();

        renderer::init_render_targets();

        post_process::ctor(post_process::instance);

        gauss_renderer::ctor(gauss_renderer::instance);

        call_function<void(__cdecl*)()>(0x0070FCF0)(); // eInitFEEnvMapPlat

        if (options::visual_treatment || options::motion_blur_enabled || options::road_reflection_detail)
        {
            blur_renderer::ctor(blur_renderer::instance);
        }

        rain_renderer::ctor(rain_renderer::instance);

        renderer::reset_renderer_state();
        
        directx::create_query(::D3DQUERYTYPE_EVENT);
        
        directx::query()->Issue(D3DISSUE_END);
        
        shader_lib::reconnect_device();
        
        prelit_pool::ctor(prelit_pool::instance);
        
        fe_renderer::ctor(fe_renderer::instance);
        
        flare_renderer::ctor(flare_renderer::instance);

        return true;
#endif
    }

    void renderer::render()
    {
        BENCHMARK();

        shader_lib::reset();

        effect::set_current_effect(nullptr);

        if (renderer::time_of_day_not_inited)
        {
            time_of_day::init();

            renderer::time_of_day_not_inited = false;
        }

        if (renderer::reinit_renderer)
        {
            shader_lib::release();

            shader_lib::create_pool();

            shader_lib::reinit();

            std::uint32_t x, y;

            window::get_resolution(x, y);

            directx::create_d3d_present_params(x, y);

            renderer::reset();

            renderer::reinit_renderer = false;
        }

        view::mode::maybe_change();

        texture::mipmap_strip_count = 3u - options::world_detail;

        if (options::vehicle_reflection_rate)
        {
            renderer::setup_car_env_map();
        }

        renderer::update_headlight_views();

        renderer::update_view_instances();

        renderer::update_render_views();

        shader_lib::set_headlights();

        renderer::reset_renderer_state();

        bool wants_reset = renderer::wants_renderer_reset();

        if (wants_reset)
        {
            wants_reset = !renderer::reset();
        }

        if (wants_reset)
        {
            std::uint32_t ticker = utils::get_ticker();

            while (utils::get_ticker_difference(ticker) < 50.0f)
            {
                ::Sleep(1u);
            }
        }
        else
        {
            renderer::begin_scene();

            renderer::set_render_target(render_target::targets[render_target_id::player], true, color32::clear());

            renderer::current_cull_mode = ::D3DCULL_CW;

            if (movie::instance == nullptr || !movie::instance->is_playing())
            {
                renderer::update_world_lights();

                if (!global::should_exit_game)
                {
                    renderer::reset_renderer_state();
                }

                renderer::render_world();

                world_renderer::render();
            }

            if (photo::mode::instance.get_state() == photo::state::initializing)
            {
                photo::mode::instance.take_photo();
            }

            renderer::render_fe();

            renderer::end_scene();
        }

        if (renderer::take_screenshot)
        {
            renderer::make_screenshot(options::screenshot_directory.pointer());

            renderer::take_screenshot = 0;
        }

        renderer::wait_render();

        renderer::update_animations();

        frame_pool::reset_buffers();

        frame_pool::instance.reset();

        global::frame_counter++;

        world_renderer::reset();

        renderer::handle_cursor();

        gamma::instance->set_brightness_level(options::brightness * 0.01f);

        gamma::instance->set_brightness();
    }
}
