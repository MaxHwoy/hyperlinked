#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/blur_renderer.hpp>

namespace hyper
{
    blur_renderer::blur_renderer()
    {
        for (std::uint32_t i = 0u; i < blur_renderer::downscale_count; ++i)
        {
            std::uint32_t x = directx::resolution_x >> (1 + i);
            std::uint32_t y = directx::resolution_y >> (1 + i);

            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_1_[i], nullptr);
            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_2_[i], nullptr);
            directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_3_[i], nullptr);

            this->textures_1_[i]->GetSurfaceLevel(0u, &this->surfaces_1_[i]);
            this->textures_2_[i]->GetSurfaceLevel(0u, &this->surfaces_2_[i]);
            this->textures_3_[i]->GetSurfaceLevel(0u, &this->surfaces_3_[i]);
        }
    }

    blur_renderer::~blur_renderer()
    {
        for (std::uint32_t i = 0u; i < blur_renderer::downscale_count; ++i)
        {
            if (this->surfaces_1_[i] != nullptr)
            {
                this->surfaces_1_[i]->Release();

                this->surfaces_1_[i] = nullptr;
            }

            if (this->surfaces_2_[i] != nullptr)
            {
                this->surfaces_2_[i]->Release();

                this->surfaces_2_[i] = nullptr;
            }

            if (this->surfaces_3_[i] != nullptr)
            {
                this->surfaces_3_[i]->Release();

                this->surfaces_3_[i] = nullptr;
            }

            if (this->textures_1_[i] != nullptr)
            {
                this->textures_1_[i]->Release();

                this->textures_1_[i] = nullptr;
            }

            if (this->textures_2_[i] != nullptr)
            {
                this->textures_2_[i]->Release();

                this->textures_2_[i] = nullptr;
            }

            if (this->textures_3_[i] != nullptr)
            {
                this->textures_3_[i]->Release();

                this->textures_3_[i] = nullptr;
            }
        }
    }

    void blur_renderer::ctor(blur_renderer& renderer)
    {
        new (&renderer) blur_renderer();
    }

    void blur_renderer::dtor(blur_renderer& renderer)
    {
        renderer.~blur_renderer();
    }
}
