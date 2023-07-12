#pragma once

#include <hyperlib/shared.hpp>

#pragma warning (disable : 26451)

namespace hyper
{
    class flare final
    {
    public:
        enum class type : std::uint8_t
        {
            car_headlight,
            car_brakelight,
            car_traffic_brakelight,
            car_reverse_light,
            car_fog_light,
            car_cop_light_red,
            car_cop_light_blue,
            car_cop_light_white,
            car_cop_headlight_right,
            car_cop_headlight_left,
            car_cop_light_bright_red,
            car_cop_light_bright_blue,
            car_cop_light_orange,
            lamppost,
            catseye_orange,
            catseye_red,
            catseye_blue,
            blinking_amber,
            blinking_red,
            blinking_green,
            hand_flare,
            sun_flare,
            generic_1,
            generic_2,
            generic_3,
            generic_4,
            generic_5,
            generic_6,
            generic_7,
            generic_8,
            generic_9,
            generic_10,
            count,
        };

        enum class flags : std::uint8_t
        {
            bi_directional = 0x1,
            n_directional = 0x2,
            uni_directional = 0x4,
        };

        enum class reflection : std::uint32_t
        {
            none,
            topo,
            fast,
        };

        enum class render : std::uint32_t
        {
            norm,
            env,
            refl,
            cat_p1,
            cat_p2,
        };

        struct instance : linked_node<instance>
        {
            std::uint32_t key;
            color32 tint;
            vector3 position;
            float reflect_pos_z;
            vector3 direction;
            type type;
            flags flags;
            std::uint16_t section_number;
        };

        struct pack : linked_node<pack>
        {
            std::uint32_t version;
            std::uint32_t key;
            std::uint8_t name[0x20];
            vector3pad bbox_min;
            vector3pad bbox_max;
            std::uint16_t light_flare_count;
            bool endian_swapped;
            __declspec(align(0x04)) std::uint16_t section_number;
            __declspec(align(0x04)) linked_list<instance> flare_list;
        };

        struct params
        {
            float min_size;
            float max_size;
            color max_colour;
            float power;
            float z_bias;
            float min_scale;
            float max_scale;
            std::uint32_t texture_id;
        };
    };

    CREATE_ENUM_EXPR_OPERATORS(flare::type);
    CREATE_ENUM_FLAG_OPERATORS(flare::flags);

    ASSERT_SIZE(flare::instance, 0x30);
    ASSERT_SIZE(flare::pack, 0x60);
    ASSERT_SIZE(flare::params, 0x2C);
}
