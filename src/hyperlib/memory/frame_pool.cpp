#include <cassert>
#include <hyperlib/shared.hpp>
#include <hyperlib/memory/frame_pool.hpp>

namespace hyper
{
    frame_pool frame_pool::instance{};

    frame_pool::frame_pool() : buffers_(), current_(nullptr), position_(0u)
    {
    }

    frame_pool::~frame_pool()
    {
        this->destroy();
    }

    auto frame_pool::heap_size() const -> alloc_size_t
    {
        alloc_size_t total = 0u;

        for (const frame_buffer* i = this->buffers_.begin(); i != this->buffers_.end(); i = i->next())
        {
            total += i->length;
        }

        return total;
    }

    auto frame_pool::free_left() const -> alloc_size_t
    {
        if (this->current_ == nullptr)
        {
            return 0u;
        }

        return this->current_->length - this->position_;
    }

    void frame_pool::init(alloc_size_t length)
    {
        assert(this->current_ == nullptr);

        length = math::align_pow_2(length, frame_pool::alignment);

        std::uint32_t params = memory::create_allocation_params(memory::pool_type::main, true, false, frame_pool::alignment, 0u);

#if defined(_DEBUG)
        frame_buffer* buffer = reinterpret_cast<frame_buffer*>(memory::malloc_debug(length + sizeof(frame_buffer), params, __FILE__, __LINE__));
#else
        frame_buffer* buffer = reinterpret_cast<frame_buffer*>(memory::malloc(length + sizeof(frame_buffer), params));
#endif
        this->buffers_.add(buffer);
        this->current_ = buffer;
        this->position_ = 0u;

        buffer->buffer = reinterpret_cast<char*>(buffer + 1);
        buffer->length = length;
    }

    void frame_pool::destroy()
    {
        for (frame_buffer* i = this->buffers_.begin(); i != this->buffers_.end(); /* empty */)
        {
            frame_buffer* next = i->next();

            memory::free(i);

            i = next;
        }

        this->buffers_.clear();
        this->current_ = nullptr;
        this->position_ = 0u;
    }

    void frame_pool::reset()
    {
        this->current_ = this->buffers_.begin();
        this->position_ = 0u;

        for (frame_buffer* i = this->current_->next(); i != this->buffers_.end(); /* empty */)
        {
            frame_buffer* next = i->next();

            memory::free(i);

            i = next;
        }

        this->buffers_.clear();
        this->buffers_.add(this->current_);
    }

    auto frame_pool::malloc(alloc_size_t size) -> void*
    {
        size = math::align_pow_2(size, frame_pool::alignment);

        alloc_size_t final_pos = this->position_ + size;

        if (final_pos <= this->current_->length)
        {
            char* result = this->current_->buffer + this->position_;

            this->position_ = final_pos;

            return result;
        }
        else
        {
            std::uint32_t params = memory::create_allocation_params(memory::pool_type::main, true, false, frame_pool::alignment, 0u);

            alloc_size_t length = math::max(size, frame_pool::frame_size);

#if defined(_DEBUG)
            frame_buffer* buffer = reinterpret_cast<frame_buffer*>(memory::malloc_debug(length + sizeof(frame_buffer), params, __FILE__, __LINE__));
#else
            frame_buffer* buffer = reinterpret_cast<frame_buffer*>(memory::malloc(length + sizeof(frame_buffer), params));
#endif
            char* result = reinterpret_cast<char*>(buffer + 1);

            this->buffers_.add(buffer);
            this->current_ = buffer;
            this->position_ = size;

            buffer->buffer = result;
            buffer->length = length;

            return result;
        }
    }

    void frame_pool::allocate_buffers(alloc_size_t total_size, std::uint32_t params)
    {
        call_function<void(__cdecl*)(alloc_size_t, std::uint32_t)>(0x00557F90)(total_size, params);
    }

    void frame_pool::free_buffers()
    {
        call_function<void(__cdecl*)()>(0x00557FF0)();
    }

    void frame_pool::reset_buffers()
    {
        call_function<void(__cdecl*)()>(0x00558040)();
    }

    auto frame_pool::current_buffer_size() -> alloc_size_t
    {
        return call_function<alloc_size_t(__cdecl*)()>(0x00557F80)();
    }

    auto frame_pool::malloc_matrix(std::uint32_t count) -> matrix4x4*
    {
        return call_function<matrix4x4*(__cdecl*)(std::uint32_t)>(0x00402850)(count);
    }
}
