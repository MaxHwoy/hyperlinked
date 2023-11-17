#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/gameplay/dal.hpp>
#include <hyperlib/gameplay/g_race.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/streamer/parameter_map.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    void renderer::create_rendering_model(geometry::mesh_entry* entry, geometry::solid* solid, draw_flags flags, hyper::effect* effect, texture::info** textures, const matrix4x4* trs, const lighting::dynamic_context* context, const light_material::instance* material, const matrix4x4* blend_trs, pca::blend_data* pca)
    {
        BENCHMARK();

        if (renderer::rendering_model_count_ < renderer::rendering_models_.length())
        {
            rendering_model& model = renderer::rendering_models_[renderer::rendering_model_count_];

            model.base_texture_info = textures[0];
            model.diffuse_texture_info = textures[0];
            model.normal_texture_info = textures[1];
            model.height_texture_info = textures[2];
            model.specular_texture_info = textures[3];
            model.opacity_texture_info = textures[4];
            model.d3d9_diffuse_texture = model.diffuse_texture_info->pinfo->texture;
            model.d3d9_normal_texture = model.normal_texture_info->pinfo->texture;
            model.d3d9_height_texture = model.height_texture_info->pinfo->texture;
            model.d3d9_specular_texture = model.specular_texture_info->pinfo->texture;
            model.d3d9_opacity_texture = model.opacity_texture_info->pinfo->texture;
            model.render_bits = model.base_texture_info->pinfo->state;
            model.light_context = context;
            model.light_material = material;
            model.solid = solid;
            model.mesh_entry = entry;
            model.blending_matrices = blend_trs;
            model.local_to_world = trs;
            model.is_tri_stripped = false;
            model.flags = flags;
            model.effect = effect;
            model.null = nullptr;
            model.negative_one = -1.0f;
            model.blend_data = pca;
            model.use_low_lod = effect->has_low_lod_technique() && (flags & draw_flags::use_low_lod) != 0u;
            model.render_bits.wants_auxiliary_textures = true;

            renderer::compute_sort_key(model);

            rendering_order& order = renderer::rendering_orders_[renderer::rendering_model_count_];

            order.model_index = renderer::rendering_model_count_++;
            order.sort_flags = model.sort_flags;
        }
    }

    void renderer::compute_sort_key(rendering_model& model)
    {
        if (model.is_tri_stripped)
        {
            model.sort_flags = 0u;
        }
        else
        {
            constexpr std::int32_t super_sort_flag = std::numeric_limits<std::int32_t>::min();

            const geometry::mesh_entry* entry = model.mesh_entry;

            if (model.base_texture_info->apply_alpha_sorting != 0u)
            {
                const view::instance& view = hyper::view::instance::views[render_target::current->view_id];

                float depth = view.get_screen_depth(entry->bbox_min, entry->bbox_max, model.local_to_world);

                model.sort_flags = static_cast<std::int32_t>(math::float_bits(depth)) | super_sort_flag;
            }
            else
            {
                std::uint8_t rendering_order = model.base_texture_info->rendering_order;

                if (rendering_order == 0u)
                {
                    std::int32_t flags = static_cast<std::int32_t>((static_cast<std::uint32_t>(model.flags) ^ entry->material_key)) & 0xFFFFFF;

                    flags |= model.render_bits.sub_sort_key << 24;

                    flags |= static_cast<std::uint32_t>(model.use_low_lod) << 25;

                    model.sort_flags = flags | ((static_cast<std::uint32_t>(model.effect->id()) & 0x1Fu) << 26);
                }
                else
                {
                    model.sort_flags = rendering_order | super_sort_flag;
                }
            }
        }
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

    void renderer::set_render_target(render_target& target, bool clear, ::D3DCOLOR clear_color)
    {
        renderer::sort_models_and_draw_world();

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

    void renderer::sort_models_and_draw_world()
    {
        call_function<void(__cdecl*)()>(0x0072C9B0)();
    }
}
