#pragma once

#include <cstdint>

#include <hyperlib/math.hpp>

namespace hyper
{
    enum class block_id : std::uint32_t
    {


        visible_section_pack_header = 0x00034151,
        visible_section_boundaries  = 0x00034152,
        visible_section_drivables   = 0x00034153,
        visible_section_specifics   = 0x00034154,
        visible_section_loadings    = 0x00034155,
        visible_section_elev_polies = 0x00034156,
        visible_section_overlay     = 0x00034158,


        visible_section_manager = 0x80034150,
    };

    class chunk
    {
    public:
        inline auto id() const -> block_id
        {
            return static_cast<block_id>(this->id_);
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

        inline auto aligned_size(size_t alignment) const -> std::uint32_t
        {
            return this->size_ - static_cast<std::uint32_t>((reinterpret_cast<uintptr_t>(this->aligned_data(alignment)) - reinterpret_cast<uintptr_t>(this->data())));
        }

        inline auto end() const -> chunk*
        {
            return reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(this->data()) + this->size_);
        }

    private:
        std::uint32_t id_;
        std::uint32_t size_;
    };
}
