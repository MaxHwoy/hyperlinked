#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/world/collision.hpp>
#include <hyperlib/renderer/enums.hpp>

namespace hyper
{
    enum class anchor_car_class : std::uint32_t
    {
        regular,
        exotic,
        muscle,
        tuner,
    };

    class __declspec(align(0x10)) camera
    {
    public:
        struct __declspec(align(0x10)) params
        {
            matrix4x4 view_matrix;
            vector4 position;
            vector4 direction;
            vector4 target;
            vector4 noise_frequency_1;
            vector4 noise_amplitude_1;
            vector4 noise_frequency_2;
            vector4 noise_amplitude_2;
            vector4 fade_color;
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

    public:
        camera();

        void set_camera_matrix(const matrix4x4& world_to_camera, float elapsed_time);

        void get_right_vector(vector3& vector) const;

        void get_up_vector(vector3& vector) const;

        void get_forward_vector(vector3& vector) const;

    public:
        params current_key;
        params previous_key;
        params velocity_key;
        bool clear_velocity;
        float elapsed_time;
        float last_update_time;
        float last_disparate_time;
        bool render_dash;
        float noise_intensity;

    public:
        static inline bool& stop_updating = *reinterpret_cast<bool*>(0x00A888D8);
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
        virtual auto outside_pov() const->std::uint32_t = 0;
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

    ASSERT_SIZE(camera::params, 0xF0);
    ASSERT_SIZE(camera, 0x2F0);
    ASSERT_SIZE(camera_anchor, 0xF0);
    ASSERT_SIZE(camera_mover, 0x8C);
}
