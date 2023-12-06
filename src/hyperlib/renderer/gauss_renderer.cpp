#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/gauss_renderer.hpp>

namespace hyper
{
    gauss_renderer::gauss_renderer()
    {
        if (this->texture_ == nullptr)
        {
            std::uint32_t x = math::floor_pow_2(directx::resolution_x >> 1);
            std::uint32_t y = math::floor_pow_2(directx::resolution_y >> 1);

            directx::device()->CreateTexture(x, y, 2u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->texture_, nullptr);
        }
    }

    gauss_renderer::~gauss_renderer()
    {
        if (this->texture_ != nullptr)
        {
            this->texture_->Release();

            this->texture_ = nullptr;
        }
    }

    void gauss_renderer::ctor(gauss_renderer& renderer)
    {
        new (&renderer) gauss_renderer();
    }

    void gauss_renderer::dtor(gauss_renderer& renderer)
    {
        renderer.~gauss_renderer();
    }
}
