#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/effect.hpp>
#include <hyperlib/renderer/fe_renderer.hpp>

namespace hyper
{
    prelit_pool::prelit_pool(std::uint32_t max_poly) : poly_manager<prelit_vertex>(max_poly)
    {
        this->texture_page_ = texture::get_texture_info(hashing::bin_const("MAIN"), false, false);
    }

    void prelit_pool::render(const render_view& view)
    {
        directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, false);

        effect_world_prelit& prelit = *effect_world_prelit::instance;

        prelit.set_current_pass(0u, nullptr, false);

        prelit.set_transforms(matrix4x4::identity(), view, false);

        poly_manager<prelit_vertex>::render(prelit, nullptr);

        prelit.finalize();

        directx::device()->SetRenderState(::D3DRS_ZWRITEENABLE, true);
    }

    void prelit_pool::ctor(prelit_pool& pool)
    {
        new (&pool) prelit_pool(2000u); // by default, prelit pool is initialized to 2000 flares
    }

    void prelit_pool::dtor(prelit_pool& pool)
    {
        pool.~prelit_pool();
    }

    fe_renderer::fe_renderer(std::uint32_t length)
    {
        directx::device()->CreateVertexBuffer(length, D3DUSAGE_DYNAMIC, 0u, ::D3DPOOL_DEFAULT, &this->vertex_buffer_, nullptr);
    }

    fe_renderer::~fe_renderer()
    {
        if (fe_renderer::vertex_buffer_ != nullptr)
        {
            fe_renderer::vertex_buffer_->Release();

            fe_renderer::vertex_buffer_ = nullptr;
        }
    }

    void fe_renderer::ctor(fe_renderer& renderer)
    {
        new (&renderer) fe_renderer(160000u);
    }

    void fe_renderer::dtor(fe_renderer& renderer)
    {
        renderer.~fe_renderer();
    }
}
