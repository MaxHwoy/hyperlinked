#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/poly_manager.hpp>

namespace hyper
{
    struct prelit_vertex
    {
        vector3 position;
        color32 color;
        vector2 uv;
    };

    class prelit_pool : public poly_manager<prelit_vertex>
    {
    public:
        prelit_pool(std::uint32_t max_poly);

    public:
        static void ctor(prelit_pool& pool);

        static void dtor(prelit_pool& pool);

    public:
        static inline prelit_pool& instance = *reinterpret_cast<prelit_pool*>(0x00B437A8);
    };

    class fe_renderer final
    {
    public:
        fe_renderer(std::uint32_t length);

        ~fe_renderer();

    public:
        static void ctor(fe_renderer& renderer);

        static void dtor(fe_renderer& renderer);

    private:
        ::IDirect3DVertexBuffer9* vertex_buffer_;

    public:
        static inline fe_renderer& instance = *reinterpret_cast<fe_renderer*>(0x00B43044);
    };

    ASSERT_SIZE(prelit_vertex, 0x18);
    ASSERT_SIZE(prelit_pool::poly, 0x60);
    ASSERT_SIZE(prelit_pool, 0x24);
}
