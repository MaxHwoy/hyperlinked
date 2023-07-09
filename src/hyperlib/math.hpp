#pragma once

#include <cstdint>
#include <cmath>

namespace hyper
{
    struct vector2
    {
        float x;
        float y;

        inline vector2() : x(0.0f), y(0.0f)
        {
        }

        inline vector2(float x, float y) : x(x), y(y)
        {
        }

        inline vector2(const vector2& other) : x(other.x), y(other.y)
        {
        }

        inline vector2(vector2&& other) noexcept : x(other.x), y(other.y)
        {
        }

        inline vector2& operator=(const vector2& other)
        {
            this->x = other.x;
            this->y = other.y;

            return *this;
        }

        inline vector2& operator=(vector2&& other) noexcept
        {
            this->x = other.x;
            this->y = other.y;

            return *this;
        }

        inline auto magnitude() const -> float
        {
            return ::sqrtf(this->sqr_magnitude());
        }

        inline auto sqr_magnitude() const -> float
        {
            return this->x * this->x + this->y * this->y;
        }

        inline auto operator+=(const vector2& other) -> vector2&
        {
            this->x += other.x;
            this->y += other.y;
        }

        inline auto operator-=(const vector2& other) -> vector2&
        {
            this->x -= other.x;
            this->y -= other.y;
        }

        inline auto operator*=(float value) -> vector2&
        {
            this->x *= value;
            this->y *= value;
        }

        inline auto operator+(const vector2& other) const -> vector2
        {
            return { this->x + other.x, this->y + other.y };
        }

        inline auto operator-(const vector2& other) const -> vector2
        {
            return { this->x - other.x, this->y - other.y };
        }

        inline auto operator*(float value) const -> vector2
        {
            return { this->x * value, this->y * value };
        }

        inline bool operator==(const vector2& other) const
        {
            return this->x == other.x && this->y == other.y;
        }

        inline bool operator!=(const vector2& other) const
        {
            return this->x != other.x || this->y != other.y;
        }

        inline auto operator[](std::uint32_t index) -> float&
        {
            return reinterpret_cast<float*>(this)[index];
        }

        inline auto operator[](std::uint32_t index) const -> const float&
        {
            return reinterpret_cast<const float*>(this)[index];
        }

        auto normalized() const -> vector2;

        inline static auto dot(const vector2& lhs, const vector2& rhs) -> float
        {
            return lhs.x * rhs.x + lhs.y * rhs.y;
        }
    };

    struct vector3
    {
        float x;
        float y;
        float z;

        inline vector3() : x(0.0f), y(0.0f), z(0.0f)
        {
        }

        inline vector3(float x, float y, float z) : x(x), y(y), z(z)
        {
        }

        inline vector3(const vector2& vector) : x(vector.x), y(vector.y), z(0.0f)
        {
        }

        inline vector3(const vector3& other) : x(other.x), y(other.y), z(other.z)
        {
        }

        inline vector3(vector3&& other) noexcept : x(other.x), y(other.y), z(other.z)
        {
        }

        inline vector3& operator=(const vector3& other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;

            return *this;
        }

        inline vector3& operator=(vector3&& other) noexcept
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;

            return *this;
        }

        inline auto magnitude() const -> float
        {
            return ::sqrtf(this->sqr_magnitude());
        }

        inline auto sqr_magnitude() const -> float
        {
            return this->x * this->x + this->y * this->y + this->z * this->z;
        }

        inline auto as_vector2() const -> const vector2&
        {
            return *reinterpret_cast<const vector2*>(this);
        }

        inline auto operator+=(const vector3& other) -> vector3&
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
        }

        inline auto operator-=(const vector3& other) -> vector3&
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
        }

        inline auto operator*=(float value) -> vector3&
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;
        }

        inline auto operator+(const vector3& other) const -> vector3
        {
            return { this->x + other.x, this->y + other.y, this->z + other.z };
        }

        inline auto operator-(const vector3& other) const -> vector3
        {
            return { this->x - other.x, this->y - other.y, this->z - other.z };
        }

        inline auto operator*(float value) const -> vector3
        {
            return { this->x * value, this->y * value, this->z * value };
        }

        inline bool operator==(const vector3& other) const
        {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }

        inline bool operator!=(const vector3& other) const
        {
            return this->x != other.x || this->y != other.y || this->z != other.z;
        }

        inline auto operator[](std::uint32_t index) -> float&
        {
            return reinterpret_cast<float*>(this)[index];
        }

        inline auto operator[](std::uint32_t index) const -> const float&
        {
            return reinterpret_cast<const float*>(this)[index];
        }

        auto normalized() const -> vector3;

        inline static auto dot(const vector3& lhs, const vector3& rhs) -> float
        {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }

        inline static auto cross(const vector3& lhs, const vector3& rhs) -> vector3
        {
            return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
        }
    };

    struct vector4
    {
        float x;
        float y;
        float z;
        float w;
    };

    struct vector3pad
    {
        float x;
        float y;
        float z;
        float pad;

        inline auto as_vector3() const -> const vector3&
        {
            return *reinterpret_cast<const vector3*>(this);
        }
    };

    struct color
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct color32
    {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };

    struct matrix3x3
    {
        float m11;
        float m12;
        float m13;
        float m21;
        float m22;
        float m23;
        float m31;
        float m32;
        float m33;
    };

    struct matrix4x4
    {
        float m11;
        float m12;
        float m13;
        float m14;
        float m21;
        float m22;
        float m23;
        float m24;
        float m31;
        float m32;
        float m33;
        float m34;
        float m41;
        float m42;
        float m43;
        float m44;

        inline auto operator[](std::uint32_t index) -> float&
        {
            return reinterpret_cast<float*>(this)[index];
        }

        inline auto operator[](std::uint32_t index) const -> const float&
        {
            return reinterpret_cast<const float*>(this)[index];
        }
    };

    struct plane
    {
        vector3 normal;
        float distance;
    };

    class math final
    {
    public:
        constexpr inline static float epsilon = 1.17549435e-38f;

    public:
        template <typename T> constexpr inline static auto align_pow_2(T value, T align) -> T
        {
            return (value + align - 1) & ~(align - 1);
        }

        template <typename T> constexpr inline static auto round_pow_2(T value, T round) -> T
        {
            return value & ~(round - 1);
        }

        template <typename T> constexpr inline static bool is_pow_2(T value)
        {
            return (value & (value - 1)) == 0 && value > 0;
        }

        template <typename T> constexpr inline static auto set_flag(T value, T flag) -> T
        {
            return value; // #TODO
        }

        template <typename T> constexpr inline static auto min(T a, T b) -> T
        {
            return (b < a) ? b : a;
        }

        template <typename T> constexpr inline static auto max(T a, T b) -> T
        {
            return (a < b) ? b : a;
        }

        template <typename T> constexpr inline static auto clamp(T value, T min, T max) -> T
        {
            value = (value < min) ? min : value;
            value = (value > max) ? max : value;

            return value;
        }

        static inline void flip_sign(float& value)
        {
            *reinterpret_cast<std::uint32_t*>(&value) ^= 0x80000000;
        }

        static auto sin(std::uint16_t angle) -> float;

        static auto sin(float angle) -> float;

        static auto cos(std::uint16_t angle) -> float;

        static auto cos(float angle) -> float;

        static auto tan(std::uint16_t angle) -> float;

        static auto tan(float angle) -> float;

        static void sincos(std::uint16_t angle, float& sin, float& cos);

        static bool approximately(float a, float b);

        static void multiply_matrix(const matrix4x4& a, const matrix4x4& b, matrix4x4& result);

        static void transform_point(const matrix4x4& trs, vector3& point);

        static void transform_bound(const matrix4x4& trs, vector3& min, vector3& max);

        static void transform_vector(const matrix4x4& trs, vector3& vector);

        static bool is_in_bounding_box(const vector2& point, const vector2& min, const vector2& max);

        static bool is_in_bounding_box(const vector3& point, const vector3& min, const vector3& max);

        static bool is_in_bounding_box(const vector2& point, const vector2& min, const vector2& max, float extent);

        static bool is_in_bounding_box(const vector3& point, const vector3& min, const vector3& max, float extent);

        static bool is_point_in_polygon(const vector2& point, const vector2* polygon, std::uint32_t point_count);

        static bool is_point_in_triangle(const vector3& point, const vector3& p1, const vector3& p2, const vector3& p3);

        static auto get_triangle_height(const vector2& at, const vector3& p1, const vector3& p2, const vector3& p3) -> float;

        static auto distance_to_line(const vector2& point, const vector2& p1, const vector2& p2) -> float;

        static void create_axis_rotation_matrix(const vector3& axis, std::uint16_t angle, matrix4x4& result);

        static void create_rotation_x(std::uint16_t angle, matrix4x4& result);

        static void create_rotation_y(std::uint16_t angle, matrix4x4& result);

        static void create_rotation_z(std::uint16_t angle, matrix4x4& result);

        static void rotate_matrix_x(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result);

        static void rotate_matrix_y(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result);

        static void rotate_matrix_z(const matrix4x4& rotation, std::uint16_t angle, matrix4x4& result);
    };
}
