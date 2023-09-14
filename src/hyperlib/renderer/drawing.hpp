#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/assets/flares.hpp>
#include <hyperlib/world/collision.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/lighting.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    enum class camera_mover_types : std::uint32_t
    {
        none,
        drive_cubic,
        debug_world,
        road_editor,
        orbit_car,
        rear_view_mirror,
        track_car,
        max,
        select_car,
        still,
        race_start,
        zone_freeze,
        zone_preview,
        auto_pilot,
        ice,
        animation_controller,
        cop_view,
        animation_entity,
        showcase,
        pip,
        count,
    };

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

    class render_target
    {
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

    struct __declspec(align(0x10)) render_view
    {
        view_id id;
        bool has_camera;
        bool is_shadow_projection_view;
        ::IDirect3DSurface9* d3d_render_target;
        ::IDirect3DSurface9* d3d_depth_stencil_surface;
        __declspec(align(0x10)) vector3 camera_direction;
        __declspec(align(0x10)) vector3 camera_up_vector;
        __declspec(align(0x10)) vector3 camera_previous_direction;
        __declspec(align(0x10)) vector3 camera_position;
        __declspec(align(0x10)) vector3 camera_velocity;
        __declspec(align(0x10)) matrix4x4 camera_matrix;
        float camera_focal_distance;
        float camera_depth_of_field;
        float camera_dof_falloff;
        float camera_dof_max_intensity;
        bool has_outside_view;
        camera_mover_types camera_mover_type;
        bool is_hood_render_view;
        std::int32_t something_0xBC;
        matrix4x4 projection_matrix;
        matrix4x4 view_matrix;
        matrix4x4 view_projection_matrix;
        vector4 vector4_0x180;
        matrix4x4 non_jittered_projection_matrix;
        render_target* target;

        static inline auto views = array<render_view, static_cast<size_t>(view_id::count)>(0x00B47A90);
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

    struct __declspec(align(0x10)) camera_params
    {
        __declspec(align(0x10)) matrix4x4 matrix;
        __declspec(align(0x10)) vector3 position;
        __declspec(align(0x10)) vector3 direction;
        __declspec(align(0x10)) vector3 target;
        __declspec(align(0x10)) vector4 noise_frequency_1;
        __declspec(align(0x10)) vector4 noise_amplitude_1;
        __declspec(align(0x10)) vector4 noise_frequency_2;
        __declspec(align(0x10)) vector4 noise_amplitude_2;
        __declspec(align(0x10)) vector4 fade_color;
        float target_distance;
        float focal_distance;
        float depth_of_field;
        float dof_falloff;
        float dof_max_intensity;
        float near_clip;
        float far_clip;
        float lb_height;
        float sim_time_multiplier;
        std::uint16_t horizontal_fov;
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

    struct __declspec(align(0x10)) camera
    {
        camera_params current_key;
        camera_params previous_key;
        camera_params velocity_key;
        bool clear_velocity;
        float elapsed_time;
        float last_update_time;
        float last_disparate_time;
        bool render_dash;
        float noise_intensity;
    };

    hyper_interface camera_mover : public linked_node<camera_mover>
    {
        virtual void on_w_collider() = 0;
        virtual ~camera_mover() = default;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual auto get_anchor() -> camera_anchor* = 0;
        virtual void set_anchor() = 0;
        virtual void set_look_back() = 0;
        virtual void set_look_back_speed() = 0;
        virtual void set_disable_lag() = 0;
        virtual void set_pov_type() = 0;
        virtual void get_pov_type() = 0;
        virtual bool is_hood_camera() = 0;
        virtual void outside_pov() = 0;

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
        geometry::pca_blend_data* blend_data;
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
        static void create_rendering_model(geometry::mesh_entry* entry, geometry::solid* solid, draw_flags flags, effect* effect, texture::info** textures, const matrix4x4* trs, const lighting::dynamic_context* context, const light_material::instance* material, const matrix4x4* blend_trs, geometry::pca_blend_data* pca);

        static void compute_sort_key(rendering_model& model);

        static auto create_flare_view_mask(view_id id) -> std::uint32_t;

        static bool can_render_flares_in_view(view_id id);

        static bool is_friend_flare_view_already_committed(view_id id);

        static auto get_next_light_flare_in_pool(std::uint32_t mask) -> flare::instance*;

        static void remove_current_light_flare_in_pool();

        static void init_light_flare_pool();

        static void reset_light_flare_pool();

        static void render_light_flare_pool(const view::instance* view);

        static void render_light_flare(const view::instance* view, flare::instance& flare, const matrix4x4* local_world, float intensity_scale, flare::reflection refl_type, flare::render render_type, float horizontal_flare_scale, float reflection_override, color32 color_override, float size_scale);

    public:
        static inline float& world_time_elapsed = *reinterpret_cast<float*>(0x00A996F8);

        static inline bool& draw_world = *reinterpret_cast<bool*>(0x00A63E0C);

        static inline std::uint32_t& world_detail = *reinterpret_cast<std::uint32_t*>(0x00A65370);

        static inline std::uint32_t& shadow_detail = *reinterpret_cast<std::uint32_t*>(0x00A65398);

        static inline bool& road_reflection_enabled = *reinterpret_cast<bool*>(0x00A65358);

        static inline view_mode& mode = *reinterpret_cast<view_mode*>(0x00AB0A38);

        static inline float& wind_angle = *reinterpret_cast<float*>(0x00B74D48);

        static inline bool& flare_pool_off = *reinterpret_cast<bool*>(0x00B42F1C);

        static inline bool& draw_light_flares = *reinterpret_cast<bool*>(0x00A6C088);

    private:
        static inline array<rendering_model, 0x1000> rendering_models_ = array<rendering_model, 0x1000>(0x00AB2780);

        static inline rendering_order*& rendering_orders_ = *reinterpret_cast<rendering_order**>(0x00B1DB90);

        static inline std::uint32_t& rendering_model_count_ = *reinterpret_cast<std::uint32_t*>(0x00AB0BF0);

        static bool flare_pool_inited_;

        static std::uint32_t active_flare_count_;

        static bool active_flare_types_[4]; // red, amber, green, generic

        static float active_flare_times_[4]; // red, amber, green, generic

        static float active_flare_blink_[4]; // red, amber, green, generic

        static std::uint32_t flare_texture_keys_[5];

        static texture::e_texture flare_texture_infos_[5];

        static bitset<static_cast<size_t>(view_id::count)> flare_mask_;

        static bitset<static_cast<size_t>(view_id::count)> view_to_flare_;

        static array<flare::params*, static_cast<size_t>(flare::type::count) * 2u> flare_params_;

        static inline flare::instance flare_pool_[1000]{};

        static inline std::uint32_t flare_bits_[1000]{};
    };

    CREATE_ENUM_EXPR_OPERATORS(model_lod);
    CREATE_ENUM_EXPR_OPERATORS(render_target_id);
    CREATE_ENUM_EXPR_OPERATORS(view_id);
    CREATE_ENUM_FLAG_OPERATORS(poly_flags);

    ASSERT_SIZE(render_target, 0x1C);
    ASSERT_SIZE(render_view, 0x1E0);
    ASSERT_SIZE(poly, 0xA0);
    ASSERT_SIZE(camera_params, 0xF0);
    ASSERT_SIZE(camera_anchor, 0xF0);
    ASSERT_SIZE(camera, 0x2F0);
    ASSERT_SIZE(camera_mover, 0x8C);
    ASSERT_SIZE(screen_effect_inf, 0x0C);
    ASSERT_SIZE(screen_effect_def, 0x50);
    ASSERT_SIZE(screen_effect_db, 0x1E8);
    ASSERT_SIZE(on_screen_rain_entry, 0x1C);
    ASSERT_SIZE(on_screen_rain, 0x234);
    ASSERT_SIZE(rain, 0x5180);
    ASSERT_SIZE(rendering_model, 0x68);
}
