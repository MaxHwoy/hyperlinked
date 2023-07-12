#include <hyperlib/assets/textures.hpp>

#include <hyperlib/renderer/directx.hpp>

#define CHECK_BARRIER_STRINGS

namespace hyper
{
    void texture::render_state::initialize(texture::info* texture)
    {
        *reinterpret_cast<std::uint32_t*>(this) = 0u; // hard reset (idk why blackbox doesn't do it btw)

        std::uint32_t class_key = texture->class_key;

        if (class_key != 0xC509D75Cu && class_key != 0x754F4DE0u)
        {
            this->sub_sort_key = true;
        }

        this->alpha_test_enabled = texture->usage_type == texture::alpha_usage_type::punchthru;

        if (texture->blend_type > texture::alpha_blend_type::src_copy)
        {
            this->alpha_blend_enabled = true;

            switch (texture->blend_type)
            {
                case texture::alpha_blend_type::blend:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVSRCALPHA;
                    break;
                
                case texture::alpha_blend_type::additive:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_ONE;
                    this->is_additive_blend = true;
                    this->colour_write_alpha = true;
                    break;
                
                case texture::alpha_blend_type::subtractive:
                    this->alpha_blend_src = D3DBLEND_ZERO;
                    this->alpha_blend_dest = D3DBLEND_INVSRCCOLOR;
                    break;
                
                case texture::alpha_blend_type::overbright:
                    this->alpha_blend_src = D3DBLEND_SRCALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVSRCALPHA;
                    break;
                
                case texture::alpha_blend_type::dest_blend:
                    this->alpha_blend_src = D3DBLEND_DESTALPHA;
                    this->alpha_blend_dest = D3DBLEND_INVDESTALPHA;
                    break;
                
                case texture::alpha_blend_type::dest_additive:
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

            if (!this->alpha_test_enabled && (texture->flags & texture::bit_flags::disable_culling) != texture::bit_flags::disable_culling)
            {
                this->is_backface_culled = true;
            }
        }

        if ((texture->tilable_uv & texture::tileable_type::u_mirror) == texture::tileable_type::u_mirror)
        {
            this->texture_address_u = D3DTADDRESS_MIRROR;
        }
        else
        {
            this->texture_address_u = (texture->tilable_uv & texture::tileable_type::u_repeat) == texture::tileable_type::u_repeat ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
        }

        if ((texture->tilable_uv & texture::tileable_type::v_mirror) == texture::tileable_type::v_mirror)
        {
            this->texture_address_v = D3DTADDRESS_MIRROR;
        }
        else
        {
            this->texture_address_v = (texture->tilable_uv & texture::tileable_type::v_repeat) == texture::tileable_type::v_repeat ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
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

        this->has_texture_animation = texture->scroll != texture::scroll_type::none;
    }

    auto texture::get_texture_info(std::uint32_t key, bool default_if_not_found, bool include_unloaded) -> texture::info*
    {
        return call_function<texture::info*(__cdecl*)(std::uint32_t, bool, bool)>(0x0055CFD0)(key, default_if_not_found, include_unloaded);
    }

    auto texture::get_texture_page_range(std::uint32_t key, std::int32_t bucket) -> texture::page_range*
    {
        return call_function<texture::page_range*(__cdecl*)(std::uint32_t, std::int32_t)>(0x0055A100)(key, bucket);
    }

    void texture::set_e_texture_key(e_texture& texture, std::uint32_t texture_key)
    {
        call_function<void(__thiscall*)(e_texture*, std::uint32_t)>(0x0055DF50)(&texture, texture_key);
    }
}
