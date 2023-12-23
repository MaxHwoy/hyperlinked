#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class visual_treatment;

    class post_process final
    {
    public:
        const static inline std::uint32_t target_count = 2;

    public:
        post_process();

        ~post_process();

    private:
        static bool can_do_noise(const post_process& process, const screen_effect& screen);

    public:
        static void ctor(post_process& process);

        static void dtor(post_process& process);

        static void apply(post_process& process, const view::instance& view);

        static void do_motion_blur(post_process& process, visual_treatment& vt, view_id id);

        static void do_uves_over_cliff(post_process& process, visual_treatment& vt, view_id id);

        static void do_uves_over_cliff_darken(post_process& process, visual_treatment& vt, view_id id);

        static auto get_current_texture(const post_process& process) -> ::IDirect3DTexture9*;

        static auto get_current_surface(const post_process& process) -> ::IDirect3DSurface9*;

        static auto get_next_texture(const post_process& process) -> ::IDirect3DTexture9*;

        static auto get_next_surface(const post_process& process) -> ::IDirect3DSurface9*;

        static void rotate_current_surface();

    private:
        ::IDirect3DSurface9* surfaces_[target_count];
        char data_[0x10];
        ::IDirect3DTexture9* textures_[target_count];

    public:
        static inline post_process& instance = *reinterpret_cast<post_process*>(0x00AB0678);

        static inline bool& inited = *reinterpret_cast<bool*>(0x00B1F2E0);

    private:
        static inline std::uint32_t& current_index_ = *reinterpret_cast<std::uint32_t*>(0x00AB0B4C);
    };
}
