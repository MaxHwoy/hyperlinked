#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class loader final
    {
    public:
        struct streaming_entry
        {
            enum class flags : std::uint8_t
            {
                compressed = 0x1,
                compressed_in_place = 0x2,
                loading = 0x10,
                queued_unload = 0x20,
            };

            std::uint32_t key;
            std::uint32_t chunk_offset;
            std::uint32_t byte_size;
            std::uint32_t uncompressed_size;
            std::uint8_t user_flags;
            flags entry_flags;
            std::uint16_t ref_count;
            void* chunk_data;
        };

        class table
        {
        public:
            inline bool is_loaded(std::uint32_t key) const
            {
                return static_cast<bool>(this->counts_[key & (std::size(this->counts_) - 1u)]);
            }

            inline void mark_loaded(std::uint32_t key)
            {
                ++this->counts_[key & (std::size(this->counts_) - 1u)];
            }

            inline void mark_unloaded(std::uint32_t key)
            {
                --this->counts_[key & (std::size(this->counts_) - 1u)];
            }

        private:
            std::uint32_t loaded_count_;
            std::uint8_t counts_[0x2000];
        };

    public:
        static void service_resource_loading();

        static void set_delayed_resource_callback(void(*callback)(void*), void* param, bool non_recursive);

        static void load_chunks(void* memory, size_t size, const char* debug_name);

        static void unload_chunks(void* memory, size_t size);

        static void verify_chunk_integrity(chunk* block, size_t size, std::uint32_t depth);

        static auto split_temp_perm_chunks(bool count_temp, chunk* block, size_t size, void* dst, size_t dst_offset, std::uint32_t depth) -> size_t;

        static void clobber_perm_chunks(chunk* block, size_t size);

        static bool load_temp_perm_chunks(void** memory, size_t* size, std::uint32_t alloc_params, const char* debug_name);

        static void post_load_fixup();

        static void fixup_tables_plat();

    public:
        static inline bool& split_vram_data = *reinterpret_cast<bool*>(0x00A62B74);

        static inline bool& post_load_fixup_disabled = *reinterpret_cast<bool*>(0x00A995F8);

        static inline size_t& chunk_movement_offset = *reinterpret_cast<size_t*>(0x00A995E8);
    };

    CREATE_ENUM_FLAG_OPERATORS(loader::streaming_entry::flags);

    ASSERT_SIZE(loader::streaming_entry, 0x18);
    ASSERT_SIZE(loader::table, 0x2004);
}
