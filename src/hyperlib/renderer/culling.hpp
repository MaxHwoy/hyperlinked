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
        geometry::model* model;
        matrix4x4* matrix;
        scenery::instance* instance;
    };

    struct scenery_cull_info
    {
        view::instance* view;
        instance_flags flags;
        scenery_draw_info* first_draw_info;
        scenery_draw_info* current_draw_info;
        scenery_draw_info* top_draw_info;
        __declspec(align(0x10)) vector3 position;
        __declspec(align(0x10)) vector3 direction;
        __declspec(align(0x10)) float pixelation;
        std::int32_t preculler_section_number;
        vectorized_visiblilty_info* vectorized_info;
    };

    class grand_scenery_cull_info
    {
    private:
        void setup_scenery_cull_info(view::instance& view, instance_flags flags);

        void do_culling();
        
        void cull_view(scenery_cull_info& cull_info);

        auto what_sections_should_we_draw(std::uint16_t* sections, std::uint32_t section_count, scenery_cull_info& cull_info) -> std::uint32_t;

        void tree_cull(scenery::pack& pack, scenery_cull_info& cull_info);

        bool draw_a_scenery(scenery::pack& pack, size_t instance_index, scenery_cull_info& cull_info, visible_state state);

        bool commit_scenery(scenery::instance& instance, scenery::info& info, scenery_cull_info& cull_info, model_lod lod, visible_state state);

        static bool info_has_markers(scenery::info& info, model_lod lod);

        static void create_transform(const scenery::instance& instance, matrix4x4* matrix);

        static auto get_pixel_size(const scenery_cull_info& cull_info, const vector3& position, float radius, float& distance) -> std::uint32_t;

        static void create_wind_matrix(const view::instance* view, std::uint32_t degrees, matrix4x4& matrix);

        static void commit_flares(const scenery::instance& instance, const geometry::solid& solid, const scenery_cull_info& cull_info);

    public:
        void setup_world_culling();

        auto get_cull_info_flags(const view::instance& view) const -> instance_flags;

        void stuff_scenery(const view::instance& view, std::uint32_t include_flags) const;

    private:
        scenery_cull_info scenery_cull_infos[12];
        std::uint32_t cull_info_count;
        scenery_draw_info* first_draw_info;
        scenery_draw_info* current_draw_info;
        scenery_draw_info* top_draw_info;
    };

    ASSERT_SIZE(scenery_draw_info, 0x0C);
    ASSERT_SIZE(scenery_cull_info, 0x50);
}
