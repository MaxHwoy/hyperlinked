#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class eastl final
    {
    public:
        template <std::uint32_t N, std::uint32_t A> struct __declspec(align(A)) aligned_buffer
        {
            std::uint8_t data[N];
        };

        template <typename T, typename Allocator> struct list_base
        {
            linked_list<T> head;
            std::uint32_t size;
            Allocator allocator;
        };

        template <typename T, typename Allocator> struct list : public list_base<T, Allocator>
        {
        };

        template <typename T, typename Allocator> struct vector_base
        {
            T* begin;
            T* end;
            T* capacity;
            Allocator allocator;
        };

        template <typename T, typename Allocator> struct vector : public vector_base<T, Allocator>
        {
        };
    };
}
