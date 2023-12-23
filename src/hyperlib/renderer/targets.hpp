#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/options.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/directx.hpp>

namespace hyper
{
    class render_target
    {
    public:
        void ctor(bool act, render_target_id tid, view_id vid, std::uint32_t x, std::uint32_t y, ::IDirect3DSurface9* target, ::IDirect3DSurface9* depth_stencil);

    public:
        render_target_id target_id;
        view_id view_id;
        ::IDirect3DSurface9* d3d_target;
        ::IDirect3DSurface9* d3d_depth_stencil;
        bool active;
        __declspec(align(0x04)) std::uint32_t resolution_x;
        __declspec(align(0x04)) std::uint32_t resolution_y;

    public:
        static inline auto targets = array<render_target, static_cast<size_t>(render_target_id::count)>(0x00AB04D0);

        static inline render_target*& current = *reinterpret_cast<render_target**>(0x00AB08F4);
    };

    class player_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::player;

        constexpr static inline hyper::view_id vid = view_id::player1;

    public:
        static inline auto instance() -> player_render_target&
        {
            return *reinterpret_cast<player_render_target*>(&render_target::targets[player_render_target::tid]);
        }

        static inline void init()
        {
            player_render_target::instance().ctor
            (
                true,
                player_render_target::tid,
                player_render_target::vid,
                directx::resolution_x,
                directx::resolution_y,
                options::visual_treatment ? player_render_target::render_target_surface_standalone : player_render_target::render_target_surface_postprocess,
                player_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::IDirect3DTexture9*& d3d_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB08FC);

        static inline ::IDirect3DSurface9*& render_target_surface_standalone = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB08F8);

        static inline ::IDirect3DSurface9*& render_target_surface_postprocess = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0900);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0904);
    };

    class vt_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::visual_treatment;

        constexpr static inline hyper::view_id vid = view_id::player1;

    public:
        static inline auto instance() -> vt_render_target&
        {
            return *reinterpret_cast<vt_render_target*>(&render_target::targets[vt_render_target::tid]);
        }

        static inline void init()
        {
            vt_render_target::instance().ctor
            (
                true,
                vt_render_target::tid,
                vt_render_target::vid,
                directx::resolution_x,
                directx::resolution_y,
                player_render_target::render_target_surface_postprocess,
                player_render_target::depth_stencil_surface
            );
        }

    public:
        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00A63F80);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00A63F84);
    };

    class reflection_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::reflection;

        constexpr static inline hyper::view_id vid = view_id::player1_reflection;

    public:
        static inline auto instance() -> reflection_render_target&
        {
            return *reinterpret_cast<reflection_render_target*>(&render_target::targets[reflection_render_target::tid]);
        }

        static inline void init()
        {
            reflection_render_target::instance().ctor
            (
                true,
                reflection_render_target::tid,
                reflection_render_target::vid,
                reflection_render_target::resolution_x,
                reflection_render_target::resolution_y,
                reflection_render_target::render_target_surface,
                reflection_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::D3DFORMAT& format = *reinterpret_cast<::D3DFORMAT*>(0x00A63AF0);

        static inline ::IDirect3DTexture9*& d3d_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB095C);

        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0958);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0960);

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00A63AF8);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00A63AFC);
    };

    class flailer_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::flailer;

        constexpr static inline hyper::view_id vid = view_id::flailer;

    public:
        static inline auto instance() -> flailer_render_target&
        {
            return *reinterpret_cast<flailer_render_target*>(&render_target::targets[flailer_render_target::tid]);
        }

        static inline void init()
        {
            flailer_render_target::instance().ctor
            (
                true,
                flailer_render_target::tid,
                flailer_render_target::vid,
                directx::resolution_x,
                directx::resolution_y,
                player_render_target::render_target_surface_postprocess,
                nullptr
            );
        }
    };

    class rvm_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::rvm;

        constexpr static inline hyper::view_id vid = view_id::player1_rvm;

    public:
        static inline auto instance() -> rvm_render_target&
        {
            return *reinterpret_cast<rvm_render_target*>(&render_target::targets[rvm_render_target::tid]);
        }

        static inline void init()
        {
            rvm_render_target::instance().ctor
            (
                true,
                rvm_render_target::tid,
                rvm_render_target::vid,
                rvm_render_target::resolution_x,
                rvm_render_target::resolution_y,
                rvm_render_target::render_target_surface,
                rvm_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::IDirect3DTexture9*& d3d_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0914);

        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0918);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB091C);

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00A63AD0);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00A63AD4);
    };

    class shadowmap_render_target : public render_target
    {
    public:
        enum class target_type
        {
            render_target,
            stencil_d24s8,
            stencil_df16,
        };

    public:
        constexpr static inline render_target_id tid = render_target_id::shadowmap;

        constexpr static inline hyper::view_id vid = view_id::player1_shadowmap;

    public:
        static inline auto instance() -> shadowmap_render_target&
        {
            return *reinterpret_cast<shadowmap_render_target*>(&render_target::targets[shadowmap_render_target::tid]);
        }

        static inline void init()
        {
            shadowmap_render_target::instance().ctor
            (
                true,
                shadowmap_render_target::tid,
                shadowmap_render_target::vid,
                shadowmap_render_target::resolution_x,
                shadowmap_render_target::resolution_y,
                shadowmap_render_target::render_target_surface,
                shadowmap_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0C00);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0C08);

        static inline ::IDirect3DTexture9*& render_target_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0C04);

        static inline ::IDirect3DTexture9*& depth_stencil_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0C0C);

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00715091);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00715091);

        static inline target_type& shadow_target_type = *reinterpret_cast<target_type*>(0x00AB0AF0);
    };

    class pip_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::pip;

        constexpr static inline hyper::view_id vid = view_id::player1_pip;

    public:
        static inline auto instance() -> pip_render_target&
        {
            return *reinterpret_cast<pip_render_target*>(&render_target::targets[pip_render_target::tid]);
        }

        static inline void init()
        {
            pip_render_target::instance().ctor
            (
                true,
                pip_render_target::tid,
                pip_render_target::vid,
                pip_render_target::resolution_x,
                pip_render_target::resolution_y,
                pip_render_target::render_target_surface,
                pip_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::D3DFORMAT& format = *reinterpret_cast<::D3DFORMAT*>(0x00A63B08);

        static inline ::IDirect3DTexture9*& d3d_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0964);

        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0968);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB096C);

        static inline std::uint32_t& resolution_x = *reinterpret_cast<std::uint32_t*>(0x00A63B00);

        static inline std::uint32_t& resolution_y = *reinterpret_cast<std::uint32_t*>(0x00A63B04);
    };

    class motion_blur_render_target : public render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::motion_blur;

        constexpr static inline hyper::view_id vid = view_id::player1;

    public:
        static inline auto instance() -> motion_blur_render_target&
        {
            return *reinterpret_cast<motion_blur_render_target*>(&render_target::targets[motion_blur_render_target::tid]);
        }

        static inline void init()
        {
            motion_blur_render_target::instance().ctor
            (
                true,
                motion_blur_render_target::tid,
                motion_blur_render_target::vid,
                directx::resolution_x,
                directx::resolution_y,
                motion_blur_render_target::render_target_surface,
                motion_blur_render_target::depth_stencil_surface
            );
        }

        static bool open();

        static void close();

    public:
        static inline ::IDirect3DTexture9*& fuzzz_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0908);

        static inline ::IDirect3DTexture9*& d3d_texture = *reinterpret_cast<::IDirect3DTexture9**>(0x00AB0908);

        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB090C);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0910);
    };

    class env_map_render_target : public render_target
    {
    public:
        static bool open();

        static void close();

    public:
        static inline ::IDirect3DCubeTexture9*& cube_texture = *reinterpret_cast<::IDirect3DCubeTexture9**>(0x00AB0920);

        static inline ::IDirect3DCubeTexture9*& fe_texture = *reinterpret_cast<::IDirect3DCubeTexture9**>(0x00AB0954);

        static inline ::IDirect3DVolumeTexture9*& car_volume = *reinterpret_cast<::IDirect3DVolumeTexture9**>(0x00AB09E8);

        static inline std::uint32_t& resolution_cube = *reinterpret_cast<std::uint32_t*>(0x00A63AE0);

        static inline std::uint32_t& resolution_fe = *reinterpret_cast<std::uint32_t*>(0x0070DE50);
    };

    class env_x_pos_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_x_pos;

        constexpr static inline hyper::view_id vid = view_id::env_x_pos;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_x_pos_render_target&
        {
            return *reinterpret_cast<env_x_pos_render_target*>(&render_target::targets[env_x_pos_render_target::tid]);
        }

        static inline void init()
        {
            env_x_pos_render_target::instance().ctor
            (
                true,
                env_x_pos_render_target::tid,
                env_x_pos_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_x_pos_render_target::render_target_surface,
                env_x_pos_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0924);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB093C);
    };

    class env_x_neg_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_x_neg;

        constexpr static inline hyper::view_id vid = view_id::env_x_neg;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_x_neg_render_target&
        {
            return *reinterpret_cast<env_x_neg_render_target*>(&render_target::targets[env_x_neg_render_target::tid]);
        }

        static inline void init()
        {
            env_x_neg_render_target::instance().ctor
            (
                true,
                env_x_neg_render_target::tid,
                env_x_neg_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_x_neg_render_target::render_target_surface,
                env_x_neg_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0928);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0940);
    };

    class env_y_pos_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_y_pos;

        constexpr static inline hyper::view_id vid = view_id::env_y_pos;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_y_pos_render_target&
        {
            return *reinterpret_cast<env_y_pos_render_target*>(&render_target::targets[env_y_pos_render_target::tid]);
        }

        static inline void init()
        {
            env_y_pos_render_target::instance().ctor
            (
                true,
                env_y_pos_render_target::tid,
                env_y_pos_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_y_pos_render_target::render_target_surface,
                env_y_pos_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB092C);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0944);
    };

    class env_y_neg_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_y_neg;

        constexpr static inline hyper::view_id vid = view_id::env_y_neg;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_y_neg_render_target&
        {
            return *reinterpret_cast<env_y_neg_render_target*>(&render_target::targets[env_y_neg_render_target::tid]);
        }

        static inline void init()
        {
            env_y_neg_render_target::instance().ctor
            (
                true,
                env_y_neg_render_target::tid,
                env_y_neg_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_y_neg_render_target::render_target_surface,
                env_y_neg_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0930);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0948);
    };

    class env_z_pos_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_z_pos;

        constexpr static inline hyper::view_id vid = view_id::env_z_pos;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_z_pos_render_target&
        {
            return *reinterpret_cast<env_z_pos_render_target*>(&render_target::targets[env_z_pos_render_target::tid]);
        }

        static inline void init()
        {
            env_z_pos_render_target::instance().ctor
            (
                true,
                env_z_pos_render_target::tid,
                env_z_pos_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_z_pos_render_target::render_target_surface,
                env_z_pos_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0934);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB094C);
    };

    class env_z_neg_render_target : public env_map_render_target
    {
    public:
        constexpr static inline render_target_id tid = render_target_id::env_z_neg;

        constexpr static inline hyper::view_id vid = view_id::env_z_neg;

    public:
        static bool open();

        static void close();

        static inline auto instance() -> env_z_neg_render_target&
        {
            return *reinterpret_cast<env_z_neg_render_target*>(&render_target::targets[env_z_neg_render_target::tid]);
        }

        static inline void init()
        {
            env_z_neg_render_target::instance().ctor
            (
                true,
                env_z_neg_render_target::tid,
                env_z_neg_render_target::vid,
                env_map_render_target::resolution_cube,
                env_map_render_target::resolution_cube,
                env_z_neg_render_target::render_target_surface,
                env_z_neg_render_target::depth_stencil_surface
            );
        }

    public:
        static inline ::IDirect3DSurface9*& render_target_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0938);

        static inline ::IDirect3DSurface9*& depth_stencil_surface = *reinterpret_cast<::IDirect3DSurface9**>(0x00AB0950);
    };

    class __declspec(align(0x10)) render_view
    {
    public:
        bool get_has_camera() const;

        auto get_focal_distance() const -> float;

        auto get_camera_dof() const -> float;

        auto get_camera_dof_falloff() const -> float;

        auto get_camera_dof_max_intensity() const -> float;

        void update(const view::instance& view);

    public:
        view_id id;
        bool has_camera;
        bool is_shadow_projection_view;
        ::IDirect3DSurface9* d3d_render_target;
        ::IDirect3DSurface9* d3d_depth_stencil_surface;
        __declspec(align(0x10)) vector3 camera_forward;
        __declspec(align(0x10)) vector3 camera_up;
        __declspec(align(0x10)) vector3 camera_prev_forward;
        __declspec(align(0x10)) vector3 camera_position;
        __declspec(align(0x10)) vector3 camera_velocity;
        __declspec(align(0x10)) matrix4x4 camera_view_matrix;
        float camera_focal_distance;
        float camera_depth_of_field;
        float camera_dof_falloff;
        float camera_dof_max_intensity;
        std::uint32_t has_camera_mover;
        camera_mover_types camera_mover_type;
        std::uint32_t outside_view_type;
        std::uint32_t something_0xBC;
        matrix4x4 projection_matrix;
        matrix4x4 view_matrix;
        matrix4x4 view_projection_matrix;
        vector4 vector4_0x180;
        matrix4x4 non_jittered_view_projection_matrix;
        render_target* target;

    public:
        static inline auto views = array<render_view, static_cast<size_t>(view_id::count)>(0x00B47A90);
    };

    ASSERT_SIZE(render_target, 0x1C);
    ASSERT_SIZE(render_view, 0x1E0);
}
