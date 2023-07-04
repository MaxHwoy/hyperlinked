#include <hyperlib/assets/textures.hpp>

#include <hyperlib/renderer/directx.hpp>

#define CHECK_BARRIER_STRINGS

namespace hyper
{
    void render_state::initialize(texture_info* texture)
    {
        *reinterpret_cast<std::uint32_t*>(this) = 0u; // hard reset (idk why blackbox doesn't do it btw)

        std::uint32_t class_key = texture->class_key;

        if (class_key != 0xC509D75Cu && class_key != 0x754F4DE0u)
        {
            this->sub_sort_key = true;
        }

        this->alpha_test_enabled = texture->alpha_usage_type == texture_alpha_usage_type::punchthru;

        if (texture->alpha_blend_type > texture_alpha_blend_type::src_copy)
        {
            this->alpha_blend_enabled = true;

            switch (texture->alpha_blend_type)
            {
                case texture_alpha_blend_type::blend:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVSRCALPHA;
                    break;

                case texture_alpha_blend_type::additive:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_ONE;
                    this->is_additive_blend = true;
                    this->colour_write_alpha = true;
                    break;

                case texture_alpha_blend_type::subtractive:
                    this->alpha_blend_src = D3DBLEND_ZERO;
                    this->alpha_blend_dest = D3DBLEND_INVSRCCOLOR;
                    break;

                case texture_alpha_blend_type::overbright:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVSRCALPHA;
                    break;

                case texture_alpha_blend_type::dest_blend:
                    this->alpha_blend_src = D3DBLEND_DESTALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVDESTALPHA;
                    break;

                case texture_alpha_blend_type::dest_additive:
                    this->alpha_blend_src = D3DBLEND_DESTALPHA;
                    this->alpha_blend_dest = D3DBLEND_ONE;
                    break;
            }
        }
        else
        {
            this->z_write_enabled = true;
            this->alpha_blend_src = D3DBLEND_ONE;
            this->alpha_blend_dest = D3DBLEND_ZERO;

            if (!this->alpha_test_enabled && (texture->flags & texture_bit_flags::disable_culling) != texture_bit_flags::disable_culling)
            {
                this->is_backface_culled = true;
            }
        }

        if ((texture->tilable_uv & texture_tileable_type::u_mirror) == texture_tileable_type::u_mirror)
        {
            this->texture_address_u = D3DTADDRESS_MIRROR;
        }
        else
        {
            this->texture_address_u = (texture->tilable_uv & texture_tileable_type::u_repeat) == texture_tileable_type::u_repeat ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
        }

        if ((texture->tilable_uv & texture_tileable_type::v_mirror) == texture_tileable_type::v_mirror)
        {
            this->texture_address_v = D3DTADDRESS_MIRROR;
        }
        else
        {
            this->texture_address_v = (texture->tilable_uv & texture_tileable_type::v_repeat) == texture_tileable_type::v_repeat ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
        }

        this->bias_level = texture->bias_level & 3;
        this->alpha_test_ref = 11u; // #TODO

        if (class_key == hashing::bin("Tree Leaves") || class_key == hashing::bin("Tree Cards") || class_key == hashing::bin("MultiPass Blend"))
        {
            this->multi_pass_blend = true;
            texture->apply_alpha_sorting = 2;
        }

        bool is_barrier = class_key == hashing::bin("Barrier Mask")
#if defined(CHECK_BARRIER_STRINGS)
            || ::strncmp(reinterpret_cast<const char*>(texture->name), "SFX_TRACKBARRIER", 16u)
#endif
            ;

        if (is_barrier)
        {
            this->colour_write_alpha = false;
        }
        else
        {
            if (class_key == hashing::bin("Bloom Alpha Mask"))
            {
                this->colour_write_alpha = true;
            }
        }

        this->has_texture_animation = texture->scroll_type != texture_scroll_type::none;
    }
}
