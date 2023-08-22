#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class bstl final
    {
    public:
        struct allocator_base
        {
        };

        struct allocator : public allocator_base
        {
        };
    };
}
