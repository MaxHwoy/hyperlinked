#pragma once

#include <cstdint>

#pragma warning (disable : 33011)

namespace hyper
{
    template <typename T> class span
    {
    public:
        inline span() = default;

        inline span(const span& other) = default;

        inline span(span&& other) = default;

        inline span& operator=(const span& other) = default;

        inline span& operator=(span&& other) = default;

        inline span(T* ptr, size_t length) : ptr_(ptr), length_(length)
        {
        }

        inline span(intptr_t address, size_t length) : ptr_(reinterpret_cast<T*>(address)), length_(length)
        {
        }

        inline span(uintptr_t address, size_t length) : ptr_(reinterpret_cast<T*>(address)), length_(length)
        {
        }

        template <size_t Length> inline span(T(&array)[Length]) : ptr_(reinterpret_cast<T*>(array)), length_(Length)
        {
        }

        template <typename Index> auto operator[](Index index) -> T&
        {
            assert(static_cast<size_t>(index) < this->length_);

            return this->ptr_[static_cast<size_t>(index)];
        }

        template <typename Index> auto operator[](Index index) const -> const T&
        {
            assert(static_cast<size_t>(index) < this->length_);

            return this->ptr_[static_cast<size_t>(index)];
        }

        inline auto length() const -> size_t
        {
            return this->length_;
        }

        inline bool is_empty() const
        {
            return this->length_ == 0u;
        }

        inline void clear()
        {
            ::memset(this->ptr_, 0, this->length_ * sizeof(T));
        }

        inline void fill(const T& value)
        {
            std::fill_n(this->ptr_, this->length_, value);
        }

    private:
        T* ptr_;
        size_t length_;
    };

    template <typename T, size_t Length> class array
    {
    public:
        inline array() = default;

        inline array(const array& other) = default;

        inline array(array&& other) = default;

        inline array& operator=(const array& other) = default;

        inline array& operator=(array&& other) = default;

        inline array(T* ptr) : ptr_(ptr)
        {
        }

        inline array(intptr_t address) : ptr_(reinterpret_cast<T*>(address))
        {
        }

        inline array(uintptr_t address) : ptr_(reinterpret_cast<T*>(address))
        {
        }

        inline auto pointer() -> T*
        {
            return this->ptr_;
        }

        inline auto pointer() const -> const T*
        {
            return this->ptr_;
        }

        inline void clear()
        {
            ::memset(this->ptr_, 0, Length * sizeof(T));
        }

        template <typename Index> auto operator[](Index index) -> T&
        {
            assert(static_cast<size_t>(index) < Length);

            return this->ptr_[static_cast<size_t>(index)];
        }

        template <typename Index> auto operator[](Index index) const -> const T&
        {
            assert(static_cast<size_t>(index) < Length);

            return this->ptr_[static_cast<size_t>(index)];
        }

        constexpr inline auto length() const -> size_t
        {
            return Length;
        }

    private:
        T* ptr_;
    };
}
