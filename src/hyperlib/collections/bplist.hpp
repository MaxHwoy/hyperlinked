#pragma once

#include <hyperlib/linked_list.hpp>

namespace hyper
{
    template <typename T> struct bpnode final : public linked_node<bpnode<T>>
    {
        T* object;
    };

    template <typename T> struct bplist final : public linked_list<bpnode<T>>
    {
    };
}
