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
    };
}
