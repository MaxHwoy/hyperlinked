#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/utils/utils.hpp>

void remove_no_order(std::int32_t* ptr, std::int32_t& count, std::int32_t value)
{
    for (std::int32_t i = 0; i < count; ++i)
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
    return 0;
}
