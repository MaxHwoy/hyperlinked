#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class parameter_map final
    {
    public:
        struct __declspec(align(0x04)) quad8
        {
            std::uint8_t is_parent;
            std::uint8_t data;
        };

        struct __declspec(align(0x08)) quad16
        {
            std::uint16_t is_parent;
            std::uint16_t data;
        };

        hyper_interface accessor : linked_node<accessor>
        {
            virtual ~accessor() = default;
            virtual void capture_data(float x, float y) = 0;
            virtual void clear_data() = 0;
            virtual auto get_float(std::uint32_t index) -> float = 0;
            virtual auto get_int(std::uint32_t index) -> std::int32_t = 0;
            virtual void setup_for_layer() = 0;

            struct layer* layer;
            std::uint32_t auto_attach_layer_key;
            char* debug_name;
            void* current_data;

            static inline accessor& rain_accessor = *reinterpret_cast<accessor*>(0x00B75078);

            static inline accessor& wind_accessor = *reinterpret_cast<accessor*>(0x00A7AAF0);

            static inline accessor& clouds_accessor = *reinterpret_cast<accessor*>(0x00B75058);

            static inline accessor& reverb_accessor = *reinterpret_cast<accessor*>(0x00A8E96C);

            static inline accessor& speech_accessor = *reinterpret_cast<accessor*>(0x00A8E5D4);

            static inline accessor& ambience_accessor = *reinterpret_cast<accessor*>(0x00A8E80C);

            static inline accessor& flare_override_accessor = *reinterpret_cast<accessor*>(0x00B43130);
        };

        struct accessor_blend : public accessor
        {
            char* last_data;
            bool has_last_data;
        };

        struct accessor_blend_by_distance : public accessor_blend
        {
            vector2 last_position;
            bool has_last_position;
            float blend_ratio;

            static inline accessor_blend_by_distance& screen_tint_accessor = *reinterpret_cast<accessor_blend_by_distance*>(0x00B77A54); // "Screen Tint"

            static inline accessor_blend_by_distance& tint_midday_accessor = *reinterpret_cast<accessor_blend_by_distance*>(0x00B70E14); // "Screen Tint"

            static inline accessor_blend_by_distance& tint_sunrise_accessor = *reinterpret_cast<accessor_blend_by_distance*>(0x00B70DE0); // "Screen Tint"

            static inline accessor_blend_by_distance& window_reflection_index_accessor = *reinterpret_cast<accessor_blend_by_distance*>(0x00B437FC); // "Window Reflection Index"

            static inline accessor_blend_by_distance& swindow_reflection_index_accessor = *reinterpret_cast<accessor_blend_by_distance*>(0x00B437C8); // " Window Reflection Index"
        };

        struct header
        {
            std::uint32_t key;
            float quad_left;
            float quad_top;
            float quad_right;
            float quad_bottom;
            std::uint32_t quad_node_count;
            std::uint32_t set_count;
            std::uint32_t set_size;
            std::uint32_t field_count;
        };

        struct layer : linked_node<layer>
        {
            header* header;
            std::int32_t* field_types;
            std::int32_t* field_offsets;
            void* data;
            quad8* quad_data8;
            quad16* quad_data16;
            linked_list<accessor> accessors;
        };

        struct manager
        {
            linked_list<layer> layers;

            static inline bool& initialized = *reinterpret_cast<bool*>(0x00B77D20);

            static inline manager& instance = *reinterpret_cast<manager*>(0x00B77D18);
        };
    };

    ASSERT_SIZE(parameter_map::quad8, 0x04);
    ASSERT_SIZE(parameter_map::quad16, 0x08);
    ASSERT_SIZE(parameter_map::accessor, 0x1C);
    ASSERT_SIZE(parameter_map::accessor_blend, 0x24);
    ASSERT_SIZE(parameter_map::accessor_blend_by_distance, 0x34);
    ASSERT_SIZE(parameter_map::header, 0x24);
    ASSERT_SIZE(parameter_map::layer, 0x28);
    ASSERT_SIZE(parameter_map::manager, 0x08);
}
