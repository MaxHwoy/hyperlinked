#include <hyperlib/math.hpp>
#include <d3dx9math.h>

#if defined(hyper)
#undef hyper
#endif

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

    auto math::sin(std::uint16_t angle) -> float
    {
        float a1 = static_cast<float>(angle) * 0.000095873802f;
        float mul = 1.0f;

        if (a1 < 4.712389f)
        {
            if (a1 >= 1.5707964f)
            {
                a1 -= 3.1415927f;
                mul = -1.0f;
            }
        }
        else
        {
            a1 -= 6.2831855f;
        }

        float a2 = a1 * a1;
        float a3 = a1 * a2;
        float a5 = a2 * a3;
        float a7 = a2 * a5;
        float a9 = a2 * a7;

        return (a1
            - a3 * 0.16666667f
            + a5 * 0.0083333338f
            - a7 * 0.0001984127f
            + a9 * 0.0000027557319f) 
            * mul;
    }

    auto math::sin(float angle) -> float
    {
        return math::sin(static_cast<std::uint16_t>(angle * 10430.378f)); // 10430.378 = UINT16_MAX / 2pi
    }

    auto math::cos(std::uint16_t angle) -> float
    {
        return math::sin(static_cast<std::uint16_t>(angle + 0x4000u));
    }

    auto math::cos(float angle) -> float
    {
        return math::cos(static_cast<std::uint16_t>(angle * 10430.378f)); // 10430.378 = UINT16_MAX / 2pi
    }

    auto math::tan(std::uint16_t angle) -> float
    {
        return math::sin(angle) / math::cos(angle);
    }

    auto math::tan(float angle) -> float
    {
        return math::sin(angle) / math::cos(angle);
    }

    void math::sincos(std::uint16_t angle, float& sin, float& cos)
    {
        float a1 = static_cast<float>(angle) * 0.000095873802f;
        float mul = 1.0f;
        
        if (a1 < 4.712389f)
        {
            if (a1 >= 1.5707964f)
            {
                a1 -= 3.1415927f;
                mul = -1.0f;
            }
        }
        else
        {
            a1 -= 6.2831855f;
        }
        
        float a2 = a1 * a1;
        float a3 = a1 * a2;
        float a4 = a1 * a3;
        float a5 = a1 * a4;
        float a6 = a1 * a5;
        float a7 = a1 * a6;
        float a8 = a1 * a7;
        float a9 = a1 * a8;
        float a10 = a1 * a9;
        
        sin = (a1
            - a3 * 0.16666667f
            + a5 * 0.0083333338f
            - a7 * 0.0001984127f
            + a9 * 0.0000027557319f) 
            * mul;

        cos = (1.00f
            - a2 * 0.5f
            + a4 * 0.041666668f
            - a6 * 0.0013888889f
            + a8 * 0.000024801588f
            - a10 * 2.755732E-7f)
            * mul;
    }

    bool math::approximately(float a, float b)
    {
        return ::fabs(b - a) < math::max(1e-06f * math::max(::fabs(a), ::fabs(b)), math::epsilon * 8.0f);
    }

    void math::multiply_matrix(const matrix4x4& a, const matrix4x4& b, matrix4x4& result)
    {
#if defined(USE_D3DX9_MATH_FUNCTIONS)
        ::D3DXMatrixMultiply(reinterpret_cast<D3DXMATRIX*>(&result), reinterpret_cast<const D3DXMATRIX*>(&a), reinterpret_cast<const D3DXMATRIX*>(&b));
#else
        matrix4x4 stack;

        stack.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
        stack.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
        stack.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
        stack.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;
        stack.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
        stack.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
        stack.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
        stack.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;
        stack.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
        stack.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
        stack.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
        stack.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;
        stack.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
        stack.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
        stack.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
        stack.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;

        result = stack;
#endif
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

    void math::transform_vector(const matrix4x4& trs, vector3& vector)
    {
        float x = vector.x * trs.m11 + vector.y * trs.m21 + vector.z * trs.m31;
        float y = vector.x * trs.m12 + vector.y * trs.m22 + vector.z * trs.m32;
        float z = vector.x * trs.m13 + vector.y * trs.m23 + vector.z * trs.m33;

        vector.x = x;
        vector.y = y;
        vector.z = z;
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
        float a1 = (point.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (point.y - p2.y);
        float a2 = (point.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (point.y - p3.y);
        float a3 = (point.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (point.y - p1.y);

        bool has_neg = (a1 < 0) || (a2 < 0) || (a3 < 0);
        bool has_pos = (a1 > 0) || (a2 > 0) || (a3 > 0);

        return !(has_neg && has_pos);
    }

    auto math::get_triangle_height(const vector2& at, const vector3& p1, const vector3& p2, const vector3& p3) -> float
    {
        vector3 cross = vector3::cross(p3 - p1, p2 - p1).normalized();

        float arg1 = cross.x * at.x + cross.y * at.y;
        float arg2 = cross.x * p1.x + cross.y * p1.y + cross.z * p1.z;

        return (arg2 - arg1) / cross.z;
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

    void math::create_axis_rotation_matrix(const vector3& axis, std::uint16_t angle, matrix4x4& result)
    {
        float sin = math::sin(angle);
        float cos = math::cos(angle);
        float inv = 1.0f - cos;

        float x2 = axis.x * axis.x;
        float y2 = axis.y * axis.y;
        float z2 = axis.z * axis.z;
        float xy = axis.x * axis.y;
        float yz = axis.y * axis.z;
        float zx = axis.z * axis.x;

        result.m11 = x2 * inv + cos;
        result.m12 = xy * inv - sin * axis.z;
        result.m13 = zx * inv + sin * axis.y;
        result.m14 = 0.0f;

        result.m21 = xy * inv + sin * axis.z;
        result.m22 = y2 * inv + cos;
        result.m23 = yz * inv - sin * axis.x;
        result.m24 = 0.0f;

        result.m31 = zx * inv - sin * axis.y;
        result.m32 = yz * inv + sin * axis.x;
        result.m33 = z2 * inv + cos;
        result.m34 = 0.0f;

        result.m41 = 0.0;
        result.m42 = 0.0;
        result.m43 = 0.0;
        result.m44 = 1.0;
    }

    void math::create_rotation_x(std::uint16_t angle, matrix4x4& result)
    {
        float sin = math::sin(angle);
        float cos = math::cos(angle);

        result.m11 = 1.0f;
        result.m12 = 0.0f;
        result.m13 = 0.0f;
        result.m14 = 0.0f;

        result.m21 = 0.0f;
        result.m22 = +cos;
        result.m23 = +sin;
        result.m24 = 0.0f;

        result.m31 = 0.0f;
        result.m32 = -sin;
        result.m33 = +cos;
        result.m34 = 0.0f;

        result.m41 = 0.0f;
        result.m42 = 0.0f;
        result.m43 = 0.0f;
        result.m44 = 1.0f;
    }

    void math::create_rotation_y(std::uint16_t angle, matrix4x4& result)
    {
        float sin = math::sin(angle);
        float cos = math::cos(angle);

        result.m11 = +cos;
        result.m12 = 0.0f;
        result.m13 = -sin;
        result.m14 = 0.0f;

        result.m21 = 0.0f;
        result.m22 = 1.0f;
        result.m23 = 0.0f;
        result.m24 = 0.0f;

        result.m31 = +sin;
        result.m32 = 0.0f;
        result.m33 = +cos;
        result.m34 = 0.0f;

        result.m41 = 0.0f;
        result.m42 = 0.0f;
        result.m43 = 0.0f;
        result.m44 = 1.0f;
    }

    void math::create_rotation_z(std::uint16_t angle, matrix4x4& result)
    {
        float sin = math::sin(angle);
        float cos = math::cos(angle);

        result.m11 = +cos;
        result.m12 = +sin;
        result.m13 = 0.0f;
        result.m14 = 0.0f;

        result.m21 = -sin;
        result.m22 = +cos;
        result.m23 = 0.0f;
        result.m24 = 0.0f;

        result.m31 = 0.0f;
        result.m32 = 0.0f;
        result.m33 = 1.0f;
        result.m34 = 0.0f;

        result.m41 = 0.0f;
        result.m42 = 0.0f;
        result.m43 = 0.0f;
        result.m44 = 1.0f;
    }

    void math::rotate_matrix_x(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result)
    {
        matrix4x4 stack;

        math::create_rotation_x(angle, stack);
        
        math::multiply_matrix(rotation, stack, result);
    }

    void math::rotate_matrix_y(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result)
    {
        matrix4x4 stack;

        math::create_rotation_y(angle, stack);

        math::multiply_matrix(rotation, stack, result);
    }

    void math::rotate_matrix_z(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result)
    {
        matrix4x4 stack;

        math::create_rotation_z(angle, stack);

        math::multiply_matrix(rotation, stack, result);
    }
}
