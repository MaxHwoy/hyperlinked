#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/eastl.hpp>
#include <hyperlib/collections/bplist.hpp>
#include <hyperlib/vault/attrib.hpp>
#include <hyperlib/assets/spline.hpp>
#include <hyperlib/assets/textures.hpp>
#include <hyperlib/assets/emitters.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    struct light_streak
    {
    public:
        enum class state : std::uint32_t
        {
            loop,
            waiting_to_loop,
            frozen,
            finishing_up,
            dead,
            count,
            first = 0,
        };

        vector3pad position_offset;
        attrib::gen::lightstreak attrib_streak;
        quick_spline* spline_path;
        emitter::emitter_attrib_wrapper* emitter_data;
        float pos_on_spline_head;
        float pos_on_spline_tail;
        float speed;
        float freeze_point;
        float wait_time;
        float wait_time_left;
        state current_state;
        float alpha_scale;
        float alpha_decrement;
        texture::info* texture;

    public:
        static inline eastl::list<bpnode<light_streak>, bstl::allocator>& list = *reinterpret_cast<eastl::list<bpnode<light_streak>, bstl::allocator>*>(0x00B4BF60);
    };

    class streak_renderer final
    {
    private:
        static void render(light_streak& streak, const view::instance& view);

    public:
        static void render(const view::instance& view);
    };

    ASSERT_SIZE(light_streak, 0x50);
}
