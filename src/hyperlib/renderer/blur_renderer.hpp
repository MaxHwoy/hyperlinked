#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class blur_renderer final
    {
    public:
        enum class downscale : std::uint32_t
        {
            d2x2 = 0,
            d4x4 = 1,
            d8x8 = 2,
        };

        const static inline std::uint32_t downscale_count = 3;

    public:
        blur_renderer();

        ~blur_renderer();

    public:
        static void ctor(blur_renderer& renderer);

        static void dtor(blur_renderer& renderer);

    private:
        ::IDirect3DSurface9* surfaces_1_[downscale_count];
        ::IDirect3DTexture9* textures_1_[downscale_count];
        ::IDirect3DTexture9* textures_2_[downscale_count];
        ::IDirect3DTexture9* textures_3_[downscale_count];
        ::IDirect3DSurface9* surfaces_2_[downscale_count];
        ::IDirect3DSurface9* surfaces_3_[downscale_count];

    public:
        static inline blur_renderer& instance = *reinterpret_cast<blur_renderer*>(0x00B42F3C);
    };
}
