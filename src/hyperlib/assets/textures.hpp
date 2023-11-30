#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/pca.hpp>
#include <hyperlib/assets/loader.hpp>

namespace hyper
{
    class texture final
    {
    public:
        struct info;

        struct render_state
        {
            void initialize(info* texture);

            operator std::uint32_t();

            /* 0b00000001 */ std::uint32_t z_write_enabled : 1;
            /* 0b00000002 */ std::uint32_t is_backface_culled : 1;
            /* 0b00000004 */ std::uint32_t alpha_test_enabled : 1;
            /* 0b00000008 */ std::uint32_t alpha_blend_enabled : 1;
            /* 0b00000010 */ std::uint32_t alpha_blend_src : 4;
            /* 0b00000100 */ std::uint32_t alpha_blend_dest : 4;
            /* 0b00001000 */ std::uint32_t texture_address_u : 2;
            /* 0b00004000 */ std::uint32_t texture_address_v : 2;
            /* 0b00010000 */ std::uint32_t has_texture_animation : 1;
            /* 0b00020000 */ std::uint32_t is_additive_blend : 1;
            /* 0b00040000 */ std::uint32_t wants_auxiliary_textures : 1;
            /* 0b00080000 */ std::uint32_t bias_level : 2;
            /* 0b00200000 */ std::uint32_t multi_pass_blend : 1;
            /* 0b00400000 */ std::uint32_t colour_write_alpha : 1;
            /* 0b00800000 */ std::uint32_t sub_sort_key : 1;
            /* 0b01000000 */ std::uint32_t alpha_test_ref : 4;
            /* 0b10000000 */ std::uint32_t padding : 4;
        };

        enum class compression_type : std::uint8_t
        {
            default_ = 0,
            p4 = 4,
            p8 = 8,
            p16 = 16,
            p16_1555 = 17,
            p16_565 = 18,
            p16_3555 = 19,
            rgb24 = 24,
            rgba32 = 32,
            dxt = 33,
            dxt1 = 34,
            dxt3 = 36,
            dxt5 = 38,
            ati2 = 39, // ATI2
            l8 = 40,
            dxt1_air = 41,
            dxt1_aig = 42,
            dxt1_aib = 43,
            ati1 = 44, // ATI1
            p8_16 = 128,
            p8_64 = 129,
            p8_ia8 = 130,
            p4_ia8 = 131,
            p4_rgb24_a8 = 140,
            p8_rgb24_a8 = 141,
            p4_rgb16_a8 = 142,
            p8_rgb16_a8 = 143,
            invalid = 255,
        };

        enum class lock_type : std::uint8_t
        {
            read = 0x0,
            write = 0x1,
            read_write = 0x2,
        };

        enum class tileable_type : std::uint8_t
        {
            clamp = 0x00,
            u_repeat = 0x01,
            v_repeat = 0x02,
            u_mirror = 0x04,
            v_mirror = 0x08,
        };

        enum class scroll_type : std::uint8_t
        {
            none = 0x00,
            smooth = 0x01,
            snap = 0x02,
            offset_scale = 0x03,
        };

        enum class alpha_blend_type : std::uint8_t
        {
            src_copy = 0,
            blend = 1,
            additive = 2,
            subtractive = 3,
            overbright = 4,
            dest_blend = 5,
            dest_additive = 6,
            dest_subtractive = 7,
            dest_overbright = 8,
        };

        enum class alpha_usage_type : std::uint8_t
        {
            none = 0,
            punchthru = 1,
            modulated = 2,
        };

        enum class bit_flags : std::uint8_t
        {
            unknown = 0x01,
            disable_culling = 0x02,
        };

        struct page_range
        {
            float u0;
            float u1;
            float v0;
            float v1;
            std::uint32_t flags;
            std::uint32_t key;
            std::uint32_t pad1;
            std::uint32_t pad2;
        };

        struct platform_info : public linked_node<platform_info>
        {
            render_state state;
            std::uint32_t type;
            ::IDirect3DTexture9* texture;
            std::uint16_t punchthru_value;
            std::uint16_t format;
        };

        struct platform_interface
        {
            platform_info* pinfo;
        };

        struct info : public platform_interface, public linked_node<info>
        {
            std::uint32_t key;
            std::uint32_t class_key;
            std::int32_t image_placement;
            std::int32_t palette_placement;
            std::int32_t image_size;
            std::int32_t palette_size;
            std::int32_t base_image_size;
            std::uint16_t width;
            std::uint16_t height;
            std::uint8_t shift_width;
            std::uint8_t shift_height;
            compression_type image_compression_type;
            std::uint8_t palette_compression_type;
            std::uint16_t num_palette_entries;
            std::uint8_t num_mipmap_levels;
            tileable_type tilable_uv;
            std::uint8_t bias_level;
            std::uint8_t rendering_order;
            scroll_type scroll;
            std::uint8_t used_flag;
            std::uint8_t apply_alpha_sorting;
            alpha_usage_type usage_type;
            alpha_blend_type blend_type;
            bit_flags flags;
            std::int16_t scroll_time_step;
            std::int16_t scroll_speed_s;
            std::int16_t scroll_speed_t;
            std::int16_t offset_s;
            std::int16_t offset_t;
            std::int16_t scale_s;
            std::int16_t scale_t;
            std::int16_t unknown;
            struct pack* pack;
            void* image_data;
            pca::weights* weights;
            std::uint8_t name_len;
            std::uint8_t name[35];
        };

        struct index_entry
        {
            std::uint32_t key;
            info* texture;
        };

        struct e_texture : public linked_node<e_texture>
        {
            std::uint32_t key;
            info* texture;
        };

        struct pack_header
        {
            std::int32_t version;
            char name[28];
            char filename[64];
            std::uint32_t key;
            std::uint32_t perm_chunk_byte_offset;
            std::uint32_t perm_chunk_byte_size;
            bool endian_swapped;
            __declspec(align(0x04)) struct pack* pack;
            index_entry* texture_index_entry_table;
            struct e_streaming_entry* texture_stream_entry_table;
        };

        struct vram_data_header : public linked_node<vram_data_header>
        {
            std::uint32_t version;
            std::uint32_t key;
            bool endian_swapped;
            chunk* vram_data_chunk;
        };

        struct pack : public linked_node<pack>
        {
            const char* name;
            const char* filename;
            std::uint32_t key;
            pack_header* header;
            std::uint32_t textures_count;
            std::uint32_t texture_data_size;
            linked_list<e_texture> e_texture_list;
        };

    public:
        static auto get_texture_info(std::uint32_t key, bool default_if_not_found, bool include_unloaded) -> info*;

        static auto get_texture_page_range(std::uint32_t key, std::int32_t bucket) -> page_range*;

        static void set_e_texture_key(e_texture& texture, std::uint32_t key);

        static auto get_scroll_s(const info& info, float delta_time) -> float;

        static auto get_scroll_t(const info& info, float delta_time) -> float;

        static auto get_scroll(float delta_time, float speed, scroll_type scroll, float step) -> float;

    public:
        static inline std::uint32_t& mipmap_strip_count = *reinterpret_cast<std::uint32_t*>(0x00AB09CC);

        static inline info*& default_texture = *reinterpret_cast<info**>(0x00A8FFB0);

        static inline info*& headlights_texture = *reinterpret_cast<info**>(0x00B43040);

        static inline info*& headlights_clip_texture = *reinterpret_cast<info**>(0x00B43038);

        static inline info*& headlights_xenon_texture = *reinterpret_cast<info**>(0x00B4303C);

        static inline info*& rvm_texture = *reinterpret_cast<info**>(0x00AB0A44);

        static inline info*& rvm_mask_texture = *reinterpret_cast<info**>(0x00AB0A48);

        static inline info*& pip_mask_texture = *reinterpret_cast<info**>(0x00AB0A50);

        static inline info*& white_16x16_no_alpha_texture = *reinterpret_cast<info**>(0x00AB0A4C);

        static inline array<info*, 5u> ghost_car_textures = array<info*, 5u>(0x00AB08C0);

        static inline array<::IDirect3DCubeTexture9*, 5u> refl_cube_textures = array<::IDirect3DCubeTexture9*, 5u>(0x00AB09D0);

        static inline array<std::uint32_t, 2u> refl_cube_world_indices = array<std::uint32_t, 2u>(0x00B42EA8);

        static inline std::uint32_t& refl_cube_texture_count = *reinterpret_cast<std::uint32_t*>(0x00AB09E4);

        static inline loader::table& loaded_table = *reinterpret_cast<loader::table*>(0x00A921B0);
    };

    CREATE_ENUM_FLAG_OPERATORS(texture::tileable_type);
    CREATE_ENUM_FLAG_OPERATORS(texture::bit_flags);

    ASSERT_SIZE(texture::render_state, sizeof(std::uint32_t));
    ASSERT_SIZE(texture::page_range, 0x20);
    ASSERT_SIZE(texture::info, 0x7C);
    ASSERT_SIZE(texture::index_entry, 0x08);
    ASSERT_SIZE(texture::e_texture, 0x10);
    ASSERT_SIZE(texture::page_range, 0x20);
    ASSERT_SIZE(texture::pack_header, 0x7C);
    ASSERT_SIZE(texture::vram_data_header, 0x18);
    ASSERT_SIZE(texture::pack, 0x28);
}
