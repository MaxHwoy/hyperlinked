#pragma once

#include <cstdint>
#include <cmath>

namespace hyper
{
    struct vector2
    {
        float x;
        float y;

        inline vector2(float x, float y) : x(x), y(y)
        {
        }

        inline vector2() = default;

        inline vector2(const vector2& other) = default;

        inline vector2(vector2&& other) noexcept = default;

        inline vector2& operator=(const vector2& other) = default;

        inline vector2& operator=(vector2&& other) noexcept = default;

        inline auto magnitude() const -> float
        {
            return ::sqrtf(this->sqr_magnitude());
        }

        inline auto sqr_magnitude() const -> float
        {
            return this->x * this->x + this->y * this->y;
        }

        inline auto operator-() -> vector2
        {
            return { -this->x, -this->y };
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

        inline vector3(float x, float y, float z) : x(x), y(y), z(z)
        {
        }

        inline vector3(const vector2& vector) : x(vector.x), y(vector.y), z(0.0f)
        {
        }

        inline vector3() = default;

        inline vector3(const vector3& other) = default;

        inline vector3(vector3&& other) noexcept = default;

        inline vector3& operator=(const vector3& other) = default;

        inline vector3& operator=(vector3&& other) noexcept = default;

        inline auto magnitude() const -> float
        {
            return ::sqrtf(this->sqr_magnitude());
        }

        inline auto sqr_magnitude() const -> float
        {
            return this->x * this->x + this->y * this->y + this->z * this->z;
        }

        inline auto as_vector2() -> vector2&
        {
            return *reinterpret_cast<vector2*>(this);
        }

        inline auto as_vector2() const -> const vector2&
        {
            return *reinterpret_cast<const vector2*>(this);
        }

        inline auto operator-() -> vector3
        {
            return { -this->x, -this->y, -this->z };
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

        inline static auto zero() -> const vector3&
        {
            return vector3::zero_;
        }

        inline static auto one() -> const vector3&
        {
            return vector3::one_;
        }

        inline static auto left() -> const vector3&
        {
            return vector3::left_;
        }

        inline static auto right() -> const vector3&
        {
            return vector3::right_;
        }

        inline static auto down() -> const vector3&
        {
            return vector3::down_;
        }

        inline static auto up() -> const vector3&
        {
            return vector3::up_;
        }

        inline static auto backward() -> const vector3&
        {
            return vector3::backward_;
        }

        inline static auto forward() -> const vector3&
        {
            return vector3::forward_;
        }

        inline static auto dot(const vector3& lhs, const vector3& rhs) -> float
        {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }

        inline static auto cross(const vector3& lhs, const vector3& rhs) -> vector3
        {
            return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
        }

    private:
        static vector3 zero_;
        static vector3 one_;
        static vector3 left_;
        static vector3 right_;
        static vector3 down_;
        static vector3 up_;
        static vector3 backward_;
        static vector3 forward_;
    };

    struct vector4
    {
        float x;
        float y;
        float z;
        float w;

        inline vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
        {
        }

        inline vector4(const vector3& vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0f)
        {
        }

        inline vector4() = default;

        inline vector4(const vector4& other) = default;

        inline vector4(vector4&& other) noexcept = default;

        inline vector4& operator=(const vector4& other) = default;

        inline vector4& operator=(vector4&& other) noexcept = default;

        inline auto magnitude() const -> float
        {
            return ::sqrtf(this->sqr_magnitude());
        }

        inline auto sqr_magnitude() const -> float
        {
            return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
        }

        inline auto as_vector3() -> vector3&
        {
            return *reinterpret_cast<vector3*>(this);
        }

        inline auto as_vector3() const -> const vector3&
        {
            return *reinterpret_cast<const vector3*>(this);
        }

        inline auto operator-() -> vector4
        {
            return { -this->x, -this->y, -this->z, -this->w };
        }

        inline auto operator+=(const vector4& other) -> vector4&
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            this->w += other.w;
        }

        inline auto operator-=(const vector4& other) -> vector4&
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            this->w -= other.w;
        }

        inline auto operator*=(float value) -> vector4&
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;
            this->w *= value;
        }

        inline auto operator+(const vector4& other) const -> vector4
        {
            return { this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w };
        }

        inline auto operator-(const vector4& other) const -> vector4
        {
            return { this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w };
        }

        inline auto operator*(float value) const -> vector4
        {
            return { this->x * value, this->y * value, this->z * value, this->w * value };
        }

        inline bool operator==(const vector4& other) const
        {
            return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
        }

        inline bool operator!=(const vector4& other) const
        {
            return this->x != other.x || this->y != other.y || this->z != other.z || this->w != other.w;
        }

        inline auto operator[](std::uint32_t index) -> float&
        {
            return reinterpret_cast<float*>(this)[index];
        }

        inline auto operator[](std::uint32_t index) const -> const float&
        {
            return reinterpret_cast<const float*>(this)[index];
        }

        auto normalized() const -> vector4;

        inline static auto dot(const vector4& lhs, const vector4& rhs) -> float
        {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
        }
    };

    struct vector3pad
    {
        float x;
        float y;
        float z;
        float pad;

        inline auto as_vector3() -> vector3&
        {
            return *reinterpret_cast<vector3*>(this);
        }

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

        inline color32(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) : r(r), g(g), b(b), a(a)
        {
        }

        inline color32() = default;

        inline color32(const color32& other) = default;

        inline color32(color32&& other) noexcept = default;

        inline color32& operator=(const color32& other) = default;

        inline color32& operator=(color32&& other) noexcept = default;

        inline bool operator==(const color32& other) const
        {
            return *reinterpret_cast<const std::uint32_t*>(this) == *reinterpret_cast<const std::uint32_t*>(&other);
        }

        inline bool operator!=(const color32& other) const
        {
            return *reinterpret_cast<const std::uint32_t*>(this) != *reinterpret_cast<const std::uint32_t*>(&other);
        }

        inline static auto clear() -> color32
        {
            return color32::clear_;
        }

        inline static auto black() -> color32
        {
            return color32::black_;
        }

        inline static auto white() -> color32
        {
            return color32::white_;
        }

    private:
        static color32 clear_;
        static color32 black_;
        static color32 white_;
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

        inline matrix4x4(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44
        ) : m11(m11), m12(m12), m13(m13), m14(m14),
            m21(m21), m22(m22), m23(m23), m24(m24),
            m31(m31), m32(m32), m33(m33), m34(m34),
            m41(m41), m42(m42), m43(m43), m44(m44)
        {
        }

        inline matrix4x4() = default;

        inline matrix4x4(const matrix4x4& other) = default;

        inline matrix4x4(matrix4x4&& other) noexcept = default;

        inline matrix4x4& operator=(const matrix4x4& other) = default;

        inline matrix4x4& operator=(matrix4x4&& other) noexcept = default;

        inline auto operator[](std::uint32_t index) -> float&
        {
            return reinterpret_cast<float*>(this)[index];
        }

        inline auto operator[](std::uint32_t index) const -> const float&
        {
            return reinterpret_cast<const float*>(this)[index];
        }

        inline auto row(std::uint32_t row) -> vector4&
        {
            return *(reinterpret_cast<vector4*>(this) + row);
        }

        inline auto row(std::uint32_t row) const -> const vector4&
        {
            return *(reinterpret_cast<const vector4*>(this) + row);
        }

        inline auto column(std::uint32_t col, vector3& vector) const
        {
            vector.x = *(&this->m11 + col);
            vector.y = *(&this->m21 + col);
            vector.z = *(&this->m31 + col);
        }

        inline static auto identity() -> const matrix4x4&
        {
            return matrix4x4::identity_;
        }

    private:
        static matrix4x4 identity_;
    };

    struct plane
    {
        vector3 normal;
        float distance;
    };

    struct bounds
    {
        vector3 center;
        vector3 extent;

        inline bounds(float x, float y, float z) : center(x, y, z), extent()
        {
        }

        inline bounds(float x, float y, float z, float r) : center(x, y, z), extent(r, r, r)
        {
        }

        inline bounds(const vector3& center) : center(center), extent()
        {
        }

        inline bounds(const vector3& center, float r) : center(center), extent(r, r, r)
        {
        }

        inline bounds(const vector3& center, const vector3& extent) : center(center), extent(extent)
        {
        }

        inline bounds() = default;

        inline bounds(const bounds& other) = default;

        inline bounds(bounds&& other) noexcept = default;

        inline bounds& operator=(const bounds& other) = default;

        inline bounds& operator=(bounds&& other) noexcept = default;

        inline auto min() const -> vector3
        {
            return this->center - this->extent;
        }

        inline auto max() const -> vector3
        {
            return this->center + this->extent;
        }

        inline void expand(float amount)
        {
            this->extent.x += amount;
            this->extent.y += amount;
            this->extent.z += amount;
        }

        inline void set_min_max(const vector3& min, const vector3& max)
        {
            this->extent = (max - min) * 0.5f;
            this->center = min + this->extent;
        }
    };

    enum class space_axis : std::int8_t
    {
        nz = -3,
        ny = -2,
        nx = -1,
        px = +1,
        py = +2,
        pz = +3,
    };

    class space_settings final
    {
    public:
        constexpr inline space_settings(space_axis x, space_axis y, space_axis z)
        {
            this->sign_x_ = static_cast<std::int8_t>(x) < 0 ? -1 : 1;
            this->sign_y_ = static_cast<std::int8_t>(y) < 0 ? -1 : 1;
            this->sign_z_ = static_cast<std::int8_t>(z) < 0 ? -1 : 1;

            this->axis_x_ = static_cast<space_axis>(static_cast<std::int8_t>(x) * this->sign_x_);
            this->axis_y_ = static_cast<space_axis>(static_cast<std::int8_t>(y) * this->sign_y_);
            this->axis_z_ = static_cast<space_axis>(static_cast<std::int8_t>(z) * this->sign_z_);

            this->valid_ = this->axis_x_ != this->axis_y_ && this->axis_y_ != this->axis_z_ && this->axis_z_ != this->axis_x_;

            this->pad_ = 0;
        }

        constexpr inline space_settings()
        {
            this->axis_x_ = static_cast<space_axis>(0);
            this->axis_y_ = static_cast<space_axis>(0);
            this->axis_z_ = static_cast<space_axis>(0);
            this->sign_x_ = 0;
            this->sign_y_ = 0;
            this->sign_z_ = 0;
            this->valid_ = false;
            this->pad_ = 0;
        }

        inline bool valid() const
        {
            return this->valid_;
        }

        inline auto axis_x() const -> space_axis
        {
            return this->axis_x_;
        }

        inline auto axis_y() const -> space_axis
        {
            return this->axis_y_;
        }

        inline auto axis_z() const -> space_axis
        {
            return this->axis_z_;
        }

        inline auto sign_x() const -> std::int8_t
        {
            return this->sign_x_;
        }

        inline auto sign_y() const -> std::int8_t
        {
            return this->sign_y_;
        }

        inline auto sign_z() const -> std::int8_t
        {
            return this->sign_z_;
        }

        inline void swap(std::int32_t& x, std::int32_t& y, std::int32_t& z)
        {
            std::int32_t values[4];

            values[1] = x;
            values[2] = y;
            values[3] = z;

            x = values[static_cast<std::int32_t>(this->axis_x_)] * this->sign_x_;
            y = values[static_cast<std::int32_t>(this->axis_y_)] * this->sign_y_;
            z = values[static_cast<std::int32_t>(this->axis_z_)] * this->sign_z_;
        }

        inline void swap(float& x, float& y, float& z)
        {
            float values[4];

            values[1] = x;
            values[2] = y;
            values[3] = z;

            x = values[static_cast<std::int32_t>(this->axis_x_)] * this->sign_x_;
            y = values[static_cast<std::int32_t>(this->axis_y_)] * this->sign_y_;
            z = values[static_cast<std::int32_t>(this->axis_z_)] * this->sign_z_;
        }

        inline void swap(vector3& vector)
        {
            float values[4];

            values[1] = vector.x;
            values[2] = vector.y;
            values[3] = vector.z;

            vector.x = values[static_cast<std::int32_t>(this->axis_x_)] * this->sign_x_;
            vector.y = values[static_cast<std::int32_t>(this->axis_y_)] * this->sign_y_;
            vector.z = values[static_cast<std::int32_t>(this->axis_z_)] * this->sign_z_;
        }

        inline void swap(vector4& vector)
        {
            float values[4];

            values[1] = vector.x;
            values[2] = vector.y;
            values[3] = vector.z;

            vector.x = values[static_cast<std::int32_t>(this->axis_x_)] * this->sign_x_;
            vector.y = values[static_cast<std::int32_t>(this->axis_y_)] * this->sign_y_;
            vector.z = values[static_cast<std::int32_t>(this->axis_z_)] * this->sign_z_;
        }

        inline void swap(matrix4x4& matrix)
        {
            this->swap(matrix.row(0u));
            this->swap(matrix.row(1u));
            this->swap(matrix.row(2u));
            this->swap(matrix.row(3u));

            this->swap(matrix.m11, matrix.m21, matrix.m31);
            this->swap(matrix.m12, matrix.m22, matrix.m32);
            this->swap(matrix.m13, matrix.m23, matrix.m33);
        }

    private:
        space_axis axis_x_;
        space_axis axis_y_;
        space_axis axis_z_;
        bool valid_;
        std::int8_t sign_x_;
        std::int8_t sign_y_;
        std::int8_t sign_z_;
        char pad_;
    };

    class math final
    {
    public:
        constexpr inline static float epsilon = 1.17549435e-38f;

    public:
        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr inline static auto align_pow_2(T value, T align) -> T
        {
            return (value + align - 1) & ~(align - 1);
        }

        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr inline static auto round_pow_2(T value, T round) -> T
        {
            return value & ~(round - 1);
        }

        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr inline static bool is_pow_2(T value)
        {
            return (value & (value - 1)) == 0 && value > 0;
        }

        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr inline static auto log_2(T value) -> size_t
        {
#if defined(__GNUC__)
            return __builtin_ffs(static_cast<size_t>(value)) - 1u;
#elif defined(_MSC_VER)
            return CHAR_BIT * sizeof(T) - ::__lzcnt(static_cast<size_t>(value)) - 1u;
#else
            size_t result = 0u;
            size_t valuex = static_cast<size_t>(value);

            for (/* empty */; valuex > 1u; valuex >>= 1u, ++result);

            return result;
#endif
        }

        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr inline static auto set_flag(T value, T flag) -> T
        {
            return value; // #TODO
        }

        template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> constexpr static inline auto ceil(T value, T divisor) -> T
        {
            return (value + (divisor - 1)) / divisor;
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

        template <typename T> constexpr inline static auto map(T value, T in_min, T in_max, T out_min, T out_max) -> T
        {
            return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

        template <typename T> constexpr inline static auto lerp(T from, T to, float t) -> T
        {
            return from + (to - from) * t;
        }

        inline static void flip_sign(float& value)
        {
            *reinterpret_cast<std::uint32_t*>(&value) ^= 0x80000000;
        }

        inline static auto to_uint16_degrees(float degrees) -> std::uint16_t
        {
            return static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 65536.0f) / 360);
        }

        static auto sin(std::uint16_t angle) -> float;

        static auto sin(float angle) -> float;

        static auto cos(std::uint16_t angle) -> float;

        static auto cos(float angle) -> float;

        static auto tan(std::uint16_t angle) -> float;

        static auto tan(float angle) -> float;

        static auto arc_sin(float value) -> std::uint16_t;

        static auto arc_tan(float x, float y) -> std::uint16_t;

        static void sincos(std::uint16_t angle, float& sin, float& cos);

        static bool approximately(float a, float b);

        static void multiply_matrix(const matrix4x4& a, const matrix4x4& b, matrix4x4& result);

        static void transpose_matrix(const matrix4x4& src, matrix4x4& dst);

        static void transform_point(const matrix4x4& trs, vector3& point);

        static void transform_point(const matrix4x4& trs, vector4& point);

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

        static void invert_rotation(const matrix4x4& src, matrix4x4& dst);

    private:
        static std::uint16_t a_tan_table_[258];
    };
}
