#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/slot_pool.hpp>
#include <hyperlib/assets/loader.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/collections/eastl.hpp>

namespace hyper
{
    enum class shader_type : std::uint32_t;

    class light_material final
    {
    public:
        struct platform_info
        {
        };

        struct platform_interface
        {
            platform_info* info;
        };

        struct data
        {
            float diffuse_power;
            float diffuse_clamp;
            float diffuse_flakes;
            float diffuse_vinyl_scale;
            float diffuse_min_scale;
            float diffuse_min_r;
            float diffuse_min_g;
            float diffuse_min_b;
            float diffuse_max_scale;
            float diffuse_max_r;
            float diffuse_max_g;
            float diffuse_max_b;
            float diffuse_min_a;
            float diffuse_max_a;
            float specular_power;
            float specular_flakes;
            float specular_vinyl_scale;
            float specular_min_scale;
            float specular_min_r;
            float specular_min_g;
            float specular_min_b;
            float specular_max_scale;
            float specular_max_r;
            float specular_max_g;
            float specular_max_b;
            float envmap_power;
            float envmap_clamp;
            float envmap_vinyl_scale;
            float envmap_min_scale;
            float envmap_min_r;
            float envmap_min_g;
            float envmap_min_b;
            float envmap_max_scale;
            float envmap_max_r;
            float envmap_max_g;
            float envmap_max_b;
            float vinyl_luminance_min_scale;
            float vinyl_luminance_max_scale;
        };

        struct instance : public platform_interface, public linked_node<instance>
        {
            std::uint32_t key;
            std::uint32_t version;
            std::uint8_t name[64];
            data material;
        };

        static inline float envmap_scale = 0.5f;
        static inline float& spec_scale = *reinterpret_cast<float*>(0x00A6B980);
        static inline float& min_clamp = *reinterpret_cast<float*>(0x009E8FBC);
    };

    class geometry final
    {
    public:
        struct model;

        struct texture_entry
        {
            std::uint32_t key;
            texture::info* texture_onfo;
        };

        struct light_material_entry
        {
            std::uint32_t key;
            light_material::instance* light_material;
        };

        struct position_marker
        {
            std::uint32_t key;
            std::int32_t iparam;
            float fparam;
            color32 tint;
            matrix4x4 matrix;
        };

        struct smooth_vertex
        {
            std::uint32_t vertex_key;
            char smoothing_group_number;
            char normal_x;
            char normal_y;
            char normal_z;
        };

        struct smooth_vertex_info
        {
            std::uint32_t vertex_key;
            std::uint32_t smoothing_group;
            std::uint32_t vertex_offset;
        };

        struct normal_smoother
        {
            smooth_vertex* smooth_vertex_table;
            smooth_vertex_info* smooth_vertex_info_table;
            std::uint16_t smooth_vertex_count;
            std::uint16_t smooth_vertex_info_count;
        };

        struct morph_target
        {
            std::uint32_t key;
            struct solid* target;
            float blend_amount;
        };

        struct edge
        {
            std::uint16_t vertex_index[2];
            std::uint16_t counter;
            std::uint16_t ref_count;
            std::uint16_t mesh_entry_index;
        };

        struct edge_list
        {
            std::uint16_t is_looped;
            std::uint16_t edge_count;
            edge* edges;
        };

        struct selection_set
        {
            std::uint32_t key;
            std::uint32_t solid_key;
            std::uint16_t edge_count;
            __declspec(align(0x04)) edge_list* edges;
        };

        struct mesh_entry
        {
            vector3 bbox_min;
            vector3 bbox_max;
            std::uint8_t diffuse_texture_index;
            std::uint8_t normal_texture_index;
            std::uint8_t height_texture_index;
            std::uint8_t specular_texture_index;
            std::uint8_t opacity_texture_index;
            std::uint8_t light_material_table_index;
            std::uint16_t padding;
            std::uint32_t blending_matrix_indices[4];
            shader_type type;
            class effect* effect;
            std::uint32_t flags;
            std::uint32_t material_key;
            std::uint32_t vertex_count;
            char* chunk_data; // #TODO
            std::uint32_t chunk_data_size;
            std::uint32_t unknown_4;
            std::uint32_t unknown_5;
            std::uint32_t unknown_6;
            std::uint32_t unknown_7;
            std::uint32_t unknown_8;
            std::uint32_t primitive_count;
            std::uint32_t triangle_count;
            std::uint32_t index_start;
            void* file_vertex_buffer;
            std::uint32_t file_vertex_buffer_size;
            ::IDirect3DVertexBuffer9* d3d_vertex_buffer;
            std::uint32_t vertex_buffer_vertex_count;
            std::uint32_t index_count;
            const char* vlt_material_name;
            std::uint32_t overriden_material_key;
            void* vertex_buffer_data;
            std::uint32_t vertex_offset;
        };

        struct platform_info : public linked_node<platform_info>
        {
            std::uint16_t version;
            std::uint8_t chunks_loaded_count;
            std::uint8_t padding;
            std::uint32_t mesh_flags;
            std::uint32_t submesh_count;
            mesh_entry* mesh_entry_table;
            std::uint32_t vertex_buffer_count;
            unsigned int* unknown_0;
            std::uint32_t are_chunks_loaded;
            std::uint16_t* file_index_buffer;
            ::IDirect3DIndexBuffer9* d3d_index_buffer;
            std::uint32_t polygon_count;
            std::uint32_t vertex_count;
        };

        struct platform_interface
        {
            platform_info* pinfo;
        };

        struct solid : public platform_interface, public linked_node<solid>
        {
            enum class flags : std::uint16_t
            {
                compressed_verts = 0x1,
                shadow_map = 0x8,
                vertex_animation = 0x10,
                randomize_start_frame = 0x20,
                is_lit = 0x40,
                is_windy = 0x80,
                duplicate_name = 0x100,
                duplicate_name_error = 0x200,
                duplicated = 0x400,
                want_spotlight_context = 0x800,
                morph_initialized = 0x1000,
                skin_info_created = 0x2000,
                pixel_damage_cleared = 0x4000,
            };

            std::uint8_t version;
            bool endian_swapped;
            flags flag;
            std::uint32_t key;
            std::uint16_t poly_count;
            std::uint16_t vert_count;
            std::uint8_t bone_count;
            std::uint8_t texture_count;
            std::uint8_t light_material_count;
            std::uint8_t position_marker_count;
            std::int32_t referenced_frame_counter;
            vector3 bbox_min;
            texture_entry* textures;
            vector3 bbox_max;
            light_material_entry* light_materials;
            matrix4x4 pivot_matrix;
            position_marker* position_markers;
            normal_smoother* normal_smoother;
            linked_list<model> model_list;
            morph_target* morph_target_list;
            selection_set* selection_set_list;
            float volume;
            float density;
            char name[0x40];
        };

        struct index_entry
        {
            std::uint32_t key;
            solid* solid;
        };

        struct list_header : public linked_node<list_header>
        {
        public:
            std::uint32_t version;
            std::uint32_t solid_count;
            std::uint8_t filename[0x38];
            std::uint8_t group_name[0x20];
            std::uint32_t perm_chunk_byte_offset;
            std::uint32_t perm_chunk_byte_size;
            std::uint16_t max_solid_chunk_byte_alignment;
            bool endian_swapped;
            __declspec(align(0x04)) index_entry* solid_index_entry_table;
            struct streaming_entry* solid_stream_entry_table;
            std::uint16_t texture_pack_count;
            std::uint16_t default_texture_count;
            linked_list<texture::pack> texture_pack_list;
            linked_list<texture::pack> default_texture_list;

        public:
            static inline linked_list<list_header>& list = *reinterpret_cast<linked_list<list_header>*>(0x00A9017C);
        };

        struct replacement_texture_table
        {
            std::uint32_t old_key;
            std::uint32_t new_key;
            texture::info* texture;
        };

        struct model : public linked_node<model>
        {
        public:
            model();

            model(std::uint32_t key);

            ~model();

            void init(std::uint32_t hash_key);

            void uninit();

            void connect(solid* solid_to_connect);

        public:
            std::uint32_t key;
            solid* solid;
            replacement_texture_table* replacement_textures;
            std::uint16_t replacement_texture_count;
            std::int16_t lod_level;

        public:
            static inline instance_pool<model>*& pool = *reinterpret_cast<instance_pool<model>**>(0x00A8FF68);

            static inline linked_list<model>& unattached_list = *reinterpret_cast<linked_list<model>*>(0x00A901A0);
        };

        struct hierarchy
        {
            struct map : eastl::map<std::uint32_t, hierarchy*>
            {
                static inline map& instance = *reinterpret_cast<map*>(0x00B70E5C);
            };

            enum class flags : std::uint8_t
            {
                internal = 0x01,
                endian_swapped = 0x02,
            };

            struct node
            {
                std::uint32_t model_key;
                std::uint32_t solid_key;
                model* model;
                flags flag;
                std::int8_t parent;
                std::int8_t children_count;
                std::int8_t child_index;
            };

            std::uint32_t key;
            std::uint8_t node_count;
            flags flag;
            __declspec(align(0x04)) node nodes[1];
        };

        struct pca_channel_info
        {
            std::uint8_t type;
            std::uint8_t weight_count;
            std::uint16_t vector_buffer_offset;
            std::uint16_t weight_offset;
            std::uint32_t param_handle;
        };

        struct ucap_pca_frame_weights
        {
            std::uint16_t frame_count;
            std::uint16_t weights_per_frame_count;
            std::uint16_t sample_count;
            std::uint16_t channel_count;
            pca_channel_info channel_infos[9];
            std::uint32_t feature_heights_param_handle;
            float* feature_heights;
            float* min_values;
            float* ranges;
            float* vector_buffer;
            std::uint16_t* short_weights;
        };

        struct pca_weights
        {
            std::uint32_t key;
            std::uint16_t frame_count;
            std::uint16_t weights_per_frame_count;
            std::uint16_t sample_count;
            std::uint16_t channel_count;
            pca_channel_info channel_infos[9];
            float* mean;
            float* frames;
        };

        struct pca_blend_data
        {
            std::uint16_t curr_frame;
            std::uint16_t next_frame;
            float blend;
            ucap_pca_frame_weights* ucap_weights;
            pca_weights* weights;
        };

    public:
        static auto find_solid(std::uint32_t key) -> solid*;

        static auto find_solid(std::uint32_t key, list_header* header) -> solid*;

    public:
        static inline float& total_find_time = *reinterpret_cast<float*>(0x00A8FFA8);

        static inline loader::table& loaded_table = *reinterpret_cast<loader::table*>(0x00A901A8);
    };

    CREATE_ENUM_FLAG_OPERATORS(geometry::solid::flags);
    CREATE_ENUM_FLAG_OPERATORS(geometry::hierarchy::flags);

    ASSERT_SIZE(light_material::instance, 0xEC);
    ASSERT_SIZE(geometry::texture_entry, 0x08);
    ASSERT_SIZE(geometry::light_material_entry, 0x08);
    ASSERT_SIZE(geometry::position_marker, 0x50);
    ASSERT_SIZE(geometry::smooth_vertex, 0x08);
    ASSERT_SIZE(geometry::smooth_vertex_info, 0x0C);
    ASSERT_SIZE(geometry::normal_smoother, 0x0C);
    ASSERT_SIZE(geometry::morph_target, 0x0C);
    ASSERT_SIZE(geometry::edge, 0x0A);
    ASSERT_SIZE(geometry::edge_list, 0x08);
    ASSERT_SIZE(geometry::selection_set, 0x10);
    ASSERT_SIZE(geometry::mesh_entry, 0x90);
    ASSERT_SIZE(geometry::platform_info, 0x34);
    ASSERT_SIZE(geometry::solid, 0xE0);
    ASSERT_SIZE(geometry::list_header, 0x90);
    ASSERT_SIZE(geometry::model, 0x18);
    ASSERT_SIZE(geometry::hierarchy, 0x08 + sizeof(geometry::hierarchy::nodes));
    ASSERT_SIZE(geometry::hierarchy::map, 0x1C);
    ASSERT_SIZE(geometry::hierarchy::node, 0x10);
    ASSERT_SIZE(geometry::pca_channel_info, 0x0C);
    ASSERT_SIZE(geometry::ucap_pca_frame_weights, 0x8C);
    ASSERT_SIZE(geometry::pca_weights, 0x80);
    ASSERT_SIZE(geometry::pca_blend_data, 0x10);
}
