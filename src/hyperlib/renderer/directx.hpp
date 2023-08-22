#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class directx final
    {
    public:
        static inline auto device() -> IDirect3DDevice9*
        {
            return *reinterpret_cast<IDirect3DDevice9**>(0x00AB0ABC);
        }

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00AB0AC8);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00AB0ACC);

        static inline D3DCAPS9& device_caps = *reinterpret_cast<D3DCAPS9*>(0x00AB0348);
    };
}
