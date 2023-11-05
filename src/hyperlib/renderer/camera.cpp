#include <hyperlib/global_vars.hpp>
#include <hyperlib/online/trolli.hpp>
#include <hyperlib/renderer/camera.hpp>

namespace hyper
{
    camera::camera() : 
        clear_velocity(false), 
        last_update_time(0.0f), 
        elapsed_time(1.0f), 
        last_disparate_time(static_cast<float>(global::real_time_frames)), 
        render_dash(false), 
        noise_intensity(0.0f)
    {
        this->current_key.position.w = 1.0f;
        this->current_key.direction.w = 0.0f;
        this->current_key.target_distance = 10.0f;
        this->current_key.focal_distance = 0.0f;
        this->current_key.depth_of_field = 0.0f;
        this->current_key.dof_falloff = 0.0f;
        this->current_key.dof_max_intensity = 0.0f;
        this->current_key.near_clip = 0.5f;
        this->current_key.far_clip = 10000.0f;
        this->current_key.horizontal_fov = 0x36FB; // 77.316 degrees
        this->current_key.lb_height = 0.0f;
        this->current_key.sim_time_multiplier = 1.0f;
        this->current_key.fade_color.x = 0.0f;
        this->current_key.fade_color.y = 0.0f;
        this->current_key.fade_color.z = 0.0f;
        this->current_key.fade_color.w = 0.0f;
        this->current_key.noise_frequency_1.x = 1.0;
        this->current_key.noise_frequency_1.y = 1.0;
        this->current_key.noise_frequency_1.z = 1.0;
        this->current_key.noise_frequency_1.w = 1.0;
        this->current_key.noise_frequency_2.x = 1.0;
        this->current_key.noise_frequency_2.y = 1.0;
        this->current_key.noise_frequency_2.z = 1.0;
        this->current_key.noise_frequency_2.w = 1.0;
        this->current_key.noise_amplitude_1.x = 0.0;
        this->current_key.noise_amplitude_1.y = 0.0;
        this->current_key.noise_amplitude_1.z = 0.0;
        this->current_key.noise_amplitude_1.w = 0.0;
        this->current_key.noise_amplitude_2.x = 0.0;
        this->current_key.noise_amplitude_2.y = 0.0;
        this->current_key.noise_amplitude_2.z = 0.0;
        this->current_key.noise_amplitude_2.w = 0.0;
        
        matrix4x4 world_to_camera;
        
        world_to_camera.m11 = 1.0f;
        world_to_camera.m12 = 0.0f;
        world_to_camera.m13 = 0.0f;
        world_to_camera.m14 = 0.0f;
        world_to_camera.m21 = 0.0f;
        world_to_camera.m22 = -1.0f;
        world_to_camera.m23 = 0.0f;
        world_to_camera.m24 = 0.0f;
        world_to_camera.m31 = 0.0f;
        world_to_camera.m32 = 0.0f;
        world_to_camera.m33 = -1.0f;
        world_to_camera.m34 = 100.0f;
        world_to_camera.m41 = 0.0f;
        world_to_camera.m42 = 0.0f;
        world_to_camera.m43 = 1200.0f;
        world_to_camera.m44 = 1.0f;

        this->set_camera_matrix(world_to_camera, 1.0f);
    }

    void camera::get_right_vector(vector3& vector) const
    {
        vector = vector3(this->current_key.view_matrix.m11, this->current_key.view_matrix.m21, this->current_key.view_matrix.m31).normalized();
    }

    void camera::get_up_vector(vector3& vector) const
    {
        vector = vector3(this->current_key.view_matrix.m12, this->current_key.view_matrix.m22, this->current_key.view_matrix.m32).normalized();
    }

    void camera::get_forward_vector(vector3& vector) const
    {
        vector = this->current_key.direction.as_vector3();
    }

    void camera::set_camera_matrix(const matrix4x4& world_to_camera, float elapsed)
    {
        if (!camera::stop_updating)
        {
            ::memcpy(&this->previous_key, &this->current_key, sizeof(camera::params));

            this->elapsed_time = elapsed;

            if (!trolli::instance.use_matrix || trolli::disable_communication)
            {
                this->current_key.view_matrix = world_to_camera;
            }
            else
            {
                this->current_key.view_matrix = trolli::instance.camera_matrix;

                this->current_key.view_matrix.row(3u).as_vector3() *= 0.01f;
            }

            matrix4x4 rotation;

            math::invert_rotation(this->current_key.view_matrix, rotation);

            rotation.m44 = 1.0f;

            this->current_key.position = this->current_key.view_matrix.row(3u);

            math::transform_point(rotation, this->current_key.position);

            vector4::negate(this->current_key.position);

            this->current_key.direction = rotation.row(2u).normalized();

            this->current_key.target = vector4::scale_add(this->current_key.position, this->current_key.direction, this->current_key.target_distance);

            if (this->clear_velocity)
            {
                this->clear_velocity = false;

                this->elapsed_time = 1.0f;

                ::memcpy(&this->previous_key, &this->current_key, sizeof(camera::params));
            }

            if (this->elapsed_time > 0.0f)
            {
                float inverse_lapsed = 1.0f / this->elapsed_time;

                this->velocity_key.position = vector4::scale(vector4::subtract(this->current_key.position, this->previous_key.position), inverse_lapsed);

                this->velocity_key.direction = vector4::scale(vector4::subtract(this->current_key.direction, this->previous_key.direction), inverse_lapsed);

                this->velocity_key.target = vector4::scale(vector4::subtract(this->current_key.target, this->previous_key.direction), inverse_lapsed);

                this->velocity_key.noise_frequency_1 = (this->current_key.noise_frequency_1 - this->previous_key.noise_frequency_1) * inverse_lapsed;

                this->velocity_key.noise_amplitude_1 = (this->current_key.noise_amplitude_1 - this->previous_key.noise_amplitude_1) * inverse_lapsed;

                this->velocity_key.noise_frequency_2 = (this->current_key.noise_frequency_2 - this->previous_key.noise_frequency_2) * inverse_lapsed;

                this->velocity_key.noise_amplitude_2 = (this->current_key.noise_amplitude_2 - this->previous_key.noise_amplitude_2) * inverse_lapsed;

                this->velocity_key.fade_color = (this->current_key.fade_color - this->previous_key.fade_color) * inverse_lapsed;

                this->velocity_key.target_distance = (this->current_key.target_distance - this->previous_key.target_distance) * inverse_lapsed;

                this->velocity_key.focal_distance = (this->current_key.focal_distance - this->previous_key.focal_distance) * inverse_lapsed;

                this->velocity_key.depth_of_field = (this->current_key.depth_of_field - this->previous_key.depth_of_field) * inverse_lapsed;

                this->velocity_key.dof_falloff = (this->current_key.dof_falloff - this->previous_key.dof_falloff) * inverse_lapsed;

                this->velocity_key.dof_max_intensity = (this->current_key.dof_max_intensity - this->previous_key.dof_max_intensity) * inverse_lapsed;

                this->velocity_key.near_clip = (this->current_key.near_clip - this->previous_key.near_clip) * inverse_lapsed;

                this->velocity_key.far_clip = (this->current_key.far_clip - this->previous_key.far_clip) * inverse_lapsed;

                this->velocity_key.lb_height = (this->current_key.lb_height - this->previous_key.lb_height) * inverse_lapsed;

                this->velocity_key.sim_time_multiplier = (this->current_key.sim_time_multiplier - this->previous_key.sim_time_multiplier) * inverse_lapsed;

                this->velocity_key.horizontal_fov = static_cast<std::uint16_t>(static_cast<std::int32_t>(this->current_key.horizontal_fov - this->previous_key.horizontal_fov) * inverse_lapsed);
            }
        }
    }
}
