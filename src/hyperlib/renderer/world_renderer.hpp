#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/pca.hpp>
#include <hyperlib/assets/lights.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    struct strip
    {
    public:
        struct vertex
        {
            vector3 position;
            color32 color;
            vector2 uv;
        };

    public:
        std::uint32_t vertex_count;
        vertex vertices[0x40];

    public:
        static inline instance_pool<strip>*& pool = *reinterpret_cast<instance_pool<strip>**>(0x00AB09AC);
    };

    struct rendering_model
    {
        texture::render_state render_bits;
        texture::info* base_texture_info;
        ::IDirect3DTexture9* d3d9_diffuse_texture;
        ::IDirect3DTexture9* d3d9_normal_texture;
        ::IDirect3DTexture9* d3d9_height_texture;
        ::IDirect3DTexture9* d3d9_specular_texture;
        ::IDirect3DTexture9* d3d9_opacity_texture;
        union {
            geometry::mesh_entry* entry;
            hyper::strip* strip;
        } mesh;
        bool is_tri_stripped;
        geometry::solid* solid;
        draw_flags flags;
        effect* effect;
        const light::context::dynamic* light_context;
        const light_material::instance* light_material;
        const matrix4x4* local_to_world;
        const matrix4x4* blending_matrices;
        const texture::info* diffuse_texture_info;
        const texture::info* normal_texture_info;
        const texture::info* height_texture_info;
        const texture::info* specular_texture_info;
        const texture::info* opacity_texture_info;
        std::int32_t sort_flags;
        void* null;
        float negative_one;
        pca::blend_data* blend_data;
        bool use_low_lod;
    };

    struct rendering_order
    {
        std::uint32_t model_index;
        std::int32_t sort_flags;

        inline bool operator <(const rendering_order& other) const
        {
            return *reinterpret_cast<const std::uint64_t*>(this) < *reinterpret_cast<const std::uint64_t*>(&other);
        }
    };

    class world_renderer final
    {
    private:
        static void render_internal();

        static void draw_indexed_primitive(effect& effect, const rendering_model& model, const render_view& view);

    public:
        static void create_rendering_model(geometry::mesh_entry& entry, geometry::solid* solid, draw_flags flags, effect* effect, texture::info** textures, const matrix4x4* trs, const light::context::dynamic* context, const light_material::instance* material, const matrix4x4* blend_trs, pca::blend_data* pca);

        static void create_rendering_strip(strip& strip, draw_flags flags, effect* effect, texture::info* texture, const matrix4x4* trs, const light::context::dynamic* context, const light_material::instance* material);

        static void compute_sort_key(rendering_model& model);

        static void reset();

        static void render();

        static void depth_prepass();

    public:
        static inline bool& draw_world = *reinterpret_cast<bool*>(0x00A63E0C);

        static inline float& wind_angle = *reinterpret_cast<float*>(0x00B74D48);

        static inline bool& use_lowlod_pass = *reinterpret_cast<bool*>(0x00AB0B98);

    private:
        static inline array<rendering_model, 0x1000> rendering_models_ = array<rendering_model, 0x1000>(0x00AB2780);

        static inline rendering_order*& rendering_orders_ = *reinterpret_cast<rendering_order**>(0x00B1DB90);

        static inline std::uint32_t& rendering_model_count_ = *reinterpret_cast<std::uint32_t*>(0x00AB0BF0);

        static inline std::uint32_t& some_value_00AB0BE8 = *reinterpret_cast<std::uint32_t*>(0x00AB0BE8);

        static inline std::uint32_t& some_value_00AB0BEC = *reinterpret_cast<std::uint32_t*>(0x00AB0BEC);

        static inline std::int32_t& some_value_00A65240 = *reinterpret_cast<std::int32_t*>(0x00A65240);

        static inline std::uint32_t& total_pass_swaps_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF64);

        static inline std::uint32_t& total_index_buffer_swaps_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF4C);

        static inline std::uint32_t& total_vertex_buffer_swaps_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF48);

        static inline std::uint32_t& total_strips_drawn_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF44);

        static inline std::uint32_t& total_transforms_set_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF54);

        static inline std::uint32_t& total_identity_matrices_encountered_ = *reinterpret_cast<std::uint32_t*>(0x00B1DF60);

        static inline std::uint32_t& vertex_buffers_created_this_frame_ = *reinterpret_cast<std::uint32_t*>(0x00AB0BF8);

        static inline ::IDirect3DIndexBuffer9*& currently_set_index_buffer_ = *reinterpret_cast<::IDirect3DIndexBuffer9**>(0x00AB0BB8);

        static inline ::IDirect3DVertexBuffer9*& currently_set_vertex_buffer_ = *reinterpret_cast<::IDirect3DVertexBuffer9**>(0x00AB0BB4);
    };

    ASSERT_SIZE(strip::vertex, 0x18);
    ASSERT_SIZE(strip, 0x604);
    ASSERT_SIZE(rendering_model, 0x68);
}
