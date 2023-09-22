#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>

namespace hyper
{
    class directx final
    {
    public:
        static inline auto device() -> IDirect3DDevice9*
        {
            return *reinterpret_cast<::IDirect3DDevice9**>(0x00AB0ABC);
        }

        static void recalculate_pixel_ratio(bool use_device_full_screen);
        
        static void create_d3d_present_params(std::uint32_t res_x, std::uint32_t res_y);

        static void reset_rendering_states();

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

        static inline ::D3DCULL& default_cull = *reinterpret_cast<::D3DCULL*>(0x00A6523C);

        static inline bool& use_16bit_depth = *reinterpret_cast<bool*>(0x00AB0A54);

        static inline std::uint32_t& texture_filtering = *reinterpret_cast<std::uint32_t*>(0x00A65384);

        static inline bool& allow_antialias = *reinterpret_cast<bool*>(0x00A63D2C);

        static inline std::int32_t& antialias_level = *reinterpret_cast<std::int32_t*>(0x00A6537C);

        static inline bool& vsync_on = *reinterpret_cast<bool*>(0x00A65390);

        static inline array<texture::info*, 5u> last_textures_used = array<texture::info*, 5u>(0x00AB0BC8);

        static inline std::uint32_t& dword_00AB0AEC = *reinterpret_cast<std::uint32_t*>(0x00AB0AEC);
    };
}
