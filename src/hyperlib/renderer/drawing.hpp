#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/pca.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/assets/flares.hpp>
#include <hyperlib/world/collision.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/lighting.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    enum class anchor_car_class : std::uint32_t
    {
        regular,
        exotic,
        muscle,
        tuner,
    };

    enum class screen_effect_control : std::uint32_t
    {
        frame,
        boolean,
        function,
    };

    enum class screen_effect_type : std::uint32_t
    {
        tint,
        motion_blur,
        visual_sig,
        glare,
        fe_blur,
        count,
    };

    enum class curtain_status : std::uint32_t
    {
        inactive,
        active,
        turn_on,
        override,
    };

    enum class rain_wind_type : std::uint32_t
    {
        point,
        vector,
        count,
    };

    struct __declspec(align(0x10)) poly
    {
        vector3pad vertices[4];
        float uvs[4][2];
        float uvs_mask[4][2];
        color32 colors[4];
        poly_flags flags;
        char flailer;
    };

    struct camera_anchor
    {
        std::uint32_t world_id;
        anchor_car_class car_class;
        __declspec(align(0x10)) vector3 dimension;
        __declspec(align(0x10)) vector3 position;
        __declspec(align(0x10)) matrix4x4 rotation;
        __declspec(align(0x10)) vector3 acceleration;
        __declspec(align(0x10)) vector3 velocity;
        __declspec(align(0x10)) float velocity_magnitude;
        float top_speed;
        float rpm;
        std::uint32_t wheel_count;
        bool is_touching_ground;
        char sim_surface[0x10];
        float forward_slip;
        float slip_angle;
        float drift_state;
        float drift;
        float steer;
        float world_collision;
        float ground_collision;
        float object_collision;
        float nos_percentage_left;
        float vertigo_camera_direction;
        float time_doing_vertigo_effect;
        bool is_nos_engaged;
        bool is_brake_engaged;
        bool is_hand_brake_engaged;
        bool is_drag_race;
        bool is_drift_race;
        bool is_canyon_drift_race;
        bool is_over_rev;
        bool is_vehicle_destroyed;
        bool is_gear_changing;
        bool is_close_to_roadblock;
        bool can_do_vertigo_again;
        bool is_doing_vertigo_effect;
        bool is_camera_frozen;
    };

    hyper_interface camera_mover : public linked_node<camera_mover>
    {
    public:
        virtual void on_w_collider() = 0;
        virtual ~camera_mover() = default;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual auto get_anchor() -> camera_anchor* = 0;
        virtual void set_look_back(bool) = 0;
        virtual void set_look_back_speed(float) = 0;
        virtual void set_disable_lag() = 0;
        virtual void set_pov_type() = 0;
        virtual void get_pov_type() const = 0;
        virtual bool is_hood_camera() const = 0;
        virtual auto outside_pov() const -> std::uint32_t = 0;
        virtual void render_car_pov() = 0;
        virtual auto min_dist_to_wall() -> float = 0;
        virtual auto get_lookback_angle() -> float = 0;
        virtual void reset_state() = 0;
        virtual void enable_camera_mover() = 0;
        virtual void disable_camera_mover() = 0;
        virtual auto get_target() -> void* = 0;

    public:
        inline auto vtable_fix() -> camera_mover*
        {
            return reinterpret_cast<camera_mover*>(reinterpret_cast<uintptr_t>(this) - sizeof(void*));
        }

        inline auto vtable_fix() const -> const camera_mover*
        {
            return reinterpret_cast<const camera_mover*>(reinterpret_cast<uintptr_t>(this) - sizeof(void*));
        }

    public:
        camera_mover_types type;
        view_id id;
        bool enabled;
        struct view_base* view;
        camera* camera;
        bool render_dash;
        collision::collider* collider;
        collision::world_pos world_pos;
        float accumulated_clearance;
        float accumulated_adjust;
        float saved_adjust;
        vector3pad saved_forward;
        float zoom;
        vector2 camera_rotation;
    };

    struct __declspec(align(0x04)) screen_effect_inf
    {
        screen_effect_control controller;
        std::uint32_t frame_num;
        bool active;
    };

    struct screen_effect_def
    {
        color color;
        float intensity;
        float data[14];
        void(__cdecl* update)(screen_effect_type, struct screen_effect_db*);
    };

    struct screen_effect_db
    {
        struct view_base* view;
        screen_effect_inf se_inf[5];
        screen_effect_def se_data[5];
        std::uint32_t num_type[5];
        float se_time;
    };

    struct face_pixelation
    {
        struct view_base* view;
        vector2 screen;
    };

    struct on_screen_rain_entry
    {
        float data[6];
        std::uint32_t index;
    };

    struct on_screen_rain
    {
        std::uint32_t entry_count;
        on_screen_rain_entry entries[20];
    };

    struct rain
    {
        on_screen_rain screen_rain;
        __declspec(align(0x04)) bool no_rain;
        __declspec(align(0x04)) bool no_rain_ahead;
        __declspec(align(0x04)) bool in_tunnel;
        __declspec(align(0x04)) bool in_overpass;
        __declspec(align(0x04)) struct track_path_zone* zone;
        char padding_0x0244[8];
        vector3pad out_vex;
        vector2 position;
        char padding_0x0268[8];
        vector3pad local_cam_velocity;
        curtain_status rain_curtain;
        std::uint32_t render_count;
        view::instance* view;
        float rain_intensity;
        float cloud_intensity;
        std::uint32_t desired_active;
        std::uint32_t raindrop_count[2];
        vector3pad raindrop_positions[350][2];
        char old_swap_buffer[350][4];
        std::uint32_t rain_point_count;
        std::uint32_t num_of_type[2];
        std::uint32_t desired_num_of_type[2];
        float percentages[2];
        float precip_wind_effect[2][2];
        texture::info* textures[2];
        char padding_0x340C[4];
        vector3pad old_car_direction;
        vector3pad precip_radius[2];
        vector3pad precip_speed_range[2];
        float precip_z_constant[2];
        rain_wind_type wind_type[2];
        float camera_speed;
        char padding_0x3474[12];
        vector3pad wind_speed;
        vector3pad desired_wind_speed;
        float desired_wind_time;
        float wind_time;
        std::uint32_t fog_red;
        std::uint32_t fog_green;
        std::uint32_t fog_blue;
        vector2 bbox_max;
        vector2 bbox_min;
        char padding_0x34C4[12];
        poly precip_poly[2];
        matrix4x4 local_to_world;
        matrix4x4 world_to_local;
        float len_modifier;
        float desired_intensity;
        float desired_cloudyness;
        float desired_road_dampness;
        float road_dampness;
        float precip_percent[2];
        char padding_0x36AC[4];
        vector3pad prevail_wind_speed;
        float weather_time;
        float desired_weather_time;
        vector3pad velocities[2][10];
        vector2 ent0;
        vector2 ent1;
        vector2 ext0;
        vector2 ext1;
        char data_0x3828[6488];
    };

    struct rendering_model
    {
        texture::render_state render_bits;
        texture::info* base_texture_info;
        ::IDirect3DTexture9* d3d9_diffuse_texture;
        ::IDirect3DTexture9* d3d9_normal_texture;
        ::IDirect3DTexture9* d3d9_height_texture;
        ::IDirect3DTexture9* d3d9_specular_texture;
        ::IDirect3DTexture9* d3d9_opacity_texture;
        geometry::mesh_entry* mesh_entry;
        bool is_tri_stripped;
        geometry::solid* solid;
        draw_flags flags;
        effect* effect;
        const lighting::dynamic_context* light_context;
        const light_material::instance* light_material;
        const matrix4x4* local_to_world;
        const matrix4x4* blending_matrices;
        const texture::info* diffuse_texture_info;
        const texture::info* normal_texture_info;
        const texture::info* height_texture_info;
        const texture::info* specular_texture_info;
        const texture::info* opacity_texture_info;
        std::int32_t sort_flags;
        void* null;
        float negative_one;
        pca::blend_data* blend_data;
        bool use_low_lod;
    };

    struct rendering_order
    {
        std::uint32_t model_index;
        std::int32_t sort_flags;
    };

    class renderer final
    {
    public:
        static void create_rendering_model(geometry::mesh_entry* entry, geometry::solid* solid, draw_flags flags, effect* effect, texture::info** textures, const matrix4x4* trs, const lighting::dynamic_context* context, const light_material::instance* material, const matrix4x4* blend_trs, pca::blend_data* pca);

        static void compute_sort_key(rendering_model& model);

        static void init_render_targets();

        static void set_render_target(render_target& target, bool clear, ::D3DCOLOR clear_color);

        static void update_render_views();

        static void sort_models_and_draw_world();

    public:
        static inline bool& draw_world = *reinterpret_cast<bool*>(0x00A63E0C);

        static inline std::uint32_t& world_detail = *reinterpret_cast<std::uint32_t*>(0x00A65370);

        static inline std::uint32_t& shadow_detail = *reinterpret_cast<std::uint32_t*>(0x00A65398);

        static inline bool& road_reflection_enabled = *reinterpret_cast<bool*>(0x00A65358);

        static inline view_mode& mode = *reinterpret_cast<view_mode*>(0x00AB0A38);

        static inline float& wind_angle = *reinterpret_cast<float*>(0x00B74D48);

        static inline bool& use_lowlod_pass = *reinterpret_cast<bool*>(0x00AB0B98);

    private:
        static inline array<rendering_model, 0x1000> rendering_models_ = array<rendering_model, 0x1000>(0x00AB2780);

        static inline rendering_order*& rendering_orders_ = *reinterpret_cast<rendering_order**>(0x00B1DB90);

        static inline std::uint32_t& rendering_model_count_ = *reinterpret_cast<std::uint32_t*>(0x00AB0BF0);
    };

    CREATE_ENUM_FLAG_OPERATORS(poly_flags);

    ASSERT_SIZE(poly, 0xA0);
    ASSERT_SIZE(camera_anchor, 0xF0);
    ASSERT_SIZE(camera_mover, 0x8C);
    ASSERT_SIZE(screen_effect_inf, 0x0C);
    ASSERT_SIZE(screen_effect_def, 0x50);
    ASSERT_SIZE(screen_effect_db, 0x1E8);
    ASSERT_SIZE(on_screen_rain_entry, 0x1C);
    ASSERT_SIZE(on_screen_rain, 0x234);
    ASSERT_SIZE(rain, 0x5180);
    ASSERT_SIZE(rendering_model, 0x68);
}
