#include <hyperlib/shared.hpp>

int ClosestPowerOfTwo(unsigned int value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value -= value >> 1;

    return value;
}

unsigned int RoundUpToPowerOf2(unsigned int value)
{
    value--;

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;

    return value + 1;
}

int main()
{
    ::printf("%d vs %d", ClosestPowerOfTwo(0xFu), RoundUpToPowerOf2(0xFu));

    return 0;
}
