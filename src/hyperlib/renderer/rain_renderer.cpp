#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/rain_renderer.hpp>

namespace hyper
{
    rain_renderer::rain_renderer(std::uint32_t vertex_format, std::uint32_t max_polies)
    {
        this->vertex_size_ = sizeof(rain_renderer::poly::vertices[0]);

        for (size_t i = 0u; i < std::size(this->vertex_buffer_); ++i)
        {
            this->vertex_buffer_[i] = nullptr;
            this->unknown_[i] = 0u;
            this->locked_[i] = false;
            this->max_vertices_[i] = max_polies << 2;
            this->min_vertex_index_[i] = 0u;
            this->polies_[i] = nullptr;
            this->curr_vert_offset_[i] = 0u;

            directx::device()->CreateVertexBuffer(this->vertex_size_ * (this->max_vertices_[i] + 10u), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, vertex_format, ::D3DPOOL_DEFAULT, &this->vertex_buffer_[i], nullptr);
        }

        directx::device()->CreateIndexBuffer(12u * max_polies, D3DUSAGE_WRITEONLY, ::D3DFMT_INDEX16, ::D3DPOOL_MANAGED, &this->index_buffer_, nullptr);

        std::uint16_t* indexer;

        if (SUCCEEDED(this->index_buffer_->Lock(0u, 0u, reinterpret_cast<void**>(&indexer), 0u)))
        {
            for (std::uint16_t i = 0u, k = 0u; i < max_polies; ++i, k += 4u)
            {
                *indexer++ = k + 3u;
                *indexer++ = k + 0u;
                *indexer++ = k + 1u;
                *indexer++ = k + 1u;
                *indexer++ = k + 2u;
                *indexer++ = k + 3u;
            }

            this->index_buffer_->Unlock();
        }
    }

    rain_renderer::~rain_renderer()
    {
        for (size_t i = 0u; i < std::size(this->vertex_buffer_); ++i)
        {
            if (this->polies_[i] != nullptr)
            {
                this->vertex_buffer_[i]->Unlock();

                this->polies_[i] = nullptr;

                this->locked_[i] = false; // #TODO ?
            }

            if (this->vertex_buffer_[i] != nullptr)
            {
                this->vertex_buffer_[i]->Release();

                this->vertex_buffer_[i] = nullptr;
            }
        }

        if (this->index_buffer_ != nullptr)
        {
            this->index_buffer_->Release();

            this->index_buffer_ = nullptr;
        }
    }

    bool rain_renderer::render(::IDirect3DSurface9* surface_dst, ::IDirect3DTexture9* texture_src)
    {
        return call_function<bool(__cdecl*)(::IDirect3DSurface9*, ::IDirect3DTexture9*)>(0x00722CB0)(surface_dst, texture_src);
    }

    void rain_renderer::ctor(rain_renderer& renderer)
    {
        new (&renderer) rain_renderer(0u, rain_renderer::max_rain_polies_);
    }

    void rain_renderer::dtor(rain_renderer& renderer)
    {
        renderer.~rain_renderer();
    }
}
