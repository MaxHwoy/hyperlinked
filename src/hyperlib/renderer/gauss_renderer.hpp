#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class gauss_renderer final
    {
    public:
        gauss_renderer();

        ~gauss_renderer();

    public:
        static void ctor(gauss_renderer& renderer);

        static void dtor(gauss_renderer& renderer);

    private:
        ::IDirect3DTexture9* texture_;

    public:
        static inline gauss_renderer& instance = *reinterpret_cast<gauss_renderer*>(0x00AB2140);
    };
}
