#pragma once

#include <cstdint>
#include <cassert>

#pragma warning (disable : 33011)

namespace hyper
{
    template <typename T> class span final
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

        template <typename Index> inline auto operator[](Index index) -> T&
        {
            assert(static_cast<size_t>(index) < this->length_);

            return this->ptr_[static_cast<size_t>(index)];
        }

        template <typename Index> inline auto operator[](Index index) const -> const T&
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

        inline auto begin() -> T*
        {
            return this->ptr_;
        }

        inline auto begin() const -> const T*
        {
            return this->ptr_;
        }

        inline auto end() -> T*
        {
            return this->ptr_ + this->length_;
        }

        inline auto end() const -> const T*
        {
            return this->ptr_ + this->length_;
        }

        inline auto slice(size_t start) -> span<T>
        {
            assert(start <= this->length_);

            return { this->ptr_ + start, this->length_ - start };
        }

        inline auto slice(size_t start, size_t count) -> span<T>
        {
            assert(start + count <= this->length_);

            return { this->ptr_ + start, count };
        }

    private:
        T* ptr_;
        size_t length_;
    };

    template <typename T> class read_only_span final
    {
    public:
        inline read_only_span() = default;

        inline read_only_span(const read_only_span& other) = default;

        inline read_only_span(read_only_span&& other) = default;

        inline read_only_span& operator=(const read_only_span& other) = default;

        inline read_only_span& operator=(read_only_span&& other) = default;

        inline read_only_span(const T* ptr, size_t length) : ptr_(ptr), length_(length)
        {
        }

        inline read_only_span(intptr_t address, size_t length) : ptr_(reinterpret_cast<const T*>(address)), length_(length)
        {
        }

        inline read_only_span(uintptr_t address, size_t length) : ptr_(reinterpret_cast<const T*>(address)), length_(length)
        {
        }

        template <size_t Length> inline read_only_span(T(&array)[Length]) : ptr_(reinterpret_cast<const T*>(array)), length_(Length)
        {
        }

        template <typename Index> inline auto operator[](Index index) const -> const T&
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

        inline auto begin() const -> const T*
        {
            return this->ptr_;
        }

        inline auto end() const -> const T*
        {
            return this->ptr_ + this->length_;
        }

        inline auto slice(size_t start) -> read_only_span<T>
        {
            assert(start <= this->length_);

            return { this->ptr_ + start, this->length_ - start };
        }

        inline auto slice(size_t start, size_t count) -> read_only_span<T>
        {
            assert(start + count <= this->length_);

            return { this->ptr_ + start, count };
        }

    private:
        const T* ptr_;
        size_t length_;
    };

    template <typename T, size_t Length> class array final
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

        inline auto begin() -> T*
        {
            return this->ptr_;
        }

        inline auto begin() const -> const T*
        {
            return this->ptr_;
        }

        inline auto end() -> T*
        {
            return this->ptr_ + Length;
        }

        inline auto end() const -> const T*
        {
            return this->ptr_ + Length;
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
