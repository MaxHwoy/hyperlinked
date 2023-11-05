#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
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

    ASSERT_SIZE(camera::params, 0xF0);
    ASSERT_SIZE(camera, 0x2F0);
}
