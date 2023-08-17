#include <iostream>

#include <hyperlib/shared.hpp>

void remove_no_order(std::int32_t* ptr, std::int32_t& count, std::int32_t value)
{
    for (size_t i = 0u; i < count; ++i)
    {
        if (value == ptr[i])
        {
            ptr[i--] = ptr[--count];

            ptr[count] = 0;
        }
    }
}

int main()
{
    std::int32_t arr[4];

    arr[0] = 3;
    arr[1] = 3;
    arr[2] = 3;
    arr[3] = 0;

    std::int32_t count = 3;

    ::remove_no_order(arr, count, 3);


    return 0;
}
