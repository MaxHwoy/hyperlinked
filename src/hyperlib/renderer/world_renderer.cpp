#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/gameplay/dal.hpp>
#include <hyperlib/gameplay/g_race.hpp>
#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/streamer/parameter_map.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/world_renderer.hpp>

namespace hyper
{
    void world_renderer::render_internal()
    {
        BENCHMARK();

        std::uint32_t count = world_renderer::rendering_model_count_;

        effect* effect = nullptr;

        std::int32_t use_low_lod = -1;

        for (std::uint32_t i = 0u; i < count; ++i)
        {
            rendering_model& model = world_renderer::rendering_models_[world_renderer::rendering_orders_[i].model_index];

            if (effect != model.effect || use_low_lod != static_cast<std::int32_t>(model.use_low_lod))
            {
                if (effect != nullptr)
                {
                    shader_lib::end_effect(*effect);
                }

                model.effect->set_current_pass(0u, nullptr, model.use_low_lod);

                use_low_lod = static_cast<std::int32_t>(model.use_low_lod);

                effect = model.effect;

                world_renderer::total_pass_swaps_++;
            }

            ASSERT(effect != nullptr);

            const render_view& view = render_view::views[render_target::current->view_id];

            effect->set_transforms(*model.local_to_world, view, model.base_texture_info->bias_level != 0u);

            world_renderer::total_transforms_set_++;

            if (model.local_to_world == &matrix4x4::identity())
            {
                world_renderer::total_identity_matrices_encountered_++;
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

                effect->set_texture_page(*model.diffuse_texture_info);

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, 0u);

                world_renderer::currently_set_vertex_buffer_ = nullptr;

                effect->commit_changes();

                for (std::uint32_t k = 0u; k < effect->pass_count(); ++k)
                {
                    directx::device()->DrawPrimitiveUP(::D3DPT_TRIANGLESTRIP, strip.vertex_count - 2u, strip.vertices, sizeof(strip::vertex));
                }

                directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, 1u);

                world_renderer::total_strips_drawn_++;

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
                    if (world_renderer::vertex_buffers_created_this_frame_ >= 3 || info->d3d_index_buffer == nullptr)
                    {
                        continue;
                    }

                    info->create_vertex_buffers();

                    world_renderer::vertex_buffers_created_this_frame_++;
                }

                if (entry.d3d_vertex_buffer != world_renderer::currently_set_vertex_buffer_)
                {
                    world_renderer::total_vertex_buffer_swaps_++;

                    world_renderer::currently_set_vertex_buffer_ = entry.d3d_vertex_buffer;

                    directx::device()->SetStreamSource(0u, entry.d3d_vertex_buffer, 0u, effect->stride());
                }

                if (info->d3d_index_buffer != world_renderer::currently_set_index_buffer_)
                {
                    world_renderer::total_index_buffer_swaps_++;

                    world_renderer::currently_set_index_buffer_ = info->d3d_index_buffer;

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

        world_renderer::currently_set_index_buffer_ = nullptr;
        world_renderer::currently_set_vertex_buffer_ = nullptr;
        world_renderer::rendering_model_count_ = 0u;
        world_renderer::some_value_00AB0BEC = 0u;
    }

    void world_renderer::create_rendering_model(geometry::mesh_entry& entry, geometry::solid* solid, draw_flags flags, hyper::effect* effect, texture::info** textures, const matrix4x4* trs, const light::context::dynamic* context, const light_material::instance* material, const matrix4x4* blend_trs, pca::blend_data* pca)
    {
        BENCHMARK();

        if (world_renderer::rendering_model_count_ < world_renderer::rendering_models_.length())
        {
            rendering_model& model = world_renderer::rendering_models_[world_renderer::rendering_model_count_];

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
            model.mesh.entry = &entry;
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

            world_renderer::compute_sort_key(model);

            rendering_order& order = world_renderer::rendering_orders_[world_renderer::rendering_model_count_];

            order.model_index = world_renderer::rendering_model_count_++;
            order.sort_flags = model.sort_flags;
        }
    }

    void world_renderer::compute_sort_key(rendering_model& model)
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

    void world_renderer::reset()
    {
        world_renderer::currently_set_index_buffer_ = nullptr;
        world_renderer::currently_set_vertex_buffer_ = nullptr;
    }

    void world_renderer::render()
    {
        BENCHMARK();

        if (world_renderer::rendering_model_count_ == 0u)
        {
            world_renderer::some_value_00AB0BE8 = 0u;
        }
        else
        {
            std::sort(world_renderer::rendering_orders_, world_renderer::rendering_orders_ + world_renderer::rendering_model_count_);

            world_renderer::render_internal();

            world_renderer::some_value_00A65240 = -1;
        }
    }
}
