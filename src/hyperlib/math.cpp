#include <hyperlib/math.hpp>
#include <d3dx9math.h>
#include <limits>

#if defined(hyper)
#undef hyper
#endif

#if defined(USE_SIMD_VECTORIZATIONS)
#include <xmmintrin.h>
#endif

namespace hyper
{
    vector3 vector3::zero_(0.0f, 0.0f, 0.0f);

    vector3 vector3::one_(1.0f, 1.0f, 1.0f);

    vector3 vector3::left_(-1.0f, 0.0f, 0.0f);

    vector3 vector3::right_(+1.0f, 0.0f, 0.0f);

    vector3 vector3::down_(0.0f, 0.0f, -1.0f);

    vector3 vector3::up_(0.0f, 0.0f, +1.0f);

    vector3 vector3::backward_(0.0f, -1.0f, 0.0f);

    vector3 vector3::forward_(0.0f, +1.0f, 0.0f);

    color32 color32::clear_(0u, 0u, 0u, 0u);

    color32 color32::black_(0u, 0u, 0u, std::numeric_limits<std::uint8_t>::max());

    color32 color32::white_(std::numeric_limits<std::uint8_t>::max(), std::numeric_limits<std::uint8_t>::max(), std::numeric_limits<std::uint8_t>::max(), std::numeric_limits<std::uint8_t>::max());

    matrix4x4 matrix4x4::identity_(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    uint16_t math::a_tan_table_[258] =
    {
        0, 41, 81, 122, 163, 204, 244, 285, 326, 367, 407, 448, 489, 529, 570,
        610, 651, 692, 732, 773, 813, 854, 894, 935, 975, 1015, 1056, 1096,
        1136, 1177, 1217, 1257, 1297, 1337, 1377, 1417, 1457, 1497, 1537, 1577,
        1617, 1656, 1696, 1736, 1775, 1815, 1854, 1894, 1933, 1973, 2012, 2051,
        2090, 2129, 2168, 2207, 2246, 2285, 2324, 2363, 2401, 2440, 2478, 2517,
        2555, 2594, 2632, 2670, 2708, 2746, 2784, 2822, 2860, 2897, 2935, 2973,
        3010, 3047, 3085, 3122, 3159, 3196, 3233, 3270, 3307, 3344, 3380, 3417,
        3453, 3490, 3526, 3562, 3599, 3635, 3670, 3706, 3742, 3778, 3813, 3849,
        3884, 3920, 3955, 3990, 4025, 4060, 4095, 4129, 4164, 4199, 4233, 4267,
        4302, 4336, 4370, 4404, 4438, 4471, 4505, 4539, 4572, 4605, 4639, 4672,
        4705, 4738, 4771, 4803, 4836, 4869, 4901, 4933, 4966, 4998, 5030, 5062,
        5094, 5125, 5157, 5188, 5220, 5251, 5282, 5313, 5344, 5375, 5406, 5437,
        5467, 5498, 5528, 5559, 5589, 5619, 5649, 5679, 5708, 5738, 5768, 5797,
        5826, 5856, 5885, 5914, 5943, 5972, 6000, 6029, 6058, 6086, 6114, 6142,
        6171, 6199, 6227, 6254, 6282, 6310, 6337, 6365, 6392, 6419, 6446, 6473,
        6500, 6527, 6554, 6580, 6607, 6633, 6660, 6686, 6712, 6738, 6764, 6790,
        6815, 6841, 6867, 6892, 6917, 6943, 6968, 6993, 7018, 7043, 7068, 7092,
        7117, 7141, 7166, 7190, 7214, 7238, 7262, 7286, 7310, 7334, 7358, 7381,
        7405, 7428, 7451, 7475, 7498, 7521, 7544, 7566, 7589, 7612, 7635, 7657,
        7679, 7702, 7724, 7746, 7768, 7790, 7812, 7834, 7856, 7877, 7899, 7920,
        7942, 7963, 7984, 8005, 8026, 8047, 8068, 8089, 8110, 8131, 8151, 8172,
        8192, 8192
    };

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

    auto vector4::normalized() const -> vector4
    {
        float magnitude = this->magnitude();

        if (math::approximately(magnitude, 0.0f))
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }

        magnitude = 1.0f / magnitude;

        return { this->x * magnitude, this->y * magnitude, this->z * magnitude, this->w * magnitude };
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

    auto math::arc_sin(float value) -> std::uint16_t
    {
        return 0u; // #TODO
    }

    auto math::arc_tan(float x, float y) -> std::uint16_t
    {
        std::uint32_t quad = 0;

        if (x < 0.0f)
        {
            quad = 1;
            x = -x;
        }

        if (y < 0.0f)
        {
            quad ^= 3;
            y = -y;
        }

        std::uint16_t a;

        if (x > y)
        {
            std::int32_t i = static_cast<std::int32_t>((y / x) * 65536.0f);
            
            const std::uint16_t* table = &math::a_tan_table_[i >> 8];

            a = (table[0] + (((table[1] - table[0]) * (i & 0xFF)) >> 8));
        }
        else
        {
            if (y > x)
            {
                std::int32_t i = static_cast<std::int32_t>((x / y) * 65536.0f);
                
                const std::uint16_t* table = &math::a_tan_table_[i >> 8];

                a = 0x4000u - (((table[1] - table[0]) * (i & 0xFF)) >> 8) - table[0];
            }
            else if (y == 0.0f)
            {
                a = 0u;
            }
            else
            {
                a = 0x2000u;
            }
        }

        switch (quad)
        {
            case 0u:
                return static_cast<std::uint16_t>(a);

            case 1u:
                return static_cast<std::uint16_t>(0x8000 - a);

            case 3u:
                return static_cast<std::uint16_t>(-a);

            default:
                return static_cast<std::uint16_t>(0x8000 + a);
        }
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
#elif defined(USE_SIMD_VECTORIZATIONS)
        __m128 row1 = _mm_load_ps(&b[0x00u]);
        __m128 row2 = _mm_load_ps(&b[0x04u]);
        __m128 row3 = _mm_load_ps(&b[0x08u]);
        __m128 row4 = _mm_load_ps(&b[0x0Cu]);

        for (std::uint32_t i = 0u; i < 4u; ++i)
        {
            __m128 brod1 = _mm_set1_ps(a[(i << 2) + 0u]);
            __m128 brod2 = _mm_set1_ps(a[(i << 2) + 1u]);
            __m128 brod3 = _mm_set1_ps(a[(i << 2) + 2u]);
            __m128 brod4 = _mm_set1_ps(a[(i << 2) + 3u]);

            __m128 row = _mm_add_ps(_mm_add_ps(_mm_mul_ps(brod1, row1), _mm_mul_ps(brod2, row2)), _mm_add_ps(_mm_mul_ps(brod3, row3), _mm_mul_ps(brod4, row4)));
            
            _mm_store_ps(&result[i << 2], row);
        }
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

    void math::transpose_matrix(const matrix4x4& src, matrix4x4& dst)
    {
        float temp;

        temp = src.m12;
        dst.m12 = src.m21;
        dst.m21 = temp;

        temp = src.m13;
        dst.m13 = src.m31;
        dst.m31 = temp;

        temp = src.m14;
        dst.m14 = src.m41;
        dst.m41 = temp;

        temp = src.m23;
        dst.m23 = src.m32;
        dst.m32 = temp;

        temp = src.m24;
        dst.m24 = src.m42;
        dst.m42 = temp;

        temp = src.m34;
        dst.m34 = src.m43;
        dst.m43 = temp;

        dst.m11 = src.m11;
        dst.m22 = src.m22;
        dst.m33 = src.m33;
        dst.m44 = src.m44;
    }

    void math::transform_point(const matrix4x4& trs, vector3& point)
    {
#if defined(USE_D3DX9_MATH_FUNCTIONS)
        ::D3DXVec3Transform(reinterpret_cast<D3DXVECTOR4*>(&point), reinterpret_cast<const D3DXVECTOR3*>(&point), reinterpret_cast<const D3DXMATRIX*>(&trs));
#elif defined(USE_SIMD_VECTORIZATIONS)
        __m128 row1 = _mm_loadu_ps(&trs[0x00u]);
        __m128 row2 = _mm_loadu_ps(&trs[0x04u]);
        __m128 row3 = _mm_loadu_ps(&trs[0x08u]);
        __m128 row4 = _mm_loadu_ps(&trs[0x0Cu]);

        __m128 x = _mm_set1_ps(point.x);
        __m128 y = _mm_set1_ps(point.y);
        __m128 z = _mm_set1_ps(point.z);

        __m128 row = _mm_add_ps(_mm_add_ps(_mm_mul_ps(x, row1), _mm_mul_ps(y, row2)), _mm_add_ps(_mm_mul_ps(z, row3), row4));

        float local[4]; // compiler will (hopefully) optimize this into int64 + int32 move

        _mm_storeu_ps(local, row);

        point = *reinterpret_cast<vector3*>(local);
#else
        float x = point.x * trs.m11 + point.y * trs.m21 + point.z * trs.m31 + trs.m41;
        float y = point.x * trs.m12 + point.y * trs.m22 + point.z * trs.m32 + trs.m42;
        float z = point.x * trs.m13 + point.y * trs.m23 + point.z * trs.m33 + trs.m43;

        point.x = x;
        point.y = y;
        point.z = z;
#endif
    }

    void math::transform_point(const matrix4x4& trs, vector4& point)
    {
#if defined(USE_D3DX9_MATH_FUNCTIONS)
        ::D3DXVec4Transform(reinterpret_cast<D3DXVECTOR4*>(&point), reinterpret_cast<const D3DXVECTOR4*>(&point), reinterpret_cast<const D3DXMATRIX*>(&trs));
#elif defined(USE_SIMD_VECTORIZATIONS)
        __m128 row1 = _mm_loadu_ps(&trs[0x00u]);
        __m128 row2 = _mm_loadu_ps(&trs[0x04u]);
        __m128 row3 = _mm_loadu_ps(&trs[0x08u]);
        __m128 row4 = _mm_loadu_ps(&trs[0x0Cu]);

        __m128 x = _mm_set1_ps(point.x);
        __m128 y = _mm_set1_ps(point.y);
        __m128 z = _mm_set1_ps(point.z);
        __m128 w = _mm_set1_ps(point.w);

        __m128 row = _mm_add_ps(_mm_add_ps(_mm_mul_ps(x, row1), _mm_mul_ps(y, row2)), _mm_add_ps(_mm_mul_ps(z, row3), _mm_mul_ps(w, row4)));

        _mm_storeu_ps(reinterpret_cast<float*>(&point), row);
#else
        float x = point.x * trs.m11 + point.y * trs.m21 + point.z * trs.m31 + point.w * trs.m41;
        float y = point.x * trs.m12 + point.y * trs.m22 + point.z * trs.m32 + point.w * trs.m42;
        float z = point.x * trs.m13 + point.y * trs.m23 + point.z * trs.m33 + point.w * trs.m43;
        float w = point.x * trs.m14 + point.y * trs.m24 + point.z * trs.m34 + point.w * trs.m44;

        point.x = x;
        point.y = y;
        point.z = z;
        point.w = w;
#endif
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

    void math::invert_rotation(const matrix4x4& src, matrix4x4& dst)
    {
        math::transpose_matrix(src, dst);

        dst.m14 = 0.0f;
        dst.m24 = 0.0f;
        dst.m34 = 0.0f;
        dst.m41 = 0.0f;
        dst.m42 = 0.0f;
        dst.m43 = 0.0f;
        dst.m44 = 0.0f;
    }
}
