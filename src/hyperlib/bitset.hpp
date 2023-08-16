#pragma once

#include <cstdint>
#include <cassert>
#include <initializer_list>
#include <hyperlib/math.hpp>

namespace hyper
{
    template <size_t Size> class bitset
    {
    public:
        inline bitset() : bits_{}
        {
        }

        inline bitset(const std::initializer_list<size_t>& list) : bits_{}
        {
            assert(list.size() <= Size);

            for (const size_t index : list)
            {
                this->bits_[index >> 5] |= 1u << (index & 0x1Fu);
            }
        }

        constexpr inline auto size() const -> size_t
        {
            return Size;
        }

        inline void clear()
        {
            if constexpr ((sizeof(this->bits_) / sizeof(std::uint32_t)) == 1u)
            {
                this->bits_[0] = 0u;
            }
            else if constexpr ((sizeof(this->bits_) / sizeof(std::uint32_t)) == 2u)
            {
                this->bits_[0] = 0u;
                this->bits_[1] = 0u;
            }
            else if constexpr ((sizeof(this->bits_) / sizeof(std::uint32_t)) == 3u)
            {
                this->bits_[0] = 0u;
                this->bits_[1] = 0u;
                this->bits_[2] = 0u;
            }
            else if constexpr ((sizeof(this->bits_) / sizeof(std::uint32_t)) == 4u)
            {
                this->bits_[0] = 0u;
                this->bits_[1] = 0u;
                this->bits_[2] = 0u;
                this->bits_[3] = 0u;
            }
            else
            {
                ::memset(this->bits_, 0, sizeof(this->bits_));
            }
        }

        template <typename Index> inline bool get(Index index) const
        {
            return (this->bits_[static_cast<size_t>(index) >> 5u] & (1u << (static_cast<size_t>(index) & 0x1Fu))) != 0;
        }

        template <typename Index> inline void set(Index index, bool bit)
        {
            this->bits_[static_cast<size_t>(index) >> 5u] &= ~(1u << (static_cast<size_t>(index) & 0x1Fu));
            this->bits_[static_cast<size_t>(index) >> 5u] |= (static_cast<size_t>(bit) << (static_cast<size_t>(index) & 0x1Fu));
        }

    private:
        std::uint32_t bits_[(Size + 0x1Fu) >> 5u];
    };

    class bit_table
    {
    public:
        inline bit_table() = default;

        inline bit_table(const bit_table& other) = default;

        inline bit_table(bit_table&& other) = default;

        inline bit_table& operator=(const bit_table& other) = default;

        inline bit_table& operator=(bit_table&& other) = default;

        inline bit_table(std::uint8_t* table, size_t bit_count) : table_(table), size_(bit_count)
        {
        }

        inline auto size() const -> size_t
        {
            return this->size_;
        }

        inline void clear()
        {
            if (this->table_ != nullptr)
            {
                ::memset(this->table_, 0, this->size_ >> 3u);
            }
        }

        template <typename Index> inline bool get(Index index) const
        {
            return (this->table_[static_cast<size_t>(index) >> 3u] & (1u << (static_cast<size_t>(index) & 0x07u))) != 0;
        }

        template <typename Index> inline void set(Index index, bool bit)
        {
            this->table_[static_cast<size_t>(index) >> 3u] &= ~(1u << (static_cast<size_t>(index) & 0x07u));
            this->table_[static_cast<size_t>(index) >> 3u] |= (static_cast<size_t>(bit) << (static_cast<size_t>(index) & 0x07u));
        }

        template <typename T> inline static auto create(T* table, size_t element_count) -> bit_table
        {
            return bit_table(reinterpret_cast<std::uint8_t*>(table), (element_count * sizeof(T)) << 3);
        }

        template <typename T, size_t Length> inline static auto create(T(&table)[Length])
        {
            return bit_table(reinterpret_cast<std::uint8_t*>(table), (Length * sizeof(T)) << 3);
        }

    private:
        size_t size_;
        std::uint8_t* table_;
    };
}
