#pragma once

#include <cstdint>

namespace hyper
{
    template <typename T, size_t Length> class array
    {
    public:
        inline array() : ptr_(nullptr)
        {
        }

        inline array(T* ptr) : ptr_(ptr)
        {
        }

        inline array(intptr_t address) : ptr_(reinterpret_cast<T*>(address))
        {
        }

        inline array(uintptr_t address) : ptr_(reinterpret_cast<T*>(address))
        {
        }

        inline array(const array& other) : ptr_(other.ptr_)
        {
        }

        inline array(array&& other) : ptr_(other.ptr_)
        {
        }

        inline array& operator=(const array& other)
        {
            this->ptr_ = other.ptr_;
        }

        inline array& operator=(array&& other)
        {
            this->ptr_ = other.ptr_;
        }

        template <typename Index> auto operator[](Index index) -> T&
        {
            return this->ptr_[static_cast<size_t>(index)];
        }

        template <typename Index> auto operator[](Index index) const -> const T&
        {
            return this->ptr_[static_cast<size_t>(index)];
        }

        inline auto length() const -> size_t
        {
            return Length;
        }

    private:
        T* ptr_;
    };
}
