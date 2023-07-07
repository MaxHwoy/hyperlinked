#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/assets/aabb.hpp>

namespace hyper
{
    class light final
    {
    public:
        enum class type : std::uint8_t
        {
            ambient,
            directional,
            omni,
            spot,
            exclude,
            spot_headlight,
            count,
        };

        enum class attenuation : std::uint8_t
        {
            none,
            near,
            far,
            inverse,
            inverse_square,
        };

        enum class shape : std::uint8_t
        {
            circle,
            rectangle,
            sphere,
            aabb,
        };

        enum class state : std::uint8_t
        {
            off,
            on,
        };

        struct instance
        {
            std::uint32_t key;
            type type;
            attenuation attenuation;
            shape shape;
            state state;
            std::uint32_t exclude_key;
            color32 color;
            vector3 position;
            float radius;
            vector3 direction;
            float intensity;
            float far_start;
            float far_end;
            float falloff;
            std::uint16_t section_number;
            std::uint8_t name[0x22];
        };

        struct pack : linked_node<pack>
        {
            std::uint16_t version;
            bool endian_swapped;
            __declspec(align(0x04)) std::uint16_t section_number;
            __declspec(align(0x04)) aabb_tree* tree;
            std::uint32_t node_count;
            instance* lights;
            std::uint32_t light_count;
        };
    };

    ASSERT_SIZE(light::instance, 0x60);
    ASSERT_SIZE(light::pack, 0x20);
}
