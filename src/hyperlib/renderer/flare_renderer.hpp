#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/enums.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/poly_manager.hpp>

namespace hyper
{
    struct flare_vertex
    {
        vector3 position;
        color32 color;
        vector2 uv;
        vector4 vector;
        float adder;
    };

    class flare_renderer : public poly_manager<flare_vertex>
    {
    private:
        void commit(const vector3& position, std::uint32_t texture_key, flare::type type, color32 color, float horizontal_scale, float vertical_scale, float degree_angle);

    public:
        flare_renderer(std::uint32_t max_flares);

    public:
        void render(const render_view& view, bool render_streaks);

    public:
        static void ctor(flare_renderer& renderer);

        static void dtor(flare_renderer& renderer);

        static auto create_flare_view_mask(view_id id) -> std::uint32_t;

        static bool can_render_flares_in_view(view_id id);

        static bool is_friend_flare_view_already_committed(view_id id);

        static auto get_next_flare(std::uint32_t mask) -> flare::instance*;

        static void remove_current_flare();

        static void init();

        static void reset();

        static void submit_pool_flares(const view::instance& view);

        static void submit_world_flares(const view::instance& view, flare::render type);

        static void submit_car_flares(const view::instance& view, bool reflection);

        static void submit_flare(const view::instance& view, flare::instance& flare, const matrix4x4* local_world, float intensity_scale, flare::reflection refl_type, flare::render render_type, float horizontal_flare_scale, float reflection_override, color32 color_override, float size_scale);

    private:
        bool pool_locked_;
        texture::info* streak_flares_texture_;

    public:
        static inline flare_renderer& instance = *reinterpret_cast<flare_renderer*>(0x00B4CF28);

        static inline bool& flare_pool_off = *reinterpret_cast<bool*>(0x00B42F1C);

        static inline bool& draw_light_flares = *reinterpret_cast<bool*>(0x00A6C088);

    private:
        static bool flare_pool_inited_;

        static std::uint32_t active_flare_count_;

        static bool active_flare_types_[4]; // red, amber, green, generic

        static float active_flare_times_[4]; // red, amber, green, generic

        static float active_flare_blink_[4]; // red, amber, green, generic

        static std::uint32_t flare_texture_keys_[5];

        static bitset<static_cast<size_t>(view_id::count)> flare_mask_;

        static bitset<static_cast<size_t>(view_id::count)> view_to_flare_;

        static array<flare::params*, static_cast<size_t>(flare::type::count) * 2u> flare_params_;

        static inline flare::instance flare_pool_[1000]{};

        static inline std::uint32_t flare_bits_[1000]{};
    };

    ASSERT_SIZE(flare_vertex, 0x2C);
    ASSERT_SIZE(flare_renderer::poly, 0xB0);
    ASSERT_SIZE(flare_renderer, 0x28);
}
