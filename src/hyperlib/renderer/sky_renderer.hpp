#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class sky_renderer final
    {
    public:
        enum class view_type : std::uint32_t
        {
            none = 0,
            player = 1,
            unknown = 2,
            rvm_pip_env = 3,
            reflection = 4,
        };

    public:
        static void draw_skydome(const view::instance& view, view_type type);

        static void render(const view::instance& view, view_type type, float lighting_scale);

    public:
        static inline geometry::model& skydome = *reinterpret_cast<geometry::model*>(0x00B77910);

    private:
        static inline array<std::uint32_t, 5u> view_type_to_sky_enabled_table_ = array<std::uint32_t, 5u>(0x00A73374);

        static inline bool& rotate_skydome_ = *reinterpret_cast<bool*>(0x00A7335C);

        static inline bool& draw_sky_in_main_views_ = *reinterpret_cast<bool*>(0x00A7335C);

        static inline float& sky_additive_scale_ = *reinterpret_cast<float*>(0x00A732AC);

        static inline std::uint16_t& sky_rotation_ = *reinterpret_cast<std::uint16_t*>(0x00B77B78);
    };
}
