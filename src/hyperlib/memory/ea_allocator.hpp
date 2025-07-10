#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class ea_allocator
    {
    public:
        enum class tag_type : std::uint32_t
        {
            none = 0u,
            debug_name = 1u,
            alignment = 2u,
            offset = 3u,
            start_from_bottom = 4u,
        };

        struct tag_value_pair
        {
            tag_type tag;
            union {
                std::int32_t vint;
                std::uint32_t size;
                float vfloat;
                const void* pointer;
            } value;
            tag_value_pair* next;
        };

        hyper_interface i_allocator
        {
            virtual auto alloc(alloc_size_t size, tag_value_pair* pair) -> void* = 0;
            virtual void free(void* ptr, alloc_size_t) = 0;
            virtual auto add_ref() -> std::int32_t = 0;
            virtual auto release() -> std::int32_t = 0;
            virtual ~i_allocator() = default;
        };
    };

    ASSERT_SIZE(ea_allocator::tag_value_pair, 0x0C);
    ASSERT_SIZE(ea_allocator::i_allocator, 0x04);
}
