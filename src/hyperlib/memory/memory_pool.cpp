#include <cassert>
#include <hyperlib/math.hpp>
#include <hyperlib/memory/memory_pool.hpp>
#include <hyperlib/memory/memory.hpp>

#if defined(_WIN32)
#include <Windows.h>

#if defined(hyper)
#under hyper
#endif
#else
bool IsBadReadPtr(void* ptr, std::uint32_t size)
{
    return false; // #TODO integrate good one
}
#endif

#pragma warning (disable : 4100)

namespace hyper
{
    memory_pool::override_info::override_info(const char* name, void* pool, void* memory, alloc_size_t size, malloc malloc_ptr, free free_ptr, amount_free amount_free_ptr, largest_free largest_free_ptr)
    {
        this->name_ = name;
        this->pool_ = pool;
        this->memory_ = memory;
        this->size_ = size;
        this->malloc_ = malloc_ptr;
        this->free_ = free_ptr;
        this->amount_free_ = amount_free_ptr;
        this->largest_free_ = largest_free_ptr;
    }

    auto memory_pool::override_info::name() const -> const char*
    {
        return this->name_;
    }

    auto memory_pool::override_info::allocate_memory(char pool_number, alloc_size_t size, const char* debug_text, std::int32_t debug_line, std::uint32_t params) const -> void*
    {
        void* ptr = this->malloc_(this->pool_, size, sizeof(alloc_block), params);

        if (ptr != nullptr)
        {
            alloc_block* block = reinterpret_cast<alloc_block*>(ptr);

            block->size = size + sizeof(alloc_block);
            block->requested_size = size;
            block->prepad = 0u;
            block->pool_number = pool_number;
            block->magic_number = memory_pool::alloc_override_magic;
#if defined(TRACE_HEAP_ALLOCATIONS)
            block->debug_text = debug_text;
            block->debug_line = debug_line;
#endif
            return block + 1;
        }

        return nullptr;
    }

    void memory_pool::override_info::release_memory(void* ptr) const
    {
        this->free_(this->pool_, reinterpret_cast<alloc_block*>(ptr) - 1);
    }

    auto memory_pool::override_info::get_amount_free() const -> alloc_size_t
    {
        return this->amount_free_(this->pool_);
    }

    auto memory_pool::override_info::get_largest_free_block() const -> alloc_size_t
    {
        return this->largest_free_(this->pool_);
    }

    memory_pool::memory_pool() : free_block_list_(), alloc_block_list_(), buffer_block_list_(), mutex_()
    {
        this->debug_name_ = nullptr;
        this->initial_address_ = 0;
        this->initial_size_ = 0;
        this->num_allocations_ = 0;
        this->total_num_allocations_ = 0;
        this->total_allocated_ = 0;
        this->amount_allocated_ = 0;
        this->most_amount_allocated_ = 0;
        this->amount_free_ = 0;
        this->least_amount_free_ = 0;
        this->debug_fill_enabled_ = false;
        this->unlimited_pool_ = false;
        this->initialized_ = false;
        this->pool_number_ = 0;
    }

    memory_pool::~memory_pool()
    {
        if (this->initialized_)
        {
            for (buffer_block* i = this->buffer_block_list_.begin(); i != this->buffer_block_list_.end(); /* empty */)
            {
                buffer_block* next = i->next(); // get next before freeing, otherwise crash

                ::free(i->buffer);
                ::free(i);

                i = next;
            }

            this->free_block_list_.clear();
            this->alloc_block_list_.clear();
            this->buffer_block_list_.clear();
            this->debug_name_ = nullptr;
            this->initial_address_ = nullptr;
            this->initial_size_ = 0u;
            this->num_allocations_ = 0u;
            this->total_num_allocations_ = 0u;
            this->total_allocated_ = 0u;
            this->amount_allocated_ = 0u;
            this->most_amount_allocated_ = 0u;
            this->amount_free_ = 0u;
            this->least_amount_free_ = 0u;
            this->debug_fill_enabled_ = false;
            this->unlimited_pool_ = false;
            this->initialized_ = false;
            this->pool_number_ = 0;
        }
    }

    void memory_pool::initialize(void* address, alloc_size_t size, char pool_number, const char* debug_name)
    {
#if defined(USE_HYPER_MEMORY) && defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
        if (!this->initialized_)
        {
            this->free_block_list_.clear();
            this->alloc_block_list_.clear();
            this->buffer_block_list_.clear();
            this->initial_address_ = address;
            this->initial_size_ = size;
            this->num_allocations_ = 0u;
            this->total_num_allocations_ = 0u;
            this->total_allocated_ = size;
            this->amount_allocated_ = 0u;
            this->most_amount_allocated_ = 0u;
            this->amount_free_ = size;
            this->least_amount_free_ = 0u;
            this->debug_fill_enabled_ = true;
            this->unlimited_pool_ = false;
            this->debug_name_ = debug_name;
            this->initialized_ = true;
            this->pool_number_ = pool_number;
#if !defined(USE_HYPER_MEMORY)
            this->mutex_.create();
#endif
            this->add_free_memory(address, size, debug_name);
        }
    }

    void memory_pool::close()
    {
#if defined(USE_HYPER_MEMORY) && defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
        if (this->initialized_)
        {
            if (this->num_allocations_ > 0)
            {
                this->print_allocations_internal();

                assert(false);
            }

            for (buffer_block* i = this->buffer_block_list_.begin(); i != this->buffer_block_list_.end(); /* empty */)
            {
                buffer_block* next = i->next(); // get next before freeing, otherwise crash

                ::free(i->buffer);
                ::free(i);

                i = next;
            }

            this->free_block_list_.clear();
            this->alloc_block_list_.clear();
            this->buffer_block_list_.clear();
            this->debug_name_ = nullptr;
            this->initial_address_ = nullptr;
            this->initial_size_ = 0u;
            this->num_allocations_ = 0u;
            this->total_num_allocations_ = 0u;
            this->total_allocated_ = 0u;
            this->amount_allocated_ = 0u;
            this->most_amount_allocated_ = 0u;
            this->amount_free_ = 0u;
            this->least_amount_free_ = 0u;
            this->debug_fill_enabled_ = false;
            this->unlimited_pool_ = false;
            this->initialized_ = false;
            this->pool_number_ = 0;
#if !defined(USE_HYPER_MEMORY)
            this->mutex_.destroy();
#endif
        }
    }

    void memory_pool::verify_pool_integrity(bool verify_free_pattern)
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        this->verify_pool_integrity_internal(verify_free_pattern);
    }

    auto memory_pool::get_amount_free() -> alloc_size_t
    {
#if defined(USE_HYPER_MEMORY)
        return this->amount_free_;
#else
        LOCK_MUTEX(this->mutex_);

        alloc_size_t total = 0u;

        for (const free_block* i = this->free_block_list_.begin(); i != this->free_block_list_.end(); i = i->next())
        {
            total += i->size;
        }

        return total;
#endif
    }

    auto memory_pool::get_largest_free_block() -> alloc_size_t
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        alloc_size_t largest = 0u;

        for (const free_block* i = this->free_block_list_.begin(); i != this->free_block_list_.end(); i = i->next())
        {
            if (i->size > largest)
            {
                largest = i->size;
            }
        }

        return largest;
    }

    auto memory_pool::allocate_memory(alloc_size_t size, std::uint32_t alignment, std::uint32_t offset, bool start_from_top, bool use_best_fit, const char* debug_text, std::int32_t debug_line) -> void*
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        alloc_size_t true_size = 0u;
        std::uint16_t prepad = 0u;

        void* ptr = this->allocate_memory_internal(size, alignment, offset, start_from_top, use_best_fit, &true_size, &prepad);

        if (ptr != nullptr)
        {
            alloc_block* block = reinterpret_cast<alloc_block*>(ptr);

            this->alloc_block_list_.add(block);

            block->size = true_size;
            block->requested_size = size;
            block->prepad = prepad;
            block->pool_number = this->pool_number_;
            block->magic_number = memory_pool::alloc_magic;
#if defined(TRACE_HEAP_ALLOCATIONS)
            block->debug_text = debug_text;
            block->debug_line = debug_line;
#endif
            return block + 1;
        }

        return nullptr;
    }

    void memory_pool::release_memory(void* ptr)
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        if (memory::auto_verify_pool_integrity())
        {
            this->verify_pool_integrity_internal(memory::verify_free_patterns());
        }

        alloc_block* block = reinterpret_cast<alloc_block*>(ptr) - 1;

        block->magic_number = 0;

        alloc_size_t size = block->size;

        this->alloc_block_list_.remove(block);

        this->add_free_memory(reinterpret_cast<char*>(block) - block->prepad, size, this->debug_name_);

        this->num_allocations_--;
        this->amount_allocated_ -= size;
        this->amount_free_ = this->total_allocated_ - this->amount_allocated_;
    }

    void memory_pool::verify_pool_integrity_internal(bool verify_free_pattern)
    {
        for (const free_block* i = this->free_block_list_.begin(); i != this->free_block_list_.end(); i = i->next())
        {
            assert(!::IsBadReadPtr((void*)i, 1u));
            assert(!::IsBadReadPtr((void*)i->next(), 1u));
            assert(!::IsBadReadPtr((void*)i->next(), 1u));
            assert(i->magic_number == memory_pool::free_magic);
        }

        for (const alloc_block* i = this->alloc_block_list_.begin(); i != this->alloc_block_list_.end(); i = i->next())
        {
            assert(!::IsBadReadPtr((void*)i, 1u));
            assert(!::IsBadReadPtr((void*)i->next(), 1u));
            assert(!::IsBadReadPtr((void*)i->next(), 1u));
            assert(i->magic_number == memory_pool::alloc_magic);
        }

        if (verify_free_pattern && this->debug_fill_enabled_)
        {
            for (const free_block* i = this->free_block_list_.begin(); i != this->free_block_list_.end(); i = i->next())
            {
                const std::uint32_t* endptr = reinterpret_cast<const std::uint32_t*>(reinterpret_cast<uintptr_t>(i) + i->size);

                for (const std::uint32_t* ptr = reinterpret_cast<const std::uint32_t*>(i + 1); ptr < endptr; ++ptr)
                {
                    assert(*ptr == memory_pool::debug_free_fill);
                }
            }
        }
    }

    void memory_pool::add_free_memory(void* address, alloc_size_t size, const char* debug_name)
    {
        if (address != nullptr)
        {
            free_block* curr = reinterpret_cast<free_block*>(address);

            free_block* next = this->free_block_list_.begin();

            while (next != this->free_block_list_.end())
            {
                if (curr < next)
                {
                    break;
                }

                next = next->next();
            }

            free_block* prev = next->prev();

            free_block* root;

            if (this->free_block_list_.begin() == this->free_block_list_.end()) // if no free block entries (avoids 'size' access which is invalid)
            {
                curr->size = size;
                curr->magic_number = memory_pool::free_magic;

                this->free_block_list_.add(curr);

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

                        if (this->debug_fill_enabled_)
                        {
                            ::memset(next, memory_pool::debug_free_fill, sizeof(free_block));
                        }
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
                        curr->magic_number = memory_pool::free_magic;

                        curr->prev() = next->prev();
                        curr->next() = next->next();

                        next->prev()->next() = curr;
                        next->next()->prev() = curr;

                        if (this->debug_fill_enabled_)
                        {
                            ::memset(next, memory_pool::debug_free_fill, sizeof(free_block));
                        }
                    }
                    else
                    {
                        curr->size = size;
                        curr->magic_number = memory_pool::free_magic;

                        curr->prev() = prev;
                        curr->next() = next;

                        prev->next() = curr;
                        next->prev() = curr;
                    }

                    root = curr;
                }
            }

            if (this->debug_fill_enabled_)
            {
                if (root == curr)
                {
                    ::memset(curr + 1, memory_pool::debug_free_fill, size - sizeof(free_block));
                }
                else
                {
                    ::memset(curr, memory_pool::debug_free_fill, size);
                }
            }

            if (memory::free_unused_buffers())
            {
                for (buffer_block* i = this->buffer_block_list_.begin(); i != this->buffer_block_list_.end(); i = i->next())
                {
                    if (root == i->buffer && root->size == i->size)
                    {
                        this->total_allocated_ -= i->size;

                        this->free_block_list_.remove(root);

                        this->buffer_block_list_.remove(i);

                        ::free(i->buffer);

                        ::free(i);

                        break;
                    }
                }
            }
        }
    }

    auto memory_pool::allocate_memory_internal(alloc_size_t size, std::uint32_t alignment, std::uint32_t offset, bool start_from_top, bool use_best_fit, alloc_size_t* new_size, std::uint16_t* prepad) -> void*
    {
        size = math::align_pow_2<alloc_size_t>(size, memory::memory_alignment);

        if (size < 0x10)
        {
            size = 0x10;
        }

        free_block* best_block = nullptr;
        char* start_address = nullptr;
        char* final_address = nullptr;
        char* result_address = nullptr;

        if (start_from_top)
        {
            // top -> down (a.k.a. 0x0000 -> 0x7FFF address-wise)
            // assume:
            //   - we have free block at 0x1230 of size 0x100
            //   - we want to have offset of 0x15 from the top
            //   - we want to align allocation by 0x10
            //   - we want to allocate total of 0x2A bytes
            // procedure:
            //   - align allocation size by 0x04/0x08 based on x86/x64 process -> 0x2A becomes 0x2C
            //   - offset from the current block forward by 0x15 -> 0x1230 + 0x15 = 0x1245
            //   - offset from the current block forward by size of alloc block -> 0x1245 + 0x20 = 0x1265
            //   - align the result address forward by 0x20 -> 0x1265 becomes 0x1280
            //   - this is the result address = 0x1250. the next block starts at 0x1280 + 0x2C = 0x12AC
            //   - the leftover block size after allocation would be 0x100 - (0x12AC - 0x1230) = 0x74

            alloc_size_t smallest_size = std::numeric_limits<alloc_size_t>::max();

            for (free_block* i = this->free_block_list_.begin(); i != this->free_block_list_.end(); i = i->next())
            {
                uintptr_t alloc_address = math::align_pow_2<uintptr_t>(reinterpret_cast<uintptr_t>(i) + offset + sizeof(alloc_block), static_cast<uintptr_t>(alignment));

                alloc_size_t padded_size = static_cast<alloc_size_t>(alloc_address - reinterpret_cast<uintptr_t>(i) + size);

                if (i->size >= padded_size)
                {
                    alloc_size_t alloc_leftover = i->size - padded_size;

                    if ((alloc_leftover == 0u || alloc_leftover >= 0x10) && alloc_leftover < smallest_size)
                    {
                        smallest_size = alloc_leftover;

                        best_block = i;

                        start_address = reinterpret_cast<char*>(i);

                        final_address = start_address + padded_size;

                        result_address = reinterpret_cast<char*>(alloc_address) - sizeof(alloc_block);

                        if (!use_best_fit || alloc_leftover == 0u)
                        {
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            // down -> top (a.k.a. 0x7FFF -> 0x0000)
            // assume:
            //   - we have free block at 0x1230 of size 0x100 
            //   - we want to have offset of 0x15 from the bottom
            //   - we want to align allocation by 0x10
            //   - we want to allocate total of 0x2A bytes
            // procedure:
            //   - align allocation size by 0x04/0x08 based on x86/x64 process -> 0x2A becomes 0x2C
            //   - offset from the current block forward by size of block -> 0x1230 + 0x100 = 0x1330
            //   - offset backwards by size of offset -> 0x1330 - 0x15 = 0x131B
            //   - offset backwards by size of alloc block -> 0x131B - 0x20 = 0x12FB
            //   - offset backwards by size of aligned allocation -> 0x12FB - 0x2C = 0x12CF
            //   - align result address backwards by alignment value -> 0x12CF & ~(0x20 - 1) = 0x12C0
            //   - this is the true start address 0x12C0.
            //   - the result address is (0x12C0 + 0x15 + sizeof(alloc_block) + 0x20 - 1) & ~(0x20 - 1) = 0x1300
            //   - the leftover block size is after allocation would be 0x100 - ((0x12C0 - 0x1230) = 0x70
            //   - the next block after allocation still starts at 0x1230

            alloc_size_t smallest_size = std::numeric_limits<alloc_size_t>::max();

            for (free_block* i = this->free_block_list_.tail(); i != this->free_block_list_.end(); i = i->prev())
            {
                uintptr_t true_end = reinterpret_cast<uintptr_t>(i) + i->size;

                uintptr_t alloc_address = math::round_pow_2<uintptr_t>(true_end - size, alignment);

                uintptr_t true_start = math::round_pow_2<uintptr_t>(alloc_address - offset - sizeof(alloc_block), memory::memory_alignment);

                alloc_size_t padded_size = static_cast<alloc_size_t>(true_end - true_start);

                if (i->size >= padded_size)
                {
                    alloc_size_t alloc_leftover = i->size - padded_size;

                    if ((alloc_leftover == 0u || alloc_leftover >= 0x10) && alloc_leftover < smallest_size)
                    {
                        smallest_size = alloc_leftover;

                        best_block = i;

                        start_address = reinterpret_cast<char*>(true_start);

                        final_address = reinterpret_cast<char*>(true_end);

                        result_address = reinterpret_cast<char*>(alloc_address) - sizeof(alloc_block);

                        if (!use_best_fit || alloc_leftover == 0u)
                        {
                            break;
                        }
                    }
                }
            }
        }

        if (best_block == nullptr)
        {
            alloc_size_t required_size = size + alignment + offset + sizeof(free_block) + 0x100;

            if (required_size < 100u * 1024u) // 0x19000 = 100 KB
            {
                required_size = 100u * 1024u;
            }

            buffer_block* buffer = reinterpret_cast<buffer_block*>(::malloc(sizeof(buffer_block)));

            if (buffer == nullptr)
            {
                return nullptr;
            }

            buffer->size = required_size;
            buffer->buffer = ::malloc(required_size);

            if (buffer == nullptr)
            {
                ::free(buffer);

                return nullptr;
            }

            this->total_allocated_ += required_size;

            this->add_free_memory(buffer->buffer, required_size, this->debug_name_);

            this->buffer_block_list_.add(buffer);

            return this->allocate_memory_internal(size, alignment, offset, start_from_top, use_best_fit, new_size, prepad);
        }

        *prepad = static_cast<std::uint16_t>(result_address - start_address);

        *new_size = static_cast<alloc_size_t>(final_address - start_address);

        alloc_size_t leftover = best_block->size - *new_size;

        if (start_from_top)
        {
            if (leftover == 0u)
            {
                this->free_block_list_.remove(best_block);
            }
            else
            {
                free_block* next_free = reinterpret_cast<free_block*>(reinterpret_cast<uintptr_t>(best_block) + *new_size);

                next_free->prev() = best_block->prev();
                next_free->next() = best_block->next();
                
                next_free->prev()->next() = next_free;
                next_free->next()->prev() = next_free;

                next_free->size = leftover;
                next_free->magic_number = memory_pool::free_magic;
            }
        }
        else
        {
            if (leftover == 0u)
            {
                this->free_block_list_.remove(best_block);
            }
            else
            {
                best_block->size = leftover;
            }
        }

        if (this->debug_fill_enabled_)
        {
            ::memset(start_address, memory_pool::debug_alloc_fill, *new_size);
        }

        this->num_allocations_++;
        this->total_num_allocations_++;
        this->amount_allocated_ += *new_size;
        this->most_amount_allocated_ = this->most_amount_allocated_ < this->amount_allocated_ ? this->amount_allocated_ : this->most_amount_allocated_;
        this->amount_free_ = this->total_allocated_ - this->amount_allocated_;
        this->least_amount_free_ = this->total_allocated_ - this->most_amount_allocated_;

        return result_address;
    }

    auto memory_pool::get_allocations(void** allocations, std::uint32_t max_allocations) -> std::uint32_t
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        this->alloc_block_list_.sort([](const alloc_block* lhs, const alloc_block* rhs) -> std::int32_t
        {
            intptr_t diff = (reinterpret_cast<intptr_t>(lhs) - lhs->prepad) - (reinterpret_cast<intptr_t>(rhs) - rhs->prepad);
        
            if (diff == 0)
            {
                return 0;
            }
        
            return diff < 0 ? -1 : 1;
        });

        std::uint32_t result = 0u;

        for (alloc_block* i = this->alloc_block_list_.begin(); i != this->alloc_block_list_.end(); i = i->next())
        {
            if (result < max_allocations)
            {
                allocations[result++] = i + 1;
            }
        }

        return result;
    }

    void memory_pool::print_allocations()
    {
#if defined(USE_HYPER_MEMORY)
#if defined(CONCURRENT_POOL_ACCESS)
        const std::lock_guard<std::mutex> lock = std::lock_guard(this->mutex_);
#endif
#else
        LOCK_MUTEX(this->mutex_);
#endif
        this->print_allocations_internal();
    }

    void memory_pool::print_allocations_internal()
    {
        this->alloc_block_list_.sort([](const alloc_block* lhs, const alloc_block* rhs) -> std::int32_t
        {
            intptr_t diff = (reinterpret_cast<intptr_t>(lhs) - lhs->prepad) - (reinterpret_cast<intptr_t>(rhs) - rhs->prepad);

            if (diff == 0)
            {
                return 0;
            }

            return diff < 0 ? -1 : 1;
        });

        ::printf("\nMemoryPool: \"%s\"     Amount Allocated: %d bytes      Amount Free: %d bytes\n", this->debug_name_, this->amount_allocated_, this->amount_free_);
        ::printf("AllocationNumber   Raw Address    Aligned Address          Size        True Size     Debug Text / Call Address (& Line)\n");
        ::printf("======================================================================================================================\n");

        std::uint32_t index = 0u;

        for (const alloc_block* i = this->alloc_block_list_.begin(); i != this->alloc_block_list_.end(); i = i->next())
        {
#if defined(USE_HYPER_MEMORY) && defined(TRACE_HEAP_ALLOCATIONS)
            if (i->debug_line < 0)
            {
                ::printf("    %5d          0x%08X       0x%08X       %10d       %10d     Caller: 0x%08X\n", index++, reinterpret_cast<uintptr_t>(i) - i->prepad, 
                    reinterpret_cast<uintptr_t>(i + 1), i->size, i->requested_size, reinterpret_cast<uintptr_t>(i->debug_text));
            }
            else
            {
                ::printf("    %5d          0x%08X       0x%08X       %10d       %10d     %s (%d)\n", index++, reinterpret_cast<uintptr_t>(i) - i->prepad, 
                    reinterpret_cast<uintptr_t>(i + 1), i->size, i->requested_size, i->debug_text, i->debug_line);
            }
#else
            ::printf("    %5d          0x%08X       0x%08X       %10d       %10d     [none]\n", index++, reinterpret_cast<uintptr_t>(i) - i->prepad,
                reinterpret_cast<uintptr_t>(i + 1), i->size, i->requested_size);
#endif
        }
    }
}
