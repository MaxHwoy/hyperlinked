#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/pca.hpp>
#include <hyperlib/assets/lights.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/screen_effect.hpp>

namespace hyper
{
    enum class clipping_plane_type : std::uint32_t
    {
        left,
        right,
        bottom,
        top,
        near,
        far,
        count,
    };

    struct clipping_planes
    {
        plane planes[static_cast<std::uint32_t>(clipping_plane_type::count)];
    };

    class view final
    {
    public:
        struct platform_info
        {
            matrix4x4 view_matrix;
            matrix4x4 projection_matrix;
            matrix4x4 view_projection_matrix;
            matrix4x4 non_jittered_projection_matrix;
            matrix4x4 non_jittered_view_projection_matrix;
            clipping_planes clipping_planes;

            static array<platform_info, static_cast<size_t>(view_id::count)> infos;
        };

        struct platform_interface
        {
            platform_info* pinfo;

            auto get_visible_state_sb(const bounds& bounds) const -> visible_state;

            auto get_visible_state_sb(const vector3& point, const matrix4x4* trs) const -> visible_state;

            auto get_visible_state_sb(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> visible_state;

            void get_screen_position(const vector3& world_pos, vector3& screen_pos) const;

            auto shadow_map_cull(const vector3& bbox_min, const vector3& bbox_max) const -> visible_state;
        };

        struct base : public platform_interface
        {
        public:
            inline bool is_player()
            {
                return this->id == view_id::player1 || this->id == view_id::player2;
            }

            inline bool is_reflection()
            {
                return this->id == view_id::player1_reflection || this->id == view_id::player2_reflection;
            }

            inline bool is_env_map()
            {
                return this->id >= view_id::env_z_pos && this->id <= view_id::env_y_neg;
            }

        public:
            const char* name;
            view_id id;
            bool active;
            std::int8_t letter_box;
            float pixelation;
            float near_z;
            float far_z;
            float fov_bias;
            float fov_degrees;
            std::uint16_t fov_angle;
            std::int32_t black_and_white_mode;
            std::int32_t pixel_min_size;
            vector3 view_direction;
            std::uint32_t pad04;
            class camera* camera;
            linked_list<struct camera_mover> camera_mover_list;
            const light::context::dynamic* world_light_context;
            class render_target* attached_target;
            char pad0C[0x0C];
        };

        struct __declspec(align(0x10)) instance : public base
        {
        public:
            static auto calculate_pixelation(std::uint16_t horizontal_fov) -> float;

        public:
            void calculate_view_matrices(float near_clip, float far_clip, bool calculate_frustum);

            auto get_flare_intensity() const -> float;

            bool is_in_a_tunnel(bool check_overpass) const;

            auto get_camera_mover() const -> struct camera_mover*;

            auto get_screen_depth(const vector3& point, const matrix4x4* trs) const -> float;

            auto get_screen_depth(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> float;

            void setup_world_light_context();

            void render(geometry::model& model, const matrix4x4* local_world, const light::context::dynamic* context, draw_flags flags, const matrix4x4* blend_trs, pca::blend_data* pca) const;

        public:
            std::uint32_t num_cops_in_view;
            std::uint32_t num_cops_total;
            std::uint32_t num_cops_cherry;
            struct rain* rain;
            screen_effect::db* screen_effect;
            class face_pixelation* face_pixelation;

        public:
            static array<instance, static_cast<size_t>(view_id::count)> views;

            static inline matrix4x4& super_rotation = *reinterpret_cast<matrix4x4*>(0x00B1F5F0);
        };

        class mode final
        {
        public:
            static auto current() -> view_mode;

            static void update();

            static void maybe_change();

        private:
            static inline view_mode& current_ = *reinterpret_cast<view_mode*>(0x00AB0A38);
        };
    };

    ASSERT_SIZE(clipping_planes, 0x60);
    ASSERT_SIZE(view::platform_info, 0x1A0);
    ASSERT_SIZE(view::base, 0x60);
    ASSERT_SIZE(view::instance, 0x80);
}
