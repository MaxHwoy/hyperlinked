#pragma once

#include <cstdint>

#include <hyperlib/math.hpp>

namespace hyper
{
    class chunk
    {
    public:
        inline auto id() const -> std::uint32_t
        {
            return this->id_;
        }

        inline auto size() const -> std::uint32_t
        {
            return this->size_;
        }

        inline auto data() const -> void*
        {
            return const_cast<chunk*>(this) + 1;
        }

        inline auto aligned_data(size_t alignment) const -> void*
        {
            return reinterpret_cast<void*>(math::align_pow_2(reinterpret_cast<uintptr_t>(this->data()), alignment));
        }

    private:
        std::uint32_t id_;
        std::uint32_t size_;
    };
}
