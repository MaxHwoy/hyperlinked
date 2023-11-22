#include <d3dx9.h>
#include <d3dtypes.h>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/renderer.hpp>

namespace hyper
{
    auto directx::get_mag_filter() -> ::D3DTEXTUREFILTERTYPE
    {
        if (directx::texture_filtering == 2u && (directx::device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) != 0 && directx::adapter.VendorId != 4139u)
        {
            return ::D3DTEXF_ANISOTROPIC;
        }
        else
        {
            return ::D3DTEXF_LINEAR;
        }
    }

    auto directx::get_min_filter() -> ::D3DTEXTUREFILTERTYPE
    {
        if (directx::texture_filtering == 2u && (directx::device_caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) != 0 && directx::adapter.VendorId != 4139u)
        {
            return ::D3DTEXF_ANISOTROPIC;
        }
        else
        {
            return ::D3DTEXF_LINEAR;
        }
    }

    auto directx::get_mip_filter() -> ::D3DTEXTUREFILTERTYPE
    {
        if (directx::texture_filtering != 0u)
        {
            return ::D3DTEXF_LINEAR;
        }
        else
        {
            return ::D3DTEXF_POINT;
        }
    }

    auto directx::get_max_anisotropy() -> ::DWORD
    {
        if (directx::texture_filtering == 2u)
        {
            return directx::device_caps.MaxAnisotropy;
        }
        else
        {
            return 1u;
        }
    }

    auto directx::reset_parameters() -> ::HRESULT
    {
        return directx::device()->Reset(&directx::present);
    }

    void directx::create_query(::D3DQUERYTYPE type)
    {
        directx::device()->CreateQuery(type, &directx::query_);
    }

    void directx::release_query()
    {
        if (directx::query_ != nullptr)
        {
            directx::query_->Release();

            directx::query_ = nullptr;
        }
    }

    void directx::recalculate_pixel_ratio(bool use_device_full_screen)
    {
        HDC dc = ::GetDC(directx::window);

        std::uint32_t pixel_x = directx::resolution_x;
        std::uint32_t pixel_y = directx::resolution_y;

        std::uint32_t& screen_x = directx::screen_size_x;
        std::uint32_t& screen_y = directx::screen_size_y;

        if (screen_x == 0u || screen_y == 0u)
        {
            screen_x = static_cast<std::uint32_t>(::GetDeviceCaps(dc, HORZSIZE));
            screen_y = static_cast<std::uint32_t>(::GetDeviceCaps(dc, VERTSIZE));
        }

        if (use_device_full_screen)
        {
            pixel_x = static_cast<std::uint32_t>(::GetDeviceCaps(dc, HORZRES));
            pixel_y = static_cast<std::uint32_t>(::GetDeviceCaps(dc, VERTRES));
        }

        ::ReleaseDC(directx::window, dc);

        if (screen_x != 0u && screen_y != 0u && pixel_x != 0u && pixel_y != 0u)
        {
            directx::pixel_aspect_ratio = (static_cast<float>(screen_x) * static_cast<float>(pixel_y)) / (static_cast<float>(screen_y) * static_cast<float>(pixel_x));
        }
        else
        {
            directx::pixel_aspect_ratio = 1.0f;
        }
    }

    void directx::create_d3d_present_params(std::uint32_t res_x, std::uint32_t res_y)
    {
        ::memset(&directx::present, 0, sizeof(directx::present));

        if (directx::is_windowed)
        {
            ::GetWindowRect(directx::window, &directx::window_rect);
            ::GetClientRect(directx::window, &directx::client_rect);

            directx::present.FullScreen_RefreshRateInHz = 0u;
        }

        directx::present.Windowed = directx::is_windowed;
        directx::present.BackBufferWidth = res_x;
        directx::present.BackBufferHeight = res_y;

        if ((directx::device_caps.Caps3 & D3DCAPS3_ALPHA_FULLSCREEN_FLIP_OR_DISCARD) != 0 || directx::dword_00AB0AEC || directx::antialias_level > 0u)
        {
            directx::present.SwapEffect = ::D3DSWAPEFFECT_DISCARD;
        }
        else
        {
            directx::present.SwapEffect = ::D3DSWAPEFFECT_COPY;
        }

        directx::present.AutoDepthStencilFormat = directx::use_16bit_depth ? ::D3DFMT_D16 : ::D3DFMT_D24S8;
        directx::present.PresentationInterval = directx::vsync_on ? 1u : 0x80000000u;
        directx::present.hDeviceWindow = directx::window;
        directx::present.BackBufferFormat = ::D3DFMT_X8R8G8B8;
        directx::present.EnableAutoDepthStencil = TRUE;
        directx::present.Flags = 0u;

        directx::resolution_x = res_x;
        directx::resolution_y = res_y;

        if (directx::antialias_level > 0u)
        {
            directx::present.MultiSampleType = ::D3DMULTISAMPLE_NONMASKABLE;
            directx::present.MultiSampleQuality = directx::antialias_level;
        }
        else
        {
            directx::present.MultiSampleType = ::D3DMULTISAMPLE_NONE;
            directx::present.MultiSampleQuality = 0u;
        }

        directx::recalculate_pixel_ratio(false);
    }

    void directx::reset_rendering_states()
    {
        ::IDirect3DDevice9* xdevice = directx::device();

        ::D3DCAPS9& caps = directx::device_caps;

        bool vendor_not_4139 = directx::adapter.VendorId != 4139u;

        xdevice->SetRenderState(::D3DRS_ZENABLE, ::D3DZB_TRUE);
        xdevice->SetRenderState(::D3DRS_ZWRITEENABLE, TRUE);
        xdevice->SetRenderState(::D3DRS_ZFUNC, ::D3DCMP_LESSEQUAL);
        xdevice->SetRenderState(::D3DRS_ALPHAFUNC, ::D3DCMP_GREATER);
        xdevice->SetRenderState(::D3DRS_SPECULARENABLE, TRUE);
        xdevice->SetRenderState(::D3DRS_LIGHTING, FALSE);
        xdevice->SetRenderState(::D3DRS_FOGENABLE, FALSE);
        xdevice->SetRenderState(::D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RGB);

        renderer::current_cull_mode = ::D3DCULL_CW;

        ::D3DTEXTUREFILTERTYPE min_filter = ::D3DTEXF_LINEAR;
        ::D3DTEXTUREFILTERTYPE mag_filter = ::D3DTEXF_LINEAR;
        ::D3DTEXTUREFILTERTYPE mip_filter = ::D3DTEXF_POINT;
        ::DWORD max_anisotropy = 1u;

        if (directx::texture_filtering != 0u)
        {
            if (directx::texture_filtering == 2u)
            {
                max_anisotropy = caps.MaxAnisotropy;

                if (vendor_not_4139)
                {
                    if ((caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) != 0)
                    {
                        min_filter = ::D3DTEXF_ANISOTROPIC;
                    }

                    if ((caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) != 0)
                    {
                        mag_filter = ::D3DTEXF_ANISOTROPIC;
                    }
                }
            }

            mip_filter = ::D3DTEXF_LINEAR;
        }

        for (::DWORD i = 0u; i < caps.MaxTextureBlendStages; ++i)
        {
            xdevice->SetSamplerState(i, ::D3DSAMP_MINFILTER, min_filter);
            xdevice->SetSamplerState(i, ::D3DSAMP_MAGFILTER, mag_filter);
            xdevice->SetSamplerState(i, ::D3DSAMP_MIPFILTER, mip_filter);
            xdevice->SetSamplerState(i, ::D3DSAMP_MAXANISOTROPY, max_anisotropy);
        }

        effect_world::instance->set_int(effect::parameter_type::BaseMagTextureFilter, static_cast<std::int32_t>(mag_filter));
        effect_world::instance->set_int(effect::parameter_type::BaseMinTextureFilter, static_cast<std::int32_t>(min_filter));
        effect_world::instance->set_int(effect::parameter_type::BaseMipTextureFilter, static_cast<std::int32_t>(mip_filter));
        effect_world::instance->set_int(effect::parameter_type::BaseTextureFilterParam, static_cast<std::int32_t>(caps.MaxAnisotropy));

        if (directx::allow_antialias && directx::antialias_level > 0)
        {
            xdevice->SetRenderState(::D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        }
        else
        {
            xdevice->SetRenderState(::D3DRS_MULTISAMPLEANTIALIAS, FALSE);
        }

        for (::DWORD i = 0u; i < caps.MaxTextureBlendStages; ++i)
        {
            xdevice->SetTextureStageState(i, ::D3DTSS_COLOROP, ::D3DTOP_DISABLE);
            xdevice->SetTextureStageState(i, ::D3DTSS_ALPHAOP, ::D3DTOP_DISABLE);
            xdevice->SetTextureStageState(i, ::D3DTSS_TEXTURETRANSFORMFLAGS, ::D3DTTFF_DISABLE);
            xdevice->SetTextureStageState(i, ::D3DTSS_TEXCOORDINDEX, i);
        }

        effect_world::instance->set_int(effect::parameter_type::ColorWriteMode, D3DCOLORWRITEENABLE_RGB);

        directx::last_textures_used.clear();
    }

    void directx::set_alpha_render_state(bool enabled, ::DWORD alpha_ref, ::D3DCMPFUNC alpha_func)
    {
        directx::device()->SetRenderState(::D3DRS_ALPHATESTENABLE, enabled);

        if (enabled)
        {
            directx::device()->SetRenderState(::D3DRS_ALPHAREF, alpha_ref);
            directx::device()->SetRenderState(::D3DRS_ALPHAFUNC, alpha_func);
        }
    }
}
