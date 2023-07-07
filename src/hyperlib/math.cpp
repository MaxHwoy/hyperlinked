#include <hyperlib/math.hpp>

namespace hyper
{
    auto vector2::normalized() const -> vector2
    {
        float magnitude = this->magnitude();

        if (math::approximately(magnitude, 0.0f))
        {
            return { 0.0f, 0.0f };
        }

        magnitude = 1.0f / magnitude;

        return { this->x * magnitude, this->y * magnitude };
    }

    auto vector3::normalized() const -> vector3
    {
        float magnitude = this->magnitude();

        if (math::approximately(magnitude, 0.0f))
        {
            return { 0.0f, 0.0f, 0.0f };
        }

        magnitude = 1.0f / magnitude;

        return { this->x * magnitude, this->y * magnitude, this->z * magnitude };
    }

    bool math::approximately(float a, float b)
    {
        return ::fabs(b - a) < math::max(1e-06f * math::max(::fabs(a), ::fabs(b)), math::epsilon * 8.0f);
    }

    void math::transform_point(const matrix4x4& trs, vector3& point)
    {
        float x = point.x * trs.m11 + point.y * trs.m21 + point.z * trs.m31 + trs.m41;
        float y = point.x * trs.m12 + point.y * trs.m22 + point.z * trs.m32 + trs.m42;
        float z = point.x * trs.m13 + point.y * trs.m23 + point.z * trs.m33 + trs.m43;

        point.x = x;
        point.y = y;
        point.z = z;
    }

    void math::transform_bound(const matrix4x4& trs, vector3& min, vector3& max)
    {
        // heavy inline

        vector3 real_min { trs.m41, trs.m42, trs.m43 };
        vector3 real_max { trs.m41, trs.m42, trs.m43 };

        {
            {
                float a = trs.m11 * min.x;
                float b = trs.m11 * max.x;

                if (a < b)
                {
                    real_min.x += a;
                    real_max.x += b;
                }
                else
                {
                    real_min.x += b;
                    real_max.x += a;
                }
            }
            {
                float a = trs.m21 * min.x;
                float b = trs.m21 * max.x;

                if (a < b)
                {
                    real_min.y += a;
                    real_max.y += b;
                }
                else
                {
                    real_min.y += b;
                    real_max.y += a;
                }
            }
            {
                float a = trs.m31 * min.x;
                float b = trs.m31 * max.x;

                if (a < b)
                {
                    real_min.z += a;
                    real_max.z += b;
                }
                else
                {
                    real_min.z += b;
                    real_max.z += a;
                }
            }
        }

        {
            {
                float a = trs.m12 * min.y;
                float b = trs.m12 * max.y;

                if (a < b)
                {
                    real_min.x += a;
                    real_max.x += b;
                }
                else
                {
                    real_min.x += b;
                    real_max.x += a;
                }
            }
            {
                float a = trs.m22 * min.y;
                float b = trs.m22 * max.y;

                if (a < b)
                {
                    real_min.y += a;
                    real_max.y += b;
                }
                else
                {
                    real_min.y += b;
                    real_max.y += a;
                }
            }
            {
                float a = trs.m32 * min.y;
                float b = trs.m32 * max.y;

                if (a < b)
                {
                    real_min.z += a;
                    real_max.z += b;
                }
                else
                {
                    real_min.z += b;
                    real_max.z += a;
                }
            }
        }

        {
            {
                float a = trs.m13 * min.z;
                float b = trs.m13 * max.z;

                if (a < b)
                {
                    real_min.x += a;
                    real_max.x += b;
                }
                else
                {
                    real_min.x += b;
                    real_max.x += a;
                }
            }
            {
                float a = trs.m23 * min.z;
                float b = trs.m23 * max.z;

                if (a < b)
                {
                    real_min.y += a;
                    real_max.y += b;
                }
                else
                {
                    real_min.y += b;
                    real_max.y += a;
                }
            }
            {
                float a = trs.m33 * min.z;
                float b = trs.m33 * max.z;

                if (a < b)
                {
                    real_min.z += a;
                    real_max.z += b;
                }
                else
                {
                    real_min.z += b;
                    real_max.z += a;
                }
            }
        }

        min.x = real_min.x;
        min.y = real_min.y;
        min.z = real_min.z;

        max.x = real_max.x;
        max.y = real_max.y;
        max.z = real_max.z;
    }

    bool math::is_in_bounding_box(const vector2& point, const vector2& min, const vector2& max)
    {
        return min.x <= point.x && point.x <= max.x
            && min.y <= point.y && point.y <= max.y;
    }

    bool math::is_in_bounding_box(const vector3& point, const vector3& min, const vector3& max)
    {
        return min.x <= point.x && point.x <= max.x
            && min.y <= point.y && point.y <= max.y
            && min.z <= point.z && point.z <= max.z;
    }

    bool math::is_in_bounding_box(const vector2& point, const vector2& min, const vector2& max, float extent)
    {
        return min.x - extent <= point.x && point.x <= max.x + extent
            && min.y - extent <= point.y && point.y <= max.y + extent;
    }

    bool math::is_in_bounding_box(const vector3& point, const vector3& min, const vector3& max, float extent)
    {
        return min.x - extent <= point.x && point.x <= max.x + extent
            && min.y - extent <= point.y && point.y <= max.y + extent
            && min.z - extent <= point.z && point.z <= max.z + extent;
    }

    bool math::is_point_in_polygon(const vector2& point, const vector2* polygon, std::uint32_t point_count)
    {
        bool result = false;

        std::uint32_t j = point_count - 1u;

        for (std::uint32_t i = 0u; i < point_count; ++i)
        {
            const vector2& point_i = polygon[i];
            const vector2& point_j = polygon[j];

            if ((point_i.y < point.y && point_j.y >= point.y) || (point_j.y < point.y && point_i.y >= point.y))
            {
                if (point_i.x + (point.y - point_i.y) / (point_j.y - point_i.y) * (point_j.x - point_i.x) < point.x)
                {
                    result = !result;
                }
            }

            j = i;
        }

        return result;
    }

    bool math::is_point_in_triangle(const vector3& point, const vector3& p1, const vector3& p2, const vector3& p3)
    {
        float a1 = (point.x - p2.x) * (p1.z - p2.z) - (p1.x - p2.x) * (point.z - p2.z);
        float a2 = (point.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (point.z - p3.z);
        float a3 = (point.x - p1.x) * (p3.z - p1.z) - (p3.x - p1.x) * (point.z - p1.z);

        bool has_neg = (a1 < 0) || (a2 < 0) || (a3 < 0);
        bool has_pos = (a1 > 0) || (a2 > 0) || (a3 > 0);

        return !(has_neg && has_pos);
    }

    bool math::get_triangle_height(const vector2& at, const vector3& p1, const vector3& p2, const vector3& p3)
    {
        vector3 cross = vector3::cross(p3 - p1, p2 - p1).normalized();

        float arg1 = cross.x * at.x + cross.z * at.y;
        float arg2 = cross.x * p1.x + cross.y * p1.y + cross.z * p1.z;

        return (arg2 - arg1) / cross.y;
    }

    auto math::distance_to_line(const vector2& point, const vector2& p1, const vector2& p2) -> float
    {
        vector2 p01 = point - p1;
        vector2 p21 = p2 - p1;

        float magnitude = p21.magnitude();

        if (math::approximately(magnitude, 0.0f))
        {
            p21.x = 0.0f;
            p21.y = 0.0f;
        }
        else
        {
            p21.x /= magnitude;
            p21.y /= magnitude;
        }

        float c_x = +p01.y * p21.y + p01.x * p21.x;
        float c_y = -p21.x * p01.y + p01.x * p21.y;

        if (c_x < 0.0f)
        {
            return ::sqrtf(c_x * c_x + c_y * c_y);
        }

        if (c_x <= magnitude)
        {
            return ::fabs(c_y);
        }

        return ::sqrtf((c_x - magnitude) * (c_x - magnitude) + c_y * c_y);
    }
}
