#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class post_process final
    {
    public:
        const static inline std::uint32_t target_count = 2;

    public:
        post_process();

        ~post_process();

    public:
        static void ctor(post_process& process);

        static void dtor(post_process& process);

        static void apply(post_process& process, const view::instance& view);

    private:
        ::IDirect3DSurface9* surfaces_[target_count];
        char data_[0x10];
        ::IDirect3DTexture9* textures_[target_count];

    public:
        static inline post_process& instance = *reinterpret_cast<post_process*>(0x00AB0678);

        static inline bool& inited = *reinterpret_cast<bool*>(0x00B1F2E0);
    };
}