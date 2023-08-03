#include <hyperlib/renderer/drawing.hpp>
#include <hyperlib/renderer/lighting.hpp>

namespace hyper
{
    bool lighting::setup_light_context(dynamic_context& context, const shaper_light_rigorous& shaper, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base)
    {
        ::memset(&context, 0, sizeof(dynamic_context));

        if (inst_trs != nullptr)
        {
            lighting::setup_envmap(context, *inst_trs, camera_trs, center);
            lighting::setup_lights(context, shaper, inst_trs->row(3u).as_vector3(), inst_trs, camera_trs, base);

            return true;
        }

        return false;
    }

    bool lighting::clone_light_context(dynamic_context& dst, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base, const dynamic_context& src)
    {
        lighting::rotate_light_context(dst, src, inst_trs);
        lighting::setup_envmap(dst, inst_trs, camera_trs, center);

        return true;
    }

    void lighting::rotate_light_context(dynamic_context& dst, const dynamic_context& src, const matrix4x4& inst_trs)
    {
        matrix4x4 rotation;

        irradiance radiance;

        math::invert_rotation(rotation, rotation);

        dst.type = src.type;
        dst.padv3 = src.padv3;
        dst.local_color = src.local_color;
        dst.local_eye = src.local_eye;
        dst.envmap_pos = src.envmap_pos;
        dst.idk_vector4 = src.idk_vector4;
        dst.envmap_rotation = src.envmap_rotation;

        vector3 column_x;
        vector3 column_y;
        vector3 column_z;

        column_x.x = src.local_direction.m11;
        column_x.y = src.local_direction.m21;
        column_x.z = src.local_direction.m31;

        column_y.x = src.local_direction.m12;
        column_y.y = src.local_direction.m22;
        column_y.z = src.local_direction.m32;

        column_z.x = src.local_direction.m13;
        column_z.y = src.local_direction.m23;
        column_z.z = src.local_direction.m33;

        math::transform_vector(rotation, column_x);
        math::transform_vector(rotation, column_y);
        math::transform_vector(rotation, column_z);

        dst.local_direction.m11 = column_x.x;
        dst.local_direction.m21 = column_x.y;
        dst.local_direction.m31 = column_x.z;
        dst.local_direction.m41 = 0.0f;

        dst.local_direction.m12 = column_y.x;
        dst.local_direction.m22 = column_y.y;
        dst.local_direction.m32 = column_y.z;
        dst.local_direction.m42 = 0.0f;

        dst.local_direction.m13 = column_z.x;
        dst.local_direction.m23 = column_z.y;
        dst.local_direction.m33 = column_z.z;
        dst.local_direction.m43 = 0.0f;

        dst.local_direction.m14 = 0.0f;
        dst.local_direction.m24 = 0.0f;
        dst.local_direction.m34 = 0.0f;
        dst.local_direction.m44 = 0.0f;

        ::memcpy(&radiance, &src.harmonics, sizeof(irradiance));

        // #TODO irradiance stuff here

        ::memcpy(&dst.harmonics, &radiance, sizeof(irradiance));
    }

    bool lighting::setup_envmap(dynamic_context& context, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center)
    {
        matrix4x4 rotation;

        math::invert_rotation(inst_trs, rotation);

        if (center != nullptr)
        {
            vector3 to_camera_dir = (*center - inst_trs.row(3u).as_vector3()).normalized();

            const float distance = 4.0f; // yeah blackbox, that definitely makes sense (0x00739853)

            context.envmap_pos.x = to_camera_dir.x * distance;
            context.envmap_pos.y = to_camera_dir.y * distance;
            context.envmap_pos.z = to_camera_dir.z * distance;
            context.envmap_pos.w = 1.0f;
        }

        if (camera_trs != nullptr)
        {
            math::multiply_matrix(inst_trs, *camera_trs, context.envmap_rotation);

            context.envmap_rotation.m41 = context.envmap_pos.x;
            context.envmap_rotation.m42 = context.envmap_pos.y;
            context.envmap_rotation.m43 = context.envmap_pos.z;

            context.envmap_rotation.m14 = 0.0f;
            context.envmap_rotation.m24 = 0.0f;
            context.envmap_rotation.m34 = 0.0f;
            context.envmap_rotation.m44 = 0.0f;
        }

        return true;
    }

    void lighting::setup_lights(dynamic_context& context, const shaper_light_rigorous& shaper, const vector3& inst_pos, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const view::base* base)
    {
        if (base != nullptr && base->camera != nullptr)
        {
            camera_trs = &base->camera->current_key.matrix;
        }

        matrix4x4 rotation;

        math::invert_rotation(*camera_trs, rotation);

        color colors[19];
        vector3 true_directions[19];
        vector3 norm_directions[19];

        for (std::uint32_t i = 0; i < std::size(shaper.lights); ++i)
        {
            const shaper_light& light = shaper.lights[i];

            float scale = light.color.a;

            if (scale != 0.0f && time_of_day::instance != nullptr)
            {
                switch (light.mode)
                {
                    case shaper_light_mode::world_space:
                        lighting::make_world_space_light_direction(true_directions[i], light);
                        break;

                    case shaper_light_mode::camera_space:
                        lighting::make_camera_space_light_direction(true_directions[i], light, rotation);
                        break;

                    case shaper_light_mode::sun_direction:
                        lighting::make_sun_direction_light_direction(true_directions[i], light);
                        break;

                    case shaper_light_mode::inverse_sun_direction:
                        lighting::make_inverse_sun_direction_light_direction(true_directions[i], light);
                        break;

                    case shaper_light_mode::world_position:
                        lighting::make_world_position_light_direction(true_directions[i], shaper, inst_pos);
                        break;
                }
            }

            colors[i].r = light.color.r * scale * 255.0f;
            colors[i].g = light.color.g * scale * 255.0f;
            colors[i].b = light.color.b * scale * 255.0f;
            colors[i].a = 0.0f;
        }

        // #TODO
    }

    void lighting::make_world_space_light_direction(vector3& direction, const shaper_light& light)
    {
        lighting::make_light_direction_from_angles(direction, 180.0f + light.theta, 90.0f - light.phi, 1.0f);
    }

    void lighting::make_camera_space_light_direction(vector3& direction, const shaper_light& light, const matrix4x4& rotation)
    {
        lighting::make_light_direction_from_angles(direction, 180.0f + light.theta, 90.0f - light.phi, 1.0f);

        space_settings(space_axis::ny, space_axis::nz, space_axis::px).swap(direction);

        math::transform_vector(rotation, direction);
    }

    void lighting::make_sun_direction_light_direction(vector3& direction, const shaper_light& light)
    {
        lighting::make_light_direction_from_sun(direction, time_of_day::instance->sun_direction.as_vector3());

        direction.x = direction.x * 0.0054931641f + light.theta;
        direction.y = direction.y * 0.0054931641f + light.phi + 90.0f;

        lighting::make_light_direction_from_angles(direction, 180.0f + direction.x, 90.0f - direction.y, 1.0f);
    }

    void lighting::make_inverse_sun_direction_light_direction(vector3& direction, const shaper_light& light)
    {
        vector3 inverse = -time_of_day::instance->sun_direction.as_vector3();

        lighting::make_light_direction_from_sun(direction, inverse);

        direction.x = direction.x * 0.0054931641f + light.theta;
        direction.y = direction.y * 0.0054931641f + light.phi + 90.0f;

        lighting::make_light_direction_from_angles(direction, 180.0f + direction.x, 90.0f - direction.y, 1.0f);
    }

    void lighting::make_world_position_light_direction(vector3& direction, const shaper_light_rigorous& shaper, const vector3& inst_pos)
    {
        direction = shaper.position - inst_pos;
    }

    void lighting::make_light_direction_from_angles(vector3& direction, float theta, float phi, float scale)
    {
        float theta_sin, theta_cos;
        float phi_sin, phi_cos;

        math::sincos(math::to_uint16_degrees(theta), theta_sin, theta_cos);
        math::sincos(math::to_uint16_degrees(phi), phi_sin, phi_cos);

        direction.x = phi_sin * scale * theta_cos;
        direction.y = phi_sin * scale * theta_sin;
        direction.z = phi_cos * scale;
    }

    void lighting::make_light_direction_from_sun(vector3& direction, const vector3& sun)
    {
        direction.z = sun.magnitude();
        direction.x = math::arc_tan(sun.x, sun.y);
        direction.y = static_cast<std::uint16_t>(0x4000 - math::arc_sin(sun.z / direction.z));
    }
}
