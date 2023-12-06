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
    void renderer::render_world_in_game()
    {
        std::uint32_t count = renderer::rendering_model_count_;

        effect* effect = nullptr;

        std::int32_t use_low_lod = -1;

        for (std::uint32_t i = 0u; i < count; ++i)
        {
            rendering_model& model = renderer::rendering_models_[renderer::rendering_orders_[i].model_index];

            if (effect != model.effect || use_low_lod != static_cast<std::int32_t>(model.use_low_lod))
            {
                if (effect != nullptr)
                {
                    shader_lib::end_effect(*effect);
                }

                model.effect->set_current_pass(0u, nullptr, model.use_low_lod);

                use_low_lod = static_cast<std::int32_t>(model.use_low_lod);

                effect = model.effect;

                renderer::total_pass_swaps_++;
            }

            ASSERT(effect != nullptr);

            const render_view& view = render_view::views[render_target::current->view_id];

            effect->set_transforms(*model.local_to_world, view, model.base_texture_info->bias_level != 0u);

            renderer::total_transforms_set_++;

            if (model.local_to_world == &matrix4x4::identity())
            {
                renderer::total_identity_matrices_encountered_++;
            }

            if (model.light_context != nullptr)
            {
                effect->set_light_context(*model.light_context, *model.local_to_world);
            }

            if (model.light_material != nullptr)
            {
                effect->handle_material_data(*model.light_material, model.flags);
            }

            if (model.blend_data != nullptr)
            {
                effect->set_pca_blend_data(*model.blend_data);
            }

            if (model.is_tri_stripped)
            {
                const strip& strip = *model.mesh.strip;

                effect->set_diffuse_map(model);

                effect->set_texture_animation(*model.diffuse_texture_info);

                directx::device()->SetRenderState(::D3DRS_CULLMODE, ::D3DCULL_NONE);
                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, 0u);

                renderer::currently_set_vertex_buffer_ = nullptr;

                effect->commit_changes();

                for (std::uint32_t k = 0u; k < effect->pass_count(); ++k)
                {
                    directx::device()->DrawPrimitiveUP(::D3DPT_TRIANGLESTRIP, strip.vertex_count - 2u, strip.vertices, sizeof(strip::vertex));
                }

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, 1u);

                renderer::total_strips_drawn_++;

                strip::pool->free(model.mesh.strip);
            }
            else
            {
                geometry::platform_info* info = model.solid->pinfo;

                const geometry::mesh_entry& entry = *model.mesh.entry;

                effect->set_texture_maps(model, model.flags);

                if (model.blending_matrices != nullptr)
                {
                    effect->set_blend_matrices(model.blending_matrices, entry);
                }

                if (entry.d3d_vertex_buffer == nullptr && entry.vertex_count != 0u)
                {
                    if (renderer::vertex_buffers_created_this_frame_ >= 3 || info->d3d_index_buffer == nullptr)
                    {
                        continue;
                    }

                    info->create_vertex_buffers();

                    renderer::vertex_buffers_created_this_frame_++;
                }

                if (entry.d3d_vertex_buffer != renderer::currently_set_vertex_buffer_)
                {
                    renderer::total_vertex_buffer_swaps_++;

                    renderer::currently_set_vertex_buffer_ = entry.d3d_vertex_buffer;

                    directx::device()->SetStreamSource(0u, entry.d3d_vertex_buffer, 0u, effect->stride());
                }

                if (info->d3d_index_buffer != renderer::currently_set_index_buffer_)
                {
                    renderer::total_index_buffer_swaps_++;

                    renderer::currently_set_index_buffer_ = info->d3d_index_buffer;

                    directx::device()->SetIndices(info->d3d_index_buffer);
                }

                if (info->polygon_count != 0u && entry.vertex_count < std::numeric_limits<std::uint16_t>::max())
                {
                    if (entry.triangle_count != 0u && info->d3d_index_buffer != nullptr)
                    {
                        effect->commit_and_draw_indexed(entry.d3d_vertex_count, entry.index_start, entry.triangle_count, model);
                    }
                }
            }
        }

        if (effect != nullptr)
        {
            shader_lib::end_effect(*effect);
        }

        renderer::currently_set_index_buffer_ = nullptr;
        renderer::currently_set_vertex_buffer_ = nullptr;
        renderer::rendering_model_count_ = 0u;
        renderer::some_value_00AB0BEC = 0u;
    }

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
            model.mesh.entry = entry;
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

            const geometry::mesh_entry* entry = model.mesh.entry;

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
        if (renderer::rendering_model_count_ == 0u)
        {
            renderer::some_value_00AB0BE8 = 0u;
        }
        else
        {
            std::sort(renderer::rendering_orders_, renderer::rendering_orders_ + renderer::rendering_model_count_);

            renderer::render_world_in_game();

            renderer::some_value_00A65240 = -1;
        }
    }
}
