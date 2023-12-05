#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>

#define D3DCOLORWRITEENABLE_RGB D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE
#define D3DCOLORWRITEENABLE_ALL D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA

namespace hyper
{
    class directx final
    {
    public:
        static inline auto device() -> IDirect3DDevice9*
        {
            return *reinterpret_cast<::IDirect3DDevice9**>(0x00AB0ABC);
        }

        static inline auto query() -> ::IDirect3DQuery9*
        {
            return directx::query_;
        }

        static auto get_mag_filter() -> ::D3DTEXTUREFILTERTYPE;

        static auto get_min_filter() -> ::D3DTEXTUREFILTERTYPE;

        static auto get_mip_filter() -> ::D3DTEXTUREFILTERTYPE;

        static auto get_max_anisotropy() -> ::DWORD;

        static auto reset_parameters() -> ::HRESULT;

        static void create_query(::D3DQUERYTYPE type);

        static void release_query();

        static void recalculate_pixel_ratio(bool use_device_full_screen);

        static void create_d3d_present_params(std::uint32_t res_x, std::uint32_t res_y);

        static void reset_rendering_states();

        static void set_alpha_render_state(bool enabled, ::DWORD alpha_ref, ::D3DCMPFUNC alpha_func);

        static void set_z_write_enable(bool z_write_enabled, bool z_enabled);

        static void fill_with_color(::IDirect3DTexture9* texture, unsigned char color);

    private:
        static inline ::IDirect3DQuery9*& query_ = *reinterpret_cast<::IDirect3DQuery9**>(0x00AB0AC4);

    public:
        static inline float pixel_aspect_ratio = 1.0f;

        static inline std::uint32_t screen_size_x = 0u;

        static inline std::uint32_t screen_size_y = 0u;

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00AB0AC8);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00AB0ACC);

        static inline bool& is_windowed = *reinterpret_cast<bool*>(0x00AB0AD4);

        static inline ::HWND& window = *reinterpret_cast<::HWND*>(0x00AB0AD8);

        static inline ::tagRECT& window_rect = *reinterpret_cast<::tagRECT*>(0x00AB04C0);

        static inline ::tagRECT& client_rect = *reinterpret_cast<::tagRECT*>(0x00AAF540);

        static inline ::_D3DPRESENT_PARAMETERS_& present = *reinterpret_cast<::_D3DPRESENT_PARAMETERS_*>(0x00AB06FC);

        static inline ::D3DADAPTER_IDENTIFIER9& adapter = *reinterpret_cast<::D3DADAPTER_IDENTIFIER9*>(0x00AAFEB0);

        static inline ::D3DCAPS9& device_caps = *reinterpret_cast<::D3DCAPS9*>(0x00AB0348);

        static inline bool& use_16bit_depth = *reinterpret_cast<bool*>(0x00AB0A54);

        static inline array<texture::info*, 5u> last_textures_used = array<texture::info*, 5u>(0x00AB0BC8);

        static inline std::uint32_t& dword_00AB0AEC = *reinterpret_cast<std::uint32_t*>(0x00AB0AEC);
    };
}
