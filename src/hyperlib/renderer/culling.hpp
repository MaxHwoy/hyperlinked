#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/scenery.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    struct vectorized_visiblilty_info
    {
        float pixel_size;
    };

    struct scenery_draw_info
    {
        model* model;
        matrix4x4* matrix;
        scenery::instance* instance;
    };

    struct scenery_cull_info
    {
        view* view;
        instance_flags flags;
        scenery_draw_info* first_draw_info;
        scenery_draw_info* currentDraw_info;
        scenery_draw_info* top_draw_info;
        __declspec(align(0x10)) vector3 position;
        __declspec(align(0x10)) vector3 direction;
        float pixelation;
        std::int32_t preculler_section_number;
        vectorized_visiblilty_info* vectorized_info;
    };

    struct grand_scenery_cull_info
    {
        scenery_cull_info scenery_cull_infos[12];
        std::uint32_t cull_info_count;
        scenery_draw_info* first_draw_info;
        scenery_draw_info* current_draw_info;
        scenery_draw_info* top_draw_info;
    };

    ASSERT_SIZE(scenery_draw_info, 0x0C);
    ASSERT_SIZE(scenery_cull_info, 0x50);
}
