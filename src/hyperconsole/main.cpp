#include <iostream>

#include <hyperlib/shared.hpp>

#include <immintrin.h>

struct __declspec(align(0x10)) v4
{
    union
    {
        float f[4];
        int i[4];
        ::__m128 m;
    };
};

int main()
{
    v4 a{};
    v4 b{};
    v4 c{};

    a.f[0] = 1.0f;
    a.f[1] = 2.0f;
    a.f[2] = 4.0f;
    a.f[3] = 8.0f;

    b.f[0] = 1.0f;
    b.f[1] = 1.0f;
    b.f[2] = 1.0f;
    b.f[3] = 1.0f;

    ::__m128 m0 = ::_mm_rcp_ps(a.m);
    ::__m128 m1 = ::_mm_mul_ps(a.m, m0);
    ::__m128 m2 = ::_mm_sub_ps(b.m, m1);
    ::__m128 m3 = ::_mm_mul_ps(m0, m2);
    ::__m128 m4 = ::_mm_add_ps(m0, m3);

    return 0;
}
