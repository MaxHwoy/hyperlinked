#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    union ptr_offset
    {
        void* pointer;
        std::uint32_t offset;
    };

    struct udata
    {
        std::uint32_t tag;
        std::uint32_t size : 24;
        std::uint32_t section : 5;
        std::uint32_t allocated : 1;
        std::uint32_t embedded : 1;
        std::uint32_t indexed : 1;
        std::uint32_t count;
        ptr_offset data;
    };

    struct ugroup
    {
        std::uint32_t tag;
        std::uint32_t group_count : 27;
        std::uint32_t group_sorted : 1;
        std::uint32_t data_sorted : 1;
        std::uint32_t allocated : 1;
        std::uint32_t embedded : 1;
        std::uint32_t indexed : 1;
        std::uint32_t data_count;
        ptr_offset data;
    };

    struct uheader
    {
        std::uint32_t size;
        std::uint16_t section_number;
        std::uint32_t flags;
        uheader* last_address;
    };

    ASSERT_SIZE(udata, 0x10);
    ASSERT_SIZE(ugroup, 0x10);
    ASSERT_SIZE(uheader, 0x10);
}
