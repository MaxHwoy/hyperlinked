#include <iostream>

#include <hyperlib/shared.hpp>
#include <hyperlib/utils/utils.hpp>

int main()
{
    for (std::uint32_t i = 0u; i <= std::numeric_limits<std::uint16_t>::max(); ++i)
    {
        std::uint16_t lhs = static_cast<std::uint16_t>(0x4000u) - i;
        std::uint16_t rhs = static_cast<std::uint16_t>(0x4000u - i);

        assert(lhs == rhs);
    }

    return 0;
}
