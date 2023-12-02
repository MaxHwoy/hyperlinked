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
        public:
            std::uint16_t version;
            bool endian_swapped;
            __declspec(align(0x04)) std::uint16_t section_number;
            __declspec(align(0x04)) aabb_tree* tree;
            std::uint32_t node_count;
            instance* lights;
            std::uint32_t light_count;

        public:
            static inline linked_list<pack>& list = *reinterpret_cast<linked_list<pack>*>(0x00B479D4);
        };

        struct shaper
        {
            enum class light_mode : std::int32_t
            {
                invalid = -1,
                world_space = 0,
                camera_space,
                sun_direction,
                inverse_sun_direction,
                world_position,
                count,
            };

            light_mode mode;
            float theta;
            float phi;
            color color;
        };

        struct __declspec(align(0x10)) shaper_rigorous
        {
        public:
            std::uint32_t key;
            shaper lights[4];
            vector3 padv3;
            vector3 position;
            std::uint32_t padi4;
            std::uint32_t override_slot_count;

        public:
            static inline shaper_rigorous& world = *reinterpret_cast<shaper_rigorous*>(0x00A6C4A0);

            static inline shaper_rigorous& scenery = *reinterpret_cast<shaper_rigorous*>(0x00A6CFE0);
        };

        struct dynamic : public linked_node<dynamic>, public light::instance
        {
        };

        struct dynamic_pack : linked_node<dynamic_pack>
        {
        public:
            char name[0x20];
            bool enabled;
            linked_list<dynamic> lights;

        public:
            static inline linked_list<dynamic_pack>& list = *reinterpret_cast<linked_list<dynamic_pack>*>(0x00B4CF20);
        };

        struct context
        {
            struct base
            {
                std::int32_t type;
            };

            struct dynamic : public base
            {
                vector3 padv3;
                matrix4x4 local_color;
                matrix4x4 local_direction;
                matrix4x4 local_eye;
                vector4 envmap_pos;
                vector4 idk_vector4;
                matrix4x4 envmap_rotation;
                vector4 harmonics[10];
            };

            struct particle : public base
            {
                vector3 padv3;
                vector4 color;
            };

            struct scenery : public base
            {
                char name[0x22];
                std::int16_t context_number;
                matrix4x4* local_lights;
                std::uint32_t light_count;
            };
        };
    };

    ASSERT_SIZE(light::instance, 0x60);
    ASSERT_SIZE(light::pack, 0x20);
    ASSERT_SIZE(light::shaper, 0x1C);
    ASSERT_SIZE(light::shaper_rigorous, 0xA0);
    ASSERT_SIZE(light::dynamic, 0x68);
    ASSERT_SIZE(light::dynamic_pack, 0x34);
    ASSERT_SIZE(light::context::base, 0x04);
    ASSERT_SIZE(light::context::dynamic, 0x1D0);
    ASSERT_SIZE(light::context::particle, 0x20);
    ASSERT_SIZE(light::context::scenery, 0x30);
}
