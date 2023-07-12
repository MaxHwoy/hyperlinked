#pragma once

#include <cstdint>
#include <cassert>
#include <initializer_list>

namespace hyper
{
    template <size_t Size> class bitset
    {
    public:
        inline bitset() : bits_{}
        {
        }

        inline bitset(std::initializer_list<size_t> list) : bits_{}
        {
            assert(list.size() <= Size);

            for (const size_t index : list)
            {
                this->bits_[index >> 5] |= 1u << (index & 0x1Fu);
            }
        }

        inline auto size() const -> size_t
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
            return (this->bits_[static_cast<size_t>(index) >> 5] & (1u << (static_cast<size_t>(index) & 0x1Fu))) != 0;
        }

        template <typename Index> inline void set(Index index, bool bit)
        {
            this->bits_[static_cast<size_t>(index) >> 5] &= ~(1u << (static_cast<size_t>(index) & 0x1Fu));
            this->bits_[static_cast<size_t>(index) >> 5] |= (static_cast<std::uint32_t>(bit) << (static_cast<size_t>(index) & 0x1Fu));
        }

    private:
        std::uint32_t bits_[(Size + 0x1Fu) >> 5];
    };
}
