#pragma once

#include <hyperlib/shared.hpp>

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

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(visuallook, hashing::vlt_const(nameof(visuallook)));
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

                ATTRIB_DEFINE_INSTANCE_CONSTRUCTOR(visuallookeffect, hashing::vlt_const(nameof(visuallookeffect)));
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
    
    ASSERT_SIZE(attrib::gen::timeofdaylighting, 0x10);
    ASSERT_SIZE(attrib::gen::timeofdaylighting::layout, 0x64);
    ASSERT_SIZE(attrib::gen::visuallook, 0x10);
    ASSERT_SIZE(attrib::gen::visuallook::layout, 0x104);
    ASSERT_SIZE(attrib::gen::visuallookeffect, 0x10);
    ASSERT_SIZE(attrib::gen::visuallookeffect::layout, 0x50);
}
