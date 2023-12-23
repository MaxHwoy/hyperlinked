#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/bstl.hpp>
#include <hyperlib/collections/eastl.hpp>

#define ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(T, KEY)                                   \
    inline T(std::uint32_t collection_key) : instance::instance(KEY, collection_key) \
    {                                                                                \
                                                                                     \
    }

#define ATTRIB_DEFINE_ARRAY(T, NAME, SIZE)    \
    attrib::array CONCATENATE(_array_, NAME); \
    T NAME[SIZE]

namespace hyper
{
    class attrib final
    {
    public:
        hyper_interface i_type_handler
        {
        public:
            virtual ~i_type_handler() = default;
        };

        hyper_interface i_allocator
        {
        public:
            virtual auto allocate(std::uint32_t size, const char* kind) -> void* = 0;
            virtual void free(void* memory, std::uint32_t size, const char* kind) = 0;

            virtual auto amount_free() -> std::uint32_t = 0;
            virtual void dump() = 0;
        };

        hyper_interface i_garbage_collector
        {
        public:
            virtual ~i_garbage_collector() = default;

            virtual void free(std::uint8_t kind, std::uint32_t id, void* data, std::uint32_t size) = 0;
        };

        struct type_table
        {
            std::uint8_t data[0x1C];
        };

        struct string_key
        {
            std::uint32_t key;
            const char* string;
        };

        struct type_desc
        {
            std::uint32_t type;
            const char* name;

            std::uint32_t size;
            std::uint32_t index;

            attrib::i_type_handler* handler;
        };

        struct definition
        {
            enum class e_flags : std::uint8_t
            {
                is_array = 1 << 0,
                is_in_layout = 1 << 1,
                is_bound = 1 << 2,
                is_not_searchable = 1 << 3,
                is_static = 1 << 4,
            };

            std::uint32_t key;
            std::uint32_t type;

            std::uint16_t offset;
            std::uint16_t size;
            std::uint16_t max_count;

            definition::e_flags flags;
            std::int8_t alignment;
        };

        struct vault
        {
            std::uint64_t version;
            std::uint32_t user_id;

            std::uint8_t data_01_04[0x04];

            std::uint32_t reference_count;

            std::uint32_t* export_manager;
            attrib::i_garbage_collector* garbage_collector;

            std::uint32_t* dependencies;
            std::uint32_t* dependency_data_block;
            std::uint32_t* dependency_ids;

            std::uint32_t dependencies_count;
            std::uint32_t resolved_count;

            std::uint32_t* pointers;
            std::uint8_t* transient_data;

            std::uint32_t* exports;
            std::uint32_t* export_data;
            std::uint32_t* export_ids;

            std::uint32_t exports_count;

            std::uint32_t allocated_exports_count;
            std::uint32_t loaded_exports_count;

            bool is_initialized;
            bool is_deinitialized;

            std::uint8_t data_02_04[0x04];
        };

        struct array
        {
            inline auto static_data(std::uint32_t index) -> void*
            {
                return reinterpret_cast<void*(__thiscall*)(decltype(this), std::uint32_t)>(0x00462B60)(this, index);
            }

            std::uint16_t alloc;
            std::uint16_t count;
            std::uint16_t size;
            std::uint16_t encoded_type;
        };

        struct collection;

        struct node
        {
            auto get(attrib::collection* collection) const -> void*;

            std::uint32_t key;

            union
            {
                attrib::array* array;
                std::uint32_t offset;
                std::uint32_t value;
                void* pointer;
            } value;

            std::uint16_t type_index;

            std::int8_t max;
            std::int8_t flags;
        };

        struct key_list : public eastl::list<std::uint32_t, bstl::allocator>
        {
        };

        struct key_map
        {
            inline auto begin() -> attrib::node*
            {
                return &this->table[0];
            }

            inline auto begin() const -> const attrib::node*
            {
                return &this->table[0];
            }

            inline auto end() -> attrib::node*
            {
                return &this->table[this->size];
            }

            inline auto end() const -> const attrib::node*
            {
                return &this->table[this->size];
            }

            attrib::node* table;
            std::uint16_t size;

            std::uint16_t entry_count;
            std::uint16_t reference_count;

            std::int8_t worst_collision;
            std::int8_t key_shift;
        };

        struct collection_key_map
        {
            struct node
            {
                std::uint32_t key;
                attrib::collection* collection;
                std::uint32_t max;
            };

            collection_key_map::node* table;

            std::uint16_t table_size;
            std::uint16_t entry_count;
            std::uint16_t fixed_alloc;
            std::uint16_t worst_collision;
        };

        struct cls_private;

        struct cls
        {
            auto collection(std::uint32_t collection_key) -> attrib::collection*;

            inline auto collection_count() const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x00463360)(this);
            }

            inline auto definition(std::uint32_t definition_key) -> attrib::definition*
            {
                return reinterpret_cast<attrib::definition*(__thiscall*)(decltype(this), std::uint32_t)>(0x004667D0)(this, definition_key);
            }

            inline auto definition_count() const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x00460620)(this);
            }

            inline auto first_collection() const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x004652E0)(this);
            }

            inline auto next_collection(std::uint32_t previous) const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this), std::uint32_t)>(0x00465300)(this, previous);
            }

            inline auto first_definition() const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x00460630)(this);
            }

            inline auto next_definition(std::uint32_t previous) const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this), std::uint32_t)>(0x00466810)(this, previous);
            }

            std::uint32_t key;
            attrib::cls_private* privates;
        };

        struct cls_private : public attrib::cls
        {
            attrib::key_map layout_table;
            attrib::collection_key_map collections;

            std::uint16_t layout_size;
            std::uint16_t definition_count;

            attrib::definition* definitions;
            attrib::vault* source;

            void* static_data;
        };

        struct collection
        {
            static inline auto find(std::uint32_t class_key, std::uint32_t collection_key) -> attrib::collection*
            {
                return reinterpret_cast<decltype(&collection::find)>(0x00465930)(class_key, collection_key);
            }

            attrib::key_map table;
            attrib::collection* parent;

            std::uint32_t key;
            attrib::cls* cls;

            void* layout;
            attrib::vault* source;
        };

        struct ref_spec
        {
            std::uint32_t class_key;
            std::uint32_t collection_key;

            attrib::collection* collection;
        };

        struct instance;

        struct attribute
        {
            inline auto count() const -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x004640F0)(this);
            }

            attrib::instance* instance;
            attrib::collection* collection;
            attrib::node* node;
            void* data;
        };

        struct instance
        {
            inline instance(std::uint32_t class_key, std::uint32_t collection_key) : instance::instance(collection::find(class_key, collection_key))
            {
            }

            inline instance(attrib::collection* with_collection)
            {
                reinterpret_cast<void(__thiscall*)(decltype(this), attrib::collection*, std::uint32_t)>(0x00461340)(this, with_collection, 0);
            }

            inline ~instance()
            {
                reinterpret_cast<void(__thiscall*)(decltype(this))>(0x00469870)(this);
            }

            template <typename T> inline auto attribute(std::uint32_t key) const -> T*
            {
                return this->attribute<T>(key, 0);
            }

            template <typename T> inline auto attribute(std::uint32_t key, std::uint32_t index) const -> T*
            {
                return reinterpret_cast<T*(__thiscall*)(decltype(this), std::uint32_t, std::uint32_t)>(0x00463A80)(this, key, index);
            }

            inline auto get(std::uint32_t key) const -> attrib::attribute
            {
                auto result = attrib::attribute();

                reinterpret_cast<attrib::attribute*(__thiscall*)(decltype(this), attrib::attribute*, std::uint32_t)>(0x00465700)(this, &result, key);

                return result;
            }

            template <typename T> inline auto as() const -> decltype(auto)
            {
                return reinterpret_cast<T::layout*>(this->layout);
            }

            attrib::collection* collection;
            void* layout;

            std::uint32_t message_port;
            std::uint32_t flags;
        };

        struct database_privates;

        struct database
        {
            inline auto cls(std::uint32_t cls_key) -> attrib::cls*
            {
                return reinterpret_cast<attrib::cls*(__thiscall*)(decltype(this), std::uint32_t)>(0x004655E0)(this, cls_key);
            }

            inline auto first_cls() -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(0x00465630)(this);
            }

            inline auto next_cls(std::uint32_t previous) -> std::uint32_t
            {
                return reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this), std::uint32_t)>(0x00465650)(this, previous);
            }

            virtual ~database() = default;

            attrib::database_privates* privates;

            static inline attrib::database*& instance = *reinterpret_cast<attrib::database**>(0x00A8499C);
        };

        struct class_table
        {
            struct node
            {
                std::uint32_t key;
                attrib::cls* cls;
                std::uint32_t max;
            };

            class_table::node* table;

            std::uint16_t size;
            std::uint16_t entry_count;
            std::uint16_t fixed_alloc;
            std::uint16_t worst_collision;
        };

        class gen final
        {
        public:
            struct emitterdata : public attrib::instance
            {
                enum class effect_particle_animation : std::uint32_t
                {
                    particle_none = 0x0,
                    particle_2x2 = 0x2,
                    particle_4x4 = 0x4,
                    particle_8x8 = 0x8,
                    particle_16x16 = 0x10,
                };

                enum class emitter_post_processing_effect : std::uint32_t
                {
                    none,
                    smoke,
                };

                enum class effect_particle_constraint : std::uint32_t
                {
                    none = 0x0,
                    xy_axis = 0x6,
                    xz_axis = 0x5,
                    yz_axis = 0x3,
                    camera = 0x8,
                };

                struct __declspec(align(0x04)) particle_animation_info
                {
                    effect_particle_animation anim_type;
                    std::uint8_t fps;
                    std::uint8_t random_start_frame;
                };

                struct particle_texture_record
                {
                    std::uint32_t texture_key;
                    std::uint32_t index;
                };

                struct layout
                {
                    vector4 volume_extent;
                    vector4 volume_center;
                    vector4 velocity_start;
                    vector4 velocity_delta;
                    vector4 size;
                    vector4 key_positions;
                    vector4 accel_start;
                    vector4 accel_delta;
                    const char* collection_name;
                    particle_animation_info texture_animation;
                    particle_texture_record texture_record;
                    bool start_delay_random_variance;
                    float start_delay;
                    std::int32_t spread_as_disc;
                    float spread_angle;
                    float speed_variance;
                    float speed;
                    float size_variance;
                    float rotation_variance;
                    float rotational_velocity_end;
                    float rotational_velocity;
                    std::int32_t random_rotation_direction;
                    emitter_post_processing_effect post_processing_effect;
                    float on_cycle_variance;
                    float on_cycle;
                    float off_cycle_variance;
                    float off_cycle;
                    float num_particles_variance;
                    float num_particles;
                    std::int32_t motion_live;
                    float motion_inherit_variance;
                    float motion_inherit;
                    float life_variance;
                    float life;
                    bool is_one_shot;
                    float initial_angle_range;
                    float gravity;
                    float far_clip;
                    bool eliminate_unnecessary_randomness;
                    float drag;
                    float color_variance;
                    color32 color4;
                    color32 color3;
                    color32 color2;
                    color32 color1;
                    float bounce_dampen;
                    effect_particle_constraint axis_constraint;
                };

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(emitterdata, hashing::vlt_const(nameof(emitterdata)))
            };

            struct lightstreak : public attrib::instance
            {
                enum class spline_type : std::uint32_t
                {
                    GATE_CAR_SELECT_01 = 0x87F7F734,
                    GATE_CAR_SELECT_02 = 0x87F7F735,
                    GATE_MAIN_MENU_01 = 0xFBE95558,
                    GATE_MAIN_MENU_02 = 0xFBE95559,
                    GATE_MAIN_MENU_03 = 0xFBE9555A,
                    GATE_MAIN_MENU_04 = 0xFBE9555B,
                    GATE_MAIN_MENU_05 = 0xFBE9555C,
                    GATE_MAIN_MENU_06 = 0xFBE9555D,
                    GATE_OVERCLIFF_01 = 0x3AAE6DBF,
                    GATE_OVERCLIFF_02 = 0x3AAE6DC0,
                    GATE_OVERCLIFF_03 = 0x3AAE6DC1,
                    GATE_QUICKMATCH_01 = 0x34051CC9,
                    GATE_QUICKMATCH_02 = 0x34051CCA,
                    GATE_RESUME_CAREER_01 = 0x5CDD5201,
                    GATE_RESUME_CAREER_02 = 0x5CDD5202,
                    GATE_STATISTICS_01 = 0xF91FB26A,
                    GATE_STATISTICS_02 = 0xF91FB26B,
                };

                struct spline_record
                {
                    spline_type type;
                    std::uint32_t index;
                };

                struct layout
                {
                    attrib::ref_spec acid_effect;
                    spline_record path;
                    std::uint32_t vertices_per_streak;
                    float speed;
                    float length;
                    float finish_up_time;
                };

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(lightstreak, hashing::vlt_const(nameof(lightstreak)))
            };

            struct timeofdaylighting : public attrib::instance
            {
                struct layout
                {
                    vector4 specular_color;
                    vector4 fogHaze_color;
                    vector4 fixed_function_sky_color;
                    vector4 diffuse_color;
                    vector4 ambient_color;
                    float fog_sky_color_scale;
                    float fog_haze_color_scale;
                    float fog_distance_scale;
                    float env_sky_brightness;
                    float car_spec_scale;
                };

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(timeofdaylighting, hashing::vlt_const(nameof(timeofdaylighting)))
            };

            struct visuallook : public attrib::instance
            {
                struct layout
                {
                    matrix4x4 rgb_curve;
                    matrix4x4 r_curve;
                    matrix4x4 g_curve;
                    matrix4x4 b_curve;
                    float desaturation;
                };

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(visuallook, hashing::vlt_const(nameof(visuallook)))
            };

            struct visuallookeffect : public attrib::instance
            {
                struct layout
                {
                    matrix4x4 graph;
                    float magnitude;
                    float length;
                    float heattrigger;
                    float testvalue;
                };

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(visuallookeffect, hashing::vlt_const(nameof(visuallookeffect)))
            };
        };
    };

    CREATE_ENUM_FLAG_OPERATORS(attrib::definition::e_flags);

    ASSERT_SIZE(attrib::i_type_handler, 0x04);
    ASSERT_SIZE(attrib::i_allocator, 0x04);
    ASSERT_SIZE(attrib::i_garbage_collector, 0x04);
    ASSERT_SIZE(attrib::type_table, 0x1C);
    ASSERT_SIZE(attrib::string_key, 0x08);
    ASSERT_SIZE(attrib::type_desc, 0x14);
    ASSERT_SIZE(attrib::definition, 0x10);
    ASSERT_SIZE(attrib::vault, 0x58);
    ASSERT_SIZE(attrib::array, 0x08);
    ASSERT_SIZE(attrib::node, 0x0C);
    ASSERT_SIZE(attrib::key_map, 0x0C);
    ASSERT_SIZE(attrib::collection_key_map::node, 0x0C);
    ASSERT_SIZE(attrib::collection_key_map, 0x0C);
    ASSERT_SIZE(attrib::cls, 0x08);
    ASSERT_SIZE(attrib::cls_private, 0x30);
    ASSERT_SIZE(attrib::collection, 0x20);
    ASSERT_SIZE(attrib::ref_spec, 0x0C);
    ASSERT_SIZE(attrib::attribute, 0x10);
    ASSERT_SIZE(attrib::instance, 0x10);
    ASSERT_SIZE(attrib::database, 0x08);
    ASSERT_SIZE(attrib::class_table::node, 0x0C);
    ASSERT_SIZE(attrib::class_table, 0x0C);
    
    ASSERT_SIZE(attrib::gen::emitterdata, 0x10);
    ASSERT_SIZE(attrib::gen::emitterdata::particle_animation_info, 0x08);
    ASSERT_SIZE(attrib::gen::emitterdata::particle_texture_record, 0x08);
    ASSERT_SIZE(attrib::gen::emitterdata::layout, 0x124);
    ASSERT_SIZE(attrib::gen::lightstreak, 0x10);
    ASSERT_SIZE(attrib::gen::lightstreak::spline_record, 0x08);
    ASSERT_SIZE(attrib::gen::lightstreak::layout, 0x24);
    ASSERT_SIZE(attrib::gen::timeofdaylighting, 0x10);
    ASSERT_SIZE(attrib::gen::timeofdaylighting::layout, 0x64);
    ASSERT_SIZE(attrib::gen::visuallook, 0x10);
    ASSERT_SIZE(attrib::gen::visuallook::layout, 0x104);
    ASSERT_SIZE(attrib::gen::visuallookeffect, 0x10);
    ASSERT_SIZE(attrib::gen::visuallookeffect::layout, 0x50);
}
