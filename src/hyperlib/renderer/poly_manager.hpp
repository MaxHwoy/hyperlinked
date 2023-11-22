#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/directx.hpp>

namespace hyper
{
    enum class e_poly_flags : std::uint8_t
    {
        apply_aspect           = 1u << 0,
        apply_z_sort           = 1u << 1,
        multi_text_mas         = 1u << 2,
        no_tex_fix             = 1u << 3,
        use_native_screen_size = 1u << 4,
    };

    struct __declspec(align(0x10)) e_poly
    {
        vector3pad vertices[4];
        float uvs[4][2];
        float uvs_mask[4][2];
        color32 colors[4];
        e_poly_flags flags;
        char flailer;
    };

    template <typename Vertex> class poly_manager
    {
    public:
        struct poly
        {
            Vertex vertices[4];
        };

    public:
        poly_manager(std::uint32_t max_polies);

        ~poly_manager();

        void lock();

        void unlock();

        auto allocate() -> poly*;

    private:
        ::IDirect3DVertexBuffer9* vertex_buffer_;
        ::IDirect3DIndexBuffer9* index_buffer_;
        std::uint32_t vertex_count_;
        std::uint32_t max_polies_;
        std::uint32_t poly_count_;

    protected:
        texture::info* texture_page_;

    private:
        bool _0x18_;
        poly* polies_;
        bool locked_;
    };

    CREATE_ENUM_FLAG_OPERATORS(e_poly_flags);

    ASSERT_SIZE(e_poly, 0xA0);
}

namespace hyper
{
    template <typename Vertex> poly_manager<Vertex>::poly_manager(std::uint32_t max_polies)
    {
        this->max_polies_ = max_polies;
        this->poly_count_ = 0u;
        this->_0x18_ = false;
        this->vertex_count_ = max_polies * 4u;
        this->polies_ = nullptr;
        this->locked_ = false;

        directx::device()->CreateVertexBuffer(sizeof(poly) * max_polies, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0u, ::D3DPOOL_DEFAULT, &this->vertex_buffer_, nullptr);
        directx::device()->CreateIndexBuffer(sizeof(std::uint16_t) * 6u * max_polies, D3DUSAGE_WRITEONLY, ::D3DFMT_INDEX16, ::D3DPOOL_MANAGED, &this->index_buffer_, nullptr);

        std::uint16_t* indexer;

        if (SUCCEEDED(this->index_buffer_->Lock(0u, 0u, reinterpret_cast<void**>(&indexer), 0u)))
        {
            for (std::uint16_t i = 0u, k = 0u; i < max_polies; ++i, k += 4u)
            {
                *indexer++ = k + 0u;
                *indexer++ = k + 1u;
                *indexer++ = k + 2u;
                *indexer++ = k + 0u;
                *indexer++ = k + 2u;
                *indexer++ = k + 3u;
            }

            this->index_buffer_->Unlock();
        }
    }

    template <typename Vertex> poly_manager<Vertex>::~poly_manager()
    {
        if (this->vertex_buffer_ != nullptr)
        {
            this->vertex_buffer_->Release();
            this->vertex_buffer_ = nullptr;
        }

        if (this->index_buffer_ != nullptr)
        {
            this->index_buffer_->Release();
            this->index_buffer_ = nullptr;
        }
    }

    template <typename Vertex> void poly_manager<Vertex>::lock()
    {
        HRESULT result = this->vertex_buffer_->Lock(0u, 0u, reinterpret_cast<void**>(&this->polies_), D3DLOCK_DISCARD);

        ASSERT(SUCCEEDED(result));

        this->poly_count_ = 0u;
        this->_0x18_ = true;
        this->locked_ = true;
    }

    template <typename Vertex> void poly_manager<Vertex>::unlock()
    {
        if (this->_0x18_ && this->vertex_buffer_ != nullptr)
        {
            this->_0x18_ = false; // whyyyyy do we even neeeeed this thing???
        }

        this->vertex_buffer_->Unlock();
        this->polies_ = nullptr;
        this->locked_ = false;
    }

    template <typename Vertex> auto poly_manager<Vertex>::allocate() -> poly_manager<Vertex>::poly*
    {
        if (this->locked_ && this->poly_count_ < this->max_polies_)
        {
            return this->polies_ + this->poly_count_++;
        }

        return nullptr;
    }
}
