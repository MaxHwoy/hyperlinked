#include <algorithm>
#include <hyperlib/math.hpp>
#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/ts_memory_pool.hpp>

namespace hyper
{
    ts_memory_pool::ts_memory_pool(void* buffer, alloc_size_t size, const char* debug_name, char pool_number) : heap_list_(), free_list_(), used_list_()
    {
        ASSERT(buffer == nullptr || (buffer != nullptr && size >= 0x80u));

        this->debug_name_ = debug_name;
        this->super_buffer_ = buffer;
        this->super_size_ = size;
        this->total_size_ = 0u;
        this->amount_free_ = 0u;
        this->amount_allocated_ = 0u;
        this->num_allocations_ = 0u;
        this->pool_number_ = pool_number;

        this->override_ = memory_pool::override_info
        (
            debug_name,
            this,
#if !defined(USE_HYPER_MEMORY)
            buffer,
            size,
#endif
            &ts_memory_pool::override_malloc,
            &ts_memory_pool::override_free,
            &ts_memory_pool::override_amount_free,
            &ts_memory_pool::override_largest_free
        );

        if (buffer != nullptr)
        {
            uintptr_t initial = reinterpret_cast<uintptr_t>(buffer);
            uintptr_t truepos = math::align_pow_2<uintptr_t>(initial, 0x80u);

            this->total_size_ = math::round_pow_2<alloc_size_t>(size - (truepos - initial), 0x80u);
            this->amount_free_ = this->total_size_;

            heap_header* header = reinterpret_cast<heap_header*>(truepos);

            header->size = this->total_size_;
            header->prepad = 0u;

            this->free_list_.add(header);
        }
    }

    ts_memory_pool::~ts_memory_pool()
    {
        this->close();
    }

    void ts_memory_pool::close()
    {
        for (heap_buffer* i = this->heap_list_.begin(); i != this->heap_list_.end(); i = i->next())
        {
            memory::free(i->buffer);
            memory::free(i);
        }

        this->heap_list_.clear();
        this->free_list_.clear();
        this->used_list_.clear();
        this->debug_name_ = nullptr;
        this->super_buffer_ = nullptr;
        this->super_size_ = 0u;
        this->total_size_ = 0u;
        this->amount_free_ = 0u;
        this->amount_allocated_ = 0u;
        this->num_allocations_ = 0u;
        this->pool_number_ = 0;
    }

    auto ts_memory_pool::override_malloc(void* pool, alloc_size_t size, std::uint32_t header_size, std::uint32_t params) -> void*
    {
        return reinterpret_cast<ts_memory_pool*>(pool)->allocate_memory(size, header_size, params);
    }

    void ts_memory_pool::override_free(void* pool, void* ptr)
    {
        reinterpret_cast<ts_memory_pool*>(pool)->release_memory(ptr);
    }

    auto ts_memory_pool::override_amount_free(void* pool) -> alloc_size_t
    {
        return reinterpret_cast<ts_memory_pool*>(pool)->get_amount_free();
    }

    auto ts_memory_pool::override_largest_free(void* pool) -> alloc_size_t
    {
        return reinterpret_cast<ts_memory_pool*>(pool)->get_largest_free_block();
    }

    auto ts_memory_pool::allocate_memory(alloc_size_t size, std::uint32_t header_size, std::uint32_t params) -> void*
    {
        std::uint32_t offset = memory::get_offset(params);
        std::uint32_t alignment = std::max(math::align_pow_2<std::uint32_t>(memory::get_alignment(params), 0x80), 0x80u); // align by 0x80 boundary
        bool start_from_top = memory::get_start_from_top(params);
        bool use_best_fit = memory::get_use_best_fit(params);

        size = math::align_pow_2<alloc_size_t>(size, static_cast<alloc_size_t>(alignment));

        alloc_size_t real_size = size + header_size + sizeof(heap_header);
        alloc_size_t true_size = math::align_pow_2<alloc_size_t>(real_size + offset, 0x80u);

        heap_header* best_header = nullptr;

        if (use_best_fit)
        {
            alloc_size_t smallest_size = std::numeric_limits<alloc_size_t>::max();

            if (start_from_top)
            {
                for (heap_header* i = this->free_list_.begin(); i != this->free_list_.end(); i = i->next())
                {
                    if (i->size >= true_size && i->size < smallest_size)
                    {
                        best_header = i;

                        smallest_size = i->size;
                    }
                }
            }
            else
            {
                for (heap_header* i = this->free_list_.tail(); i != this->free_list_.end(); i = i->prev())
                {
                    if (i->size >= true_size && i->size < smallest_size)
                    {
                        best_header = i;

                        smallest_size = i->size;
                    }
                }
            }
        }
        else
        {
            if (start_from_top)
            {
                for (heap_header* i = this->free_list_.begin(); i != this->free_list_.end(); i = i->next())
                {
                    if (i->size >= true_size)
                    {
                        best_header = i;

                        break;
                    }
                }
            }
            else
            {
                for (heap_header* i = this->free_list_.tail(); i != this->free_list_.end(); i = i->next())
                {
                    if (i->size >= true_size)
                    {
                        best_header = i;

                        break;
                    }
                }
            }
        }

        if (best_header == nullptr)
        {
            std::uint32_t buffer_params = memory::create_allocation_params(memory::pool_type::main, true, true, 0x80u, 0x00u);

#if defined(_DEBUG)
            PRINT_INFO("TSMemoryPool ran out of basic memory. Allocating additional heap buffer...");

            heap_buffer* buffer = reinterpret_cast<heap_buffer*>(memory::malloc_debug(sizeof(heap_buffer), 0u, __FILE__, __LINE__));

            buffer->buffer = memory::malloc_debug(true_size, buffer_params, __FILE__, __LINE__);
#else
            heap_buffer* buffer = reinterpret_cast<heap_buffer*>(memory::malloc(sizeof(heap_buffer), 0u));

            buffer->buffer = memory::malloc(true_size, buffer_params);
#endif
            buffer->size = true_size;
            
            this->total_size_ += true_size;

            this->heap_list_.add(buffer);

            best_header = reinterpret_cast<heap_header*>(buffer->buffer);

            best_header->size = true_size;
            best_header->prepad = 0u;

            this->free_list_.add(best_header); // needed b/c we remove it afterwards
        }

        uintptr_t prepad_address;
        uintptr_t result_address;

        alloc_size_t leftover = best_header->size - true_size;

        if (start_from_top)
        {
            result_address = reinterpret_cast<uintptr_t>(best_header) + true_size - real_size;
            prepad_address = reinterpret_cast<uintptr_t>(best_header);

            if (leftover == 0u)
            {
                this->free_list_.remove(best_header);
            }
            else
            {
                heap_header* next_free = reinterpret_cast<heap_header*>(reinterpret_cast<uintptr_t>(best_header) + true_size);

                next_free->prev() = best_header->prev();
                next_free->next() = best_header->next();

                next_free->prev()->next() = next_free;
                next_free->next()->prev() = next_free;

                next_free->size = leftover;
                next_free->prepad = 0u;
            }
        }
        else
        {
            result_address = reinterpret_cast<uintptr_t>(best_header) + best_header->size - real_size;
            prepad_address = reinterpret_cast<uintptr_t>(best_header) + best_header->size - true_size;

            if (leftover == 0u)
            {
                this->free_list_.remove(best_header);
            }
            else
            {
                best_header->size = leftover;
            }
        }

        heap_header* true_header = reinterpret_cast<heap_header*>(result_address);

        true_header->size = true_size;
        true_header->prepad = static_cast<std::uint16_t>(result_address - prepad_address);

        this->num_allocations_++;
        this->amount_allocated_ += true_size;
        this->amount_free_ = this->total_size_ - this->amount_allocated_;

        this->used_list_.add(true_header);

        return true_header + 1;
    }

    void ts_memory_pool::release_memory(void* ptr)
    {
        if (ptr != nullptr)
        {
            heap_header* fake = reinterpret_cast<heap_header*>(ptr) - 1;

            heap_header* curr = reinterpret_cast<heap_header*>(reinterpret_cast<uintptr_t>(fake) - fake->prepad);

            heap_header* next = this->free_list_.begin();

            while (next != this->free_list_.end())
            {
                if (curr < next)
                {
                    break;
                }

                next = next->next();
            }

            heap_header* prev = next->prev();

            heap_header* root;

            alloc_size_t size = fake->size;

            this->used_list_.remove(fake);

            if (this->free_list_.begin() == this->free_list_.end()) // if no free block entries (avoids 'size' access which is invalid)
            {
                curr->size = size;
                curr->prepad = 0u;

                this->free_list_.add(curr);

                root = curr;
            }
            else
            {
                if (reinterpret_cast<uintptr_t>(prev) + prev->size == reinterpret_cast<uintptr_t>(curr))
                {
                    if (reinterpret_cast<uintptr_t>(curr) + size == reinterpret_cast<uintptr_t>(next))
                    {
                        prev->size += size + next->size;

                        prev->next() = next->next();

                        next->next()->prev() = prev;
                    }
                    else
                    {
                        prev->size += size;
                    }

                    root = prev;
                }
                else
                {
                    if (reinterpret_cast<uintptr_t>(curr) + size == reinterpret_cast<uintptr_t>(next))
                    {
                        curr->size = size + next->size;
                        curr->prepad = 0u;

                        curr->prev() = next->prev();
                        curr->next() = next->next();

                        next->prev()->next() = curr;
                        next->next()->prev() = curr;
                    }
                    else
                    {
                        curr->size = size;
                        curr->prepad = 0u;

                        curr->prev() = prev;
                        curr->next() = next;

                        prev->next() = curr;
                        next->prev() = curr;
                    }

                    root = curr;
                }
            }

            for (heap_buffer* i = this->heap_list_.begin(); i != this->heap_list_.end(); i = i->next())
            {
                if (root == i->buffer && root->size == i->size)
                {
                    this->total_size_ -= i->size;

                    this->free_list_.remove(root);

                    this->heap_list_.remove(i);

                    memory::free(i->buffer);

                    memory::free(i);

                    break;
                }
            }

            this->num_allocations_--;
            this->amount_allocated_ -= size;
            this->amount_free_ = this->total_size_ - this->amount_allocated_;
        }
    }

    auto ts_memory_pool::get_amount_free() const -> alloc_size_t
    {
        return static_cast<alloc_size_t>(std::numeric_limits<std::int32_t>::max()); // unlimit the memory one can allocate (since pool is resizable now)
    }

    auto ts_memory_pool::get_largest_free_block() -> alloc_size_t
    {
        return static_cast<alloc_size_t>(std::numeric_limits<std::int32_t>::max()); // unlimit the memory one can allocate (since pool is resizable now)
    }

    auto ts_memory_pool::get_override_info() const -> const memory_pool::override_info*
    {
        return &this->override_;
    }
}
