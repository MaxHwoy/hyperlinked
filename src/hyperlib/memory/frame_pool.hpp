#pragma once

#include <cstdint>
#include <hyperlib/memory/memory.hpp>

namespace hyper
{
    class frame_pool final
    {
    private:
        struct frame_buffer : public linked_node<frame_buffer>
        {
            char* buffer;
            alloc_size_t length;
        };

    public:
        constexpr static inline alloc_size_t alignment = 0x10u;

        constexpr static inline alloc_size_t frame_size = 100u * 1024u;

        frame_pool(const frame_pool& other) = delete;

        frame_pool& operator=(const frame_pool& other) = delete;

    public:
        frame_pool();

        ~frame_pool();

        auto heap_size() const -> alloc_size_t;

        auto free_left() const -> alloc_size_t;

        void init(alloc_size_t size);

        void destroy();

        void reset();

        auto malloc(alloc_size_t size) -> void*;

        template <typename T> inline auto malloc() -> T*
        {
            return reinterpret_cast<T*>(this->malloc(sizeof(T)));
        }

        template <typename T> inline auto malloc(std::uint32_t count) -> T*
        {
            return reinterpret_cast<T*>(this->malloc(sizeof(T) * count));
        }
        
        static void allocate_buffers(alloc_size_t total_size, std::uint32_t params);
        
        static void free_buffers();

        static void reset_buffers();

        static auto current_buffer_size() -> alloc_size_t;

        static auto malloc_matrix(std::uint32_t count) -> struct matrix4x4*;

    private:
        linked_list<frame_buffer> buffers_;
        frame_buffer* current_;
        alloc_size_t position_;

    public:
        static frame_pool instance;
    };
}
