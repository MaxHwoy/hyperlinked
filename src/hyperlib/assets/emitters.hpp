#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/slot_pool.hpp>
#include <hyperlib/vault/attrib.hpp>
#include <hyperlib/assets/textures.hpp>

namespace hyper
{
    class emitter final
    {
    public:
        enum class flags : std::uint32_t
        {
            one_shot                 = 1u << 0,
            delaying                 = 1u << 1,
            start_delay_already_done = 1u << 2,
            enabled                  = 1u << 4,
            tracked_emitter          = 1u << 5,
        };

        class acid_effect;

        struct library
        {
            struct __declspec(align(0x04)) header
            {
                std::uint32_t endian_swapped;
                std::uint32_t version;
                std::uint32_t library_count;
                std::uint16_t section_number;
            };

            std::uint32_t emitter_key;
            std::uint32_t scenery_group_key;
            std::uint16_t section_number;
            std::uint16_t trigger_count;
            acid_effect* effect;
            matrix4x4 transform;
        };

        struct fx_trigger : public linked_node<fx_trigger>
        {
            library* lib;
            float trigger_radius;
            vector3pad world_position;
            float reset_time;
            std::uint16_t probability;
            char state;
            char flags;
            float last_end;
            std::uint32_t pad;
        };

        struct effect_attrib_wrapper
        {
            attrib::gen::emitterdata static_data;
        };

        struct emitter_attrib_wrapper
        {
            attrib::gen::emitterdata static_data;
            matrix4x4 color_basis;
            vector4 size_basis;
        };

        struct control
        {
            enum class state : std::uint32_t
            {
                error,
                not_started,
                delaying,
                on_cycle,
                off_cycle,
                on,
                off,
            };

            state current_state;
            float time;
        };

        struct particle
        {
            struct variance
            {
                unsigned __int32 pad                 : 4;
                unsigned __int32 rotational_variance : 8;
                unsigned __int32 color_variance      : 8;
                unsigned __int32 size_variance       : 8;
                unsigned __int32 rotation_axis       : 3;
                unsigned __int32 rotation_direction  : 1;
            };

            vector3pad velocity;
            vector3pad accelaration;
            vector3 position;
            std::uint32_t uv_start;
            std::uint32_t uv_end;
            std::uint16_t life;
            std::uint16_t angle;
            std::uint16_t anim_frame;
            std::int16_t force_field_data;
            variance variant;
        };

        struct compressed_normal
        {
            char x;
            char y;
        };

        class acid_particle_cloud
        {
        public:
            particle particles[50];
            compressed_normal impact_normals[50];
            std::uint16_t impact_time[50];
            vector3 bbox_min;
            vector3 bbox_max;
            std::uint32_t unknown;
            std::uint32_t active_particle_count;
            emitter_attrib_wrapper* emitter_data;
            std::uint32_t ref_count;

        public:
            static inline instance_pool<acid_particle_cloud>*& pool = *reinterpret_cast<instance_pool<acid_particle_cloud>**>(0x00B42EB8);
        };

        class acid_emitter;

        class acid_effect : public linked_node<acid_effect>
        {
        public:
            linked_list<acid_emitter> emitters;
            std::uint32_t emitter_key;
            std::uint32_t scenery_group_key;
            flags state_flags;
            std::uint16_t emitter_count;
            std::uint16_t section_number;
            matrix4x4 transform;
            void* subscriber;
            float far_clip;
            float intensity;
            float unk_float;
            void(__cdecl* delete_callback)(void*, acid_effect*);
            std::uint32_t zero_particle_frame_count;
            std::uint32_t pad1;
            std::uint32_t unk[1];
            vector3pad last_position;
            effect_attrib_wrapper* dynamic_data;
            std::uint32_t unk0;
            std::uint32_t creation_time_stamp;
            std::uint32_t unk1;

        public:
            static inline instance_pool<acid_effect>*& pool = *reinterpret_cast<instance_pool<acid_effect>**>(0x00B42EB4);
        };

        class __declspec(align(0x10)) acid_emitter : public linked_node<acid_emitter>
        {
        public:
            control emm_control;
            float particle_accumulation;
            std::uint32_t random_seed;
            flags state_flags;
            std::uint16_t particle_count;
            std::uint16_t list_index;
            vector3pad inherit_velocity;
            float min_intensity;
            float max_intensity;
            texture::page_range* page_range;
            emitter_attrib_wrapper* dynamic_data;
            acid_effect* effect;
            acid_particle_cloud* currently_attached_cloud;

        public:
            static inline instance_pool<acid_emitter>*& pool = *reinterpret_cast<instance_pool<acid_emitter>**>(0x00B42EB0);
        };

        class system final
        {
        public:
            struct lib_entry
            {
                std::uint32_t key;
                library* lib;
            };

        public:
            vector3pad interest_points[2];
            vector3pad interest_vectors[2];
            std::uint32_t unknown_0;
            linked_list<int> unknown_list_0;
            char data_18[0x18];
            char data_14[0x14];
            linked_list<acid_effect> acid_effects;
            linked_list<fx_trigger> fx_triggers;
            std::uint32_t trigger_count;
            texture::info* current_texture;
            std::uint32_t acid_effect_count;
            std::uint32_t emitter_count;
            eastl::vector<lib_entry> libs;
            eastl::list<acid_particle_cloud> in_use_clouds;
            eastl::list<acid_particle_cloud> in_use_linked_clouds;
            eastl::list<acid_particle_cloud> in_use_geometry_clouds;
            eastl::list<acid_particle_cloud> available_clouds;
            texture::info* texture_page;
            texture::info* normal_map_page;

        public:
            static inline system& instance = *reinterpret_cast<system*>(0x00B4BE70);
        };
    };

    CREATE_ENUM_FLAG_OPERATORS(emitter::flags);

    ASSERT_SIZE(emitter::library, 0x50);
    ASSERT_SIZE(emitter::library::header, 0x10);
    ASSERT_SIZE(emitter::fx_trigger, 0x30);
    ASSERT_SIZE(emitter::effect_attrib_wrapper, 0x10);
    ASSERT_SIZE(emitter::emitter_attrib_wrapper, 0x60);
    ASSERT_SIZE(emitter::control, 0x08);
    ASSERT_SIZE(emitter::particle, 0x40);
    ASSERT_SIZE(emitter::acid_particle_cloud, 0xD70);
    ASSERT_SIZE(emitter::acid_effect, 0xA0);
    ASSERT_SIZE(emitter::acid_emitter, 0x50);
    ASSERT_SIZE(emitter::system, 0xF0);
}
