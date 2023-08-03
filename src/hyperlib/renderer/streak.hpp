#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/flares.hpp>

namespace hyper
{
    class streak final
    {
    public:
        struct vertex
        {
            vector3 position;
            color32 color;
            vector2 uv;
            vector4 vector;
            float adder;
        };

        struct poly
        {
            vertex vertices[4];
        };

        class manager final
        {
        public:
            void initialize();

            void destroy();

            void lock();

            void unlock();

            void render(struct render_view* view, void* flush_fac);

            void commit_flare(const vector3& position, const texture::info* texture, flare::type type, color32 color, float horizontal_scale, float vertical_scale, float degree_angle);

        public:
            IDirect3DVertexBuffer9* vertex_buffer;
            IDirect3DIndexBuffer9* index_buffer;
            std::uint32_t vertex_count;
            std::uint32_t max_streaks;
            std::uint32_t streak_count;
            texture::info* flare_texture_page;
            bool _0x18;
            poly* polies;
            bool locked;
            texture::info* streak_flares_texture;

            static inline manager& instance = *reinterpret_cast<manager*>(0x00B4CF28);

            static inline bool emergency_reeval = false;
        };
    };

    ASSERT_SIZE(streak::vertex, 0x2C);
    ASSERT_SIZE(streak::poly, 0xB0);
    ASSERT_SIZE(streak::manager, 0x28);
}
