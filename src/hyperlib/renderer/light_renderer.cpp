#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/time_of_day.hpp>
#include <hyperlib/renderer/light_renderer.hpp>

#pragma warning (disable : 6001)

namespace hyper
{
    void light_renderer::reset_light_context(light::context::dynamic& context)
    {
        ::memset(&context, 0, sizeof(light::context::dynamic));
    }

    bool light_renderer::setup_light_context(light::context::dynamic& context, const light::shaper_rigorous& shaper, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base)
    {
        light_renderer::reset_light_context(context);

        if (inst_trs != nullptr)
        {
            light_renderer::setup_envmap(context, *inst_trs, camera_trs, center);
            light_renderer::setup_lights(context, shaper, inst_trs->row(3u).as_vector3(), inst_trs, camera_trs, base);

            return true;
        }

        return false;
    }

    bool light_renderer::clone_light_context(light::context::dynamic& dst, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center, const view::base* base, const light::context::dynamic& src)
    {
        light_renderer::rotate_light_context(dst, src, inst_trs);

        light_renderer::setup_envmap(dst, inst_trs, camera_trs, center);

        return true;
    }

    void light_renderer::rotate_light_context(light::context::dynamic& dst, const light::context::dynamic& src, const matrix4x4& inst_trs)
    {
        matrix4x4 rotation;

        math::invert_rotation(inst_trs, rotation);

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
        vector3 column_w;

        column_x.x = src.local_direction.m11;
        column_x.y = src.local_direction.m21;
        column_x.z = src.local_direction.m31;

        column_y.x = src.local_direction.m12;
        column_y.y = src.local_direction.m22;
        column_y.z = src.local_direction.m32;

        column_z.x = src.local_direction.m13;
        column_z.y = src.local_direction.m23;
        column_z.z = src.local_direction.m33;

        column_w.x = src.local_direction.m14;
        column_w.y = src.local_direction.m24;
        column_w.z = src.local_direction.m34;

        math::transform_vector(rotation, column_x);
        math::transform_vector(rotation, column_y);
        math::transform_vector(rotation, column_z);
        math::transform_vector(rotation, column_w);

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

        dst.local_direction.m14 = column_w.x;
        dst.local_direction.m24 = column_w.y;
        dst.local_direction.m34 = column_w.z;
        dst.local_direction.m44 = 0.0f;

        ::memcpy(&dst.harmonics, &src.harmonics, sizeof(src.harmonics));

        irradiance& radiance = *reinterpret_cast<irradiance*>(&dst.harmonics);

        light_renderer::transform_irradiance(radiance, rotation);
    }

    bool light_renderer::setup_envmap(light::context::dynamic& context, const matrix4x4& inst_trs, const matrix4x4* camera_trs, const vector3* center)
    {
        matrix4x4 rotation;

        math::invert_rotation(inst_trs, rotation);

        if (center != nullptr)
        {
            vector3 to_camera_dir = (*center - inst_trs.row(3u).as_vector3());

            math::transform_vector(rotation, to_camera_dir);

            to_camera_dir = to_camera_dir.normalized();

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

    void light_renderer::setup_lights(light::context::dynamic& context, const light::shaper_rigorous& shaper, const vector3& inst_pos, const matrix4x4* inst_trs, const matrix4x4* camera_trs, const view::base* base)
    {
        if (base != nullptr && base->camera != nullptr)
        {
            camera_trs = &base->camera->current_key.view_matrix;
        }

        matrix4x4 rotation;

        math::invert_rotation(*camera_trs, rotation);

        light_renderer::query query;
        color colors[19];
        vector3 truest_directions[19];
        vector3 normal_directions[19];

        const std::uint32_t light_count = std::size(shaper.lights);

        for (std::uint32_t i = 0u; i < light_count; ++i)
        {
            const light::shaper& light = shaper.lights[i];

            float scale = light.color.a;

            if (scale != 0.0f && time_of_day::instance != nullptr)
            {
                switch (light.mode)
                {
                case light::shaper::light_mode::world_space:
                    light_renderer::make_world_space_light_direction(truest_directions[i], light);
                    break;

                case light::shaper::light_mode::camera_space:
                    light_renderer::make_camera_space_light_direction(truest_directions[i], light, rotation);
                    break;

                case light::shaper::light_mode::sun_direction:
                    light_renderer::make_sun_direction_light_direction(truest_directions[i], light);
                    break;

                case light::shaper::light_mode::inverse_sun_direction:
                    light_renderer::make_inverse_sun_direction_light_direction(truest_directions[i], light);
                    break;

                case light::shaper::light_mode::world_position:
                    light_renderer::make_world_position_light_direction(truest_directions[i], shaper, inst_pos);
                    break;
                }

                colors[i].r = light.color.r * scale * 255.0f;
                colors[i].g = light.color.g * scale * 255.0f;
                colors[i].b = light.color.b * scale * 255.0f;
                colors[i].a = 0.0f;
            }
            else
            {
                truest_directions[i] = vector3::zero();
                colors[i] = color::clear();
            }
        }

        std::uint32_t light_hits = light_renderer::query_light_database(query, inst_pos);

        if (light_hits + light_count > std::size(colors))
        {
            light_hits = std::size(colors) - light_count;
        }

        std::uint32_t total_lights = light_count + light_hits;

        for (std::uint32_t i = 0u, k = light_count; i < light_hits; ++i, ++k)
        {
            const light::instance* light = query.lights[i];

            float influence = query.influences[i] * 0.75f;

            truest_directions[k] = light->position - inst_pos;

            if (light->type == light::type::spot || light->type == light::type::spot_headlight)
            {
                float sin = math::sin(light->far_end * 0.5f + 1.5707964f);
                float dot = vector3::dot((inst_pos - light->position).normalized(), light->direction);

                if (dot <= sin)
                {
                    influence = 0.0f;
                }
                else
                {
                    influence *= (dot - sin) / (1.0f - sin);
                }
            }

            colors[k].r = static_cast<float>(light->color.r) * influence;
            colors[k].g = static_cast<float>(light->color.g) * influence;
            colors[k].b = static_cast<float>(light->color.b) * influence;
        }

        if (inst_trs != nullptr)
        {
            matrix4x4 inverted_trs;

            math::invert_rotation(*inst_trs, inverted_trs);

            for (std::uint32_t i = 0u; i < total_lights; ++i)
            {
                math::transform_vector(inverted_trs, truest_directions[i]);
            }
        }

        for (std::uint32_t i = 0u; i < total_lights; ++i)
        {
            normal_directions[i] = truest_directions[i].normalized();

            colors[i] *= 0.0039215689f; // divide by 255.0f
        }

        context.local_direction.m11 = normal_directions[0].x;
        context.local_direction.m21 = normal_directions[0].y;
        context.local_direction.m31 = normal_directions[0].z;
        context.local_direction.m12 = normal_directions[1].x;
        context.local_direction.m22 = normal_directions[1].y;
        context.local_direction.m32 = normal_directions[1].z;
        context.local_direction.m13 = normal_directions[2].x;
        context.local_direction.m23 = normal_directions[2].y;
        context.local_direction.m33 = normal_directions[2].z;
        context.local_direction.m14 = normal_directions[3].x;
        context.local_direction.m24 = normal_directions[3].y;
        context.local_direction.m34 = normal_directions[3].z;

        context.local_color.m11 = colors[0].r;
        context.local_color.m12 = colors[0].g;
        context.local_color.m13 = colors[0].b;
        context.local_color.m21 = colors[1].r;
        context.local_color.m22 = colors[1].g;
        context.local_color.m23 = colors[1].b;
        context.local_color.m31 = colors[2].r;
        context.local_color.m32 = colors[2].g;
        context.local_color.m33 = colors[2].b;
        context.local_color.m41 = colors[3].r;
        context.local_color.m42 = colors[3].g;
        context.local_color.m43 = colors[3].b;

        ::memset(context.harmonics, 0, sizeof(context.harmonics));

        irradiance& radiance = *reinterpret_cast<irradiance*>(&context.harmonics);

        for (std::uint32_t i = 0u; i < total_lights; ++i)
        {
            color& light_color = colors[i];

            light_color.a = light_color.r * 0.3f + light_color.g * 0.5f + light_color.b * 0.2f;

            light_renderer::update_irradiance(radiance, normal_directions[i], light_color);
        }

        light_renderer::normalize_irradiance(radiance, light_renderer::normalize_rgb_irradiance, light_renderer::scale_rgb_irradiance, light_renderer::normalize_alpha_irradiance, light_renderer::scale_alpha_irradiance);
    }

    auto light_renderer::query_light_database(light_renderer::query& query, const vector3& position) -> std::uint32_t
    {
        constexpr std::uint32_t max_lights_allowed = 10u;

        light_renderer::query_helper helper;

        query.lights[0] = nullptr;
        query.influences[0] = 0.0f;
        query.ambience = color::clear();

        helper.root_ambient_colour = &helper.ambient_colors[0];
        helper.current_ambient_colour = &helper.ambient_colors[1];
        helper.ambient_sentinel = std::end(helper.ambient_colors);
        helper.total_ambient_influence = 0.0f;
        helper.smallest_root_ambient_distance = std::numeric_limits<float>::infinity();
        helper.total_light_influence = 0.0f;
        helper.light_hits = 0u;
        helper.ambient_colors[0] = color::black();

        for (const light::pack* pack = light::pack::list.begin(); pack != light::pack::list.end(); pack = pack->next())
        {
            if (helper.light_hits < max_lights_allowed && helper.current_ambient_colour < helper.ambient_sentinel)
            {
                const aabb_node* node = pack->tree->query_leaf(position);

                if (node != nullptr)
                {
                    std::uint32_t count = static_cast<std::uint32_t>(-node->child_count);

                    for (std::uint32_t i = 0u; i < count; ++i)
                    {
                        light_renderer::query_light_internal(query, helper, pack->lights[node->children[i]], position);

                        if (helper.light_hits >= max_lights_allowed)
                        {
                            break;
                        }
                    }
                }
            }
        }

        for (const light::dynamic_pack* pack = light::dynamic_pack::list.begin(); pack != light::dynamic_pack::list.end(); pack = pack->next())
        {
            if (pack->enabled)
            {
                for (const light::dynamic* light = pack->lights.begin(); light != pack->lights.end(); light = light->next())
                {
                    light_renderer::query_light_internal(query, helper, *light, position);
                }
            }
        }

        float total_ambient_influence = helper.total_ambient_influence;
        float invrs_ambient_influence = 1.0f - total_ambient_influence;

        if (total_ambient_influence > 1.0f)
        {
            total_ambient_influence = 1.0f / total_ambient_influence;
            invrs_ambient_influence = 0.0f;
        }

        query.ambience = *helper.root_ambient_colour * invrs_ambient_influence;

        for (const color* i = &helper.ambient_colors[1]; i != helper.current_ambient_colour; ++i)
        {
            query.ambience += *i * total_ambient_influence;
        }

        return helper.light_hits;
    }

    void light_renderer::make_world_space_light_direction(vector3& direction, const light::shaper& light)
    {
        light_renderer::make_light_direction_from_angles(direction, 180.0f + light.theta, 90.0f - light.phi, 1.0f);
    }

    void light_renderer::make_camera_space_light_direction(vector3& direction, const light::shaper& light, const matrix4x4& rotation)
    {
        light_renderer::make_light_direction_from_angles(direction, 180.0f + light.theta, 90.0f - light.phi, 1.0f);

        space_settings(space_axis::ny, space_axis::nz, space_axis::px).swap(direction);

        math::transform_vector(rotation, direction);
    }

    void light_renderer::make_sun_direction_light_direction(vector3& direction, const light::shaper& light)
    {
        light_renderer::make_light_direction_from_sun(direction, time_of_day::instance->sun_direction.as_vector3());

        direction.x = direction.x * 0.0054931641f + light.theta;
        direction.y = direction.y * 0.0054931641f + light.phi + 90.0f;

        light_renderer::make_light_direction_from_angles(direction, 180.0f + direction.x, 90.0f - direction.y, 1.0f);
    }

    void light_renderer::make_inverse_sun_direction_light_direction(vector3& direction, const light::shaper& light)
    {
        vector3 inverse = -time_of_day::instance->sun_direction.as_vector3();

        light_renderer::make_light_direction_from_sun(direction, inverse);

        direction.x = direction.x * 0.0054931641f + light.theta;
        direction.y = direction.y * 0.0054931641f + light.phi + 90.0f;

        light_renderer::make_light_direction_from_angles(direction, 180.0f + direction.x, 90.0f - direction.y, 1.0f);
    }

    void light_renderer::make_world_position_light_direction(vector3& direction, const light::shaper_rigorous& shaper, const vector3& inst_pos)
    {
        direction = shaper.position - inst_pos;
    }

    void light_renderer::make_light_direction_from_angles(vector3& direction, float theta, float phi, float scale)
    {
        float theta_sin, theta_cos;
        float phi_sin, phi_cos;

        math::sincos(math::to_uint16_degrees(theta), theta_sin, theta_cos);
        math::sincos(math::to_uint16_degrees(phi), phi_sin, phi_cos);

        direction.x = phi_sin * scale * theta_cos;
        direction.y = phi_sin * scale * theta_sin;
        direction.z = phi_cos * scale;
    }

    void light_renderer::make_light_direction_from_sun(vector3& direction, const vector3& sun)
    {
        direction.z = sun.magnitude();
        direction.x = math::arc_tan(sun.x, sun.y);
        direction.y = static_cast<std::uint16_t>(0x4000 - math::arc_sin(sun.z / direction.z));
    }

    void light_renderer::query_light_internal(light_renderer::query& query, light_renderer::query_helper& helper, const light::instance& light, const vector3& position)
    {
        if (light.state == light::state::on)
        {
            float distance = (position - light.position).sqr_magnitude();
            float radiisqr = light.radius * light.radius;

            if (distance < radiisqr || light_renderer::light_query_ignore_distance)
            {
                float farstart2 = light.far_start * light.far_start;
                float influence = 1.0f;

                if (distance > farstart2)
                {
                    influence = 1.00f - (distance - farstart2) / (radiisqr - farstart2);
                }

                if (light.type == light::type::ambient)
                {
                    color light_color = color(light.color.r, light.color.g, light.color.b, light.intensity);

                    if (light.attenuation == light::attenuation::far)
                    {
                        if (helper.current_ambient_colour < helper.ambient_sentinel && influence > 0.0f)
                        {
                            helper.current_ambient_colour->r = influence * static_cast<float>(light.color.r);
                            helper.current_ambient_colour->g = influence * static_cast<float>(light.color.g);
                            helper.current_ambient_colour->b = influence * static_cast<float>(light.color.b);
                            helper.current_ambient_colour->a = influence * light.intensity;
                            helper.total_ambient_influence += influence;
                            helper.current_ambient_colour++;
                        }
                    }
                    else if (distance < helper.smallest_root_ambient_distance)
                    {
                        helper.root_ambient_colour->r = static_cast<float>(light.color.r);
                        helper.root_ambient_colour->g = static_cast<float>(light.color.g);
                        helper.root_ambient_colour->b = static_cast<float>(light.color.b);
                        helper.root_ambient_colour->a = light.intensity;
                        helper.smallest_root_ambient_distance = distance;
                    }
                }
                else
                {
                    if (light.type == light::type::omni ||
                        light.type == light::type::spot ||
                        light.type == light::type::spot_headlight)
                    {
                        if (helper.light_hits < std::size(query.lights))
                        {
                            query.lights[helper.light_hits] = &light;
                            query.influences[helper.light_hits] = influence;
                            helper.light_hits++;
                        }
                    }
                }
            }
        }
    }

    void light_renderer::update_irradiance(light_renderer::irradiance& radiance, const vector3& direction, const color& light_color)
    {
        light_renderer::update_irradiance(radiance, light_color, 1.0f, direction);

        if (light_renderer::enable_echo_irradiance)
        {
            color inverted_color = light_color * -0.05f;

            vector3 inverted_dir = vector3(-direction.x, -direction.y, -direction.z);

            light_renderer::update_irradiance(radiance, inverted_color, 1.0f, inverted_dir);
        }
    }

    void light_renderer::update_irradiance(light_renderer::irradiance& radiance, const color& light_color, float scale, const vector3& direction)
    {
        float y_mod = direction.z * direction.z * 3.0f - 1.0f;
        float p_sqr = direction.x * direction.x - direction.y * direction.y;

        for (std::uint32_t i = 0u; i < 4u; ++i)
        {
            float current = light_color[i] * scale;

            float scalar_1 = current * 0.488603f * 1.0233279f;
            float scalar_2 = current * 1.092548f;

            float scaled_x = scalar_1 * direction.x;
            float scaled_y = scalar_1 * direction.y;
            float scaled_z = scalar_1 * direction.z;

            float scaled_xy = scalar_2 * direction.x * direction.y * 0.85808599f;
            float scaled_yz = scalar_2 * direction.y * direction.z * 0.42904299f;
            float scaled_zx = scalar_2 * direction.z * direction.x * 0.85808599f;

            float a_coeff = current * 0.31539199f * y_mod;
            float b_coeff = current * 0.54627401f * p_sqr * 0.42904299f;

            radiance.coeffs[0][i] += current * 0.28209499f * 0.88622701f - a_coeff * 0.24770799f;
            radiance.coeffs[1][i] += b_coeff;
            radiance.coeffs[2][i] -= b_coeff;
            radiance.coeffs[3][i] += a_coeff * 0.74312502f;
            radiance.coeffs[4][i] += scaled_xy;
            radiance.coeffs[5][i] += scaled_zx;
            radiance.coeffs[6][i] += scaled_yz;
            radiance.coeffs[7][i] += scaled_x;
            radiance.coeffs[8][i] += scaled_y;
            radiance.coeffs[9][i] += scaled_z;
        }
    }

    void light_renderer::normalize_irradiance(light_renderer::irradiance& radiance, bool normalize_rgb, float scale_rgb, bool normalize_alpha, float scale_alpha)
    {
        if (normalize_rgb)
        {
            float sum_r = 0.0f;
            float sum_g = 0.0f;
            float sum_b = 0.0f;

            for (std::uint32_t i = 0u; i < std::size(radiance.coeffs); ++i)
            {
                sum_r += radiance.coeffs[i][0] * radiance.coeffs[i][0];
                sum_g += radiance.coeffs[i][1] * radiance.coeffs[i][1];
                sum_b += radiance.coeffs[i][2] * radiance.coeffs[i][2];
            }

            float max = math::max(math::max(sum_r, sum_g), sum_b);

            if (max > 0.0f)
            {
                float inverse = scale_rgb / ::sqrt(max);

                for (std::uint32_t i = 0u; i < std::size(radiance.coeffs); ++i)
                {
                    radiance.coeffs[i][0] *= inverse;
                    radiance.coeffs[i][1] *= inverse;
                    radiance.coeffs[i][2] *= inverse;
                }
            }
        }

        if (normalize_alpha)
        {
            float sum_a = 0.0f;

            for (std::uint32_t i = 0u; i < std::size(radiance.coeffs); ++i)
            {
                sum_a += radiance.coeffs[i][3] * radiance.coeffs[i][3];
            }

            if (sum_a > 0.0f)
            {
                float inverse = scale_alpha / ::sqrt(sum_a);

                for (std::uint32_t i = 0u; i < std::size(radiance.coeffs); ++i)
                {
                    radiance.coeffs[i][3] *= inverse;
                }
            }
        }
    }

    void light_renderer::transform_irradiance(light_renderer::irradiance& radiance, const matrix4x4& rotation)
    {
        for (std::uint32_t i = 0u; i < 4u; ++i)
        {
            float coeff_1 = radiance.coeffs[1][i];
            float coeff_2 = radiance.coeffs[2][i];
            float coeff_3 = radiance.coeffs[3][i];
            float coeff_4 = radiance.coeffs[4][i];
            float coeff_5 = radiance.coeffs[5][i];
            float coeff_6 = radiance.coeffs[6][i];
            float coeff_7 = radiance.coeffs[7][i];
            float coeff_8 = radiance.coeffs[8][i];
            float coeff_9 = radiance.coeffs[9][i];

            radiance.coeffs[1][i] =
                rotation.m11 * (coeff_1 * rotation.m11) +
                rotation.m21 * (coeff_4 * rotation.m11 + coeff_2 * rotation.m21) +
                rotation.m31 * (coeff_5 * rotation.m11 + coeff_6 * rotation.m21 + coeff_3 * rotation.m31);

            radiance.coeffs[2][i] =
                rotation.m12 * (coeff_1 * rotation.m12) +
                rotation.m22 * (coeff_4 * rotation.m12 + coeff_2 * rotation.m22) +
                rotation.m32 * (coeff_5 * rotation.m12 + coeff_6 * rotation.m22 + coeff_3 * rotation.m32);

            radiance.coeffs[3][i] =
                rotation.m13 * (coeff_1 * rotation.m13) +
                rotation.m23 * (coeff_4 * rotation.m13 + coeff_2 * rotation.m23) +
                rotation.m33 * (coeff_5 * rotation.m13 + coeff_6 * rotation.m23 + coeff_3 * rotation.m33);

            radiance.coeffs[4][i] =
                coeff_1 * rotation.m11 * rotation.m12 +
                coeff_1 * rotation.m11 * rotation.m12 +
                coeff_2 * rotation.m21 * rotation.m22 +
                coeff_2 * rotation.m21 * rotation.m22 +
                coeff_3 * rotation.m31 * rotation.m32 +
                coeff_3 * rotation.m31 * rotation.m32 +
                coeff_4 * rotation.m11 * rotation.m22 +
                coeff_4 * rotation.m21 * rotation.m12 +
                coeff_5 * rotation.m11 * rotation.m32 +
                coeff_5 * rotation.m12 * rotation.m31 +
                coeff_6 * rotation.m21 * rotation.m32 +
                coeff_6 * rotation.m22 * rotation.m31;

            radiance.coeffs[5][i] =
                coeff_1 * rotation.m13 * rotation.m11 +
                coeff_1 * rotation.m13 * rotation.m11 +
                coeff_2 * rotation.m23 * rotation.m21 +
                coeff_2 * rotation.m23 * rotation.m21 +
                coeff_3 * rotation.m33 * rotation.m31 +
                coeff_3 * rotation.m33 * rotation.m31 +
                coeff_4 * rotation.m11 * rotation.m23 +
                coeff_4 * rotation.m21 * rotation.m13 +
                coeff_5 * rotation.m11 * rotation.m33 +
                coeff_5 * rotation.m31 * rotation.m13 +
                coeff_6 * rotation.m21 * rotation.m33 +
                coeff_6 * rotation.m31 * rotation.m23;

            radiance.coeffs[6][i] =
                coeff_1 * rotation.m12 * rotation.m13 +
                coeff_1 * rotation.m12 * rotation.m13 +
                coeff_2 * rotation.m22 * rotation.m23 +
                coeff_2 * rotation.m22 * rotation.m23 +
                coeff_3 * rotation.m32 * rotation.m33 +
                coeff_3 * rotation.m32 * rotation.m33 +
                coeff_4 * rotation.m12 * rotation.m23 +
                coeff_4 * rotation.m22 * rotation.m13 +
                coeff_5 * rotation.m12 * rotation.m33 +
                coeff_5 * rotation.m32 * rotation.m13 +
                coeff_6 * rotation.m22 * rotation.m33 +
                coeff_6 * rotation.m32 * rotation.m23;

            radiance.coeffs[7][i] =
                coeff_7 * rotation.m11 +
                coeff_8 * rotation.m21 +
                coeff_9 * rotation.m31;

            radiance.coeffs[8][i] =
                coeff_7 * rotation.m12 +
                coeff_8 * rotation.m22 +
                coeff_9 * rotation.m32;

            radiance.coeffs[9][i] =
                coeff_7 * rotation.m13 +
                coeff_8 * rotation.m23 +
                coeff_9 * rotation.m33;
        }
    }
}
