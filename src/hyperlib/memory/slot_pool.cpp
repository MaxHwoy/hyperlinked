#include <hyperlib/memory/slot_pool.hpp>

namespace hyper
{
    constexpr auto slot_pool::header_size() -> alloc_size_t
    {
        return sizeof(slot_pool) - sizeof(slot_pool::slots_);
    }

    slot_pool::slot_pool() :
        next_(nullptr),
        name_(nullptr),
        free_(nullptr),
        flags_(),
        alloc_count_(0u),
        most_alloc_count_(0u),
        pool_type_(static_cast<memory::pool_type>(0u)),
        slot_count_(0u),
        slot_size_(0u),
        slot_total_(0u)
    {
    }

    slot_pool::slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type) :
        next_(nullptr),
        name_(name),
        free_(nullptr),
        flags_(flags::overflow_if_full | flags::zero_allocated_memory | flags::warn_if_nonempty_delete),
        alloc_count_(0u),
        most_alloc_count_(0u),
        pool_type_(type),
        slot_count_(slot_count),
        slot_size_(slot_size),
        slot_total_(slot_count)
    {
        this->flush();
    }

    slot_pool::~slot_pool()
    {
        // #TODO
    }

    void slot_pool::flush()
    {
        for (slot_pool* pool = this; pool != nullptr; pool = pool->next_)
        {
            std::uint32_t count = pool->slot_count_;
            std::uint32_t size = pool->slot_size_;

            if (count > 0u)
            {
                entry* current = pool->free_ = this->slots_;

                for (std::uint32_t i = 1u; i < count; ++i)
                {
                    current = current->next = reinterpret_cast<entry*>(reinterpret_cast<uintptr_t>(this->slots_) + this->slot_size_ * i);
                }

                current->next = nullptr;
            }
        }

        this->alloc_count_ = 0u;
    }

    auto slot_pool::malloc() -> void*
    {
        if (this->free_ == nullptr)
        {
            if ((this->flags_ & flags::warn_if_overflow) != 0)
            {
                ::printf("Warning: trying to allocate a slot at pool \"%s\" which is out of free entries!", this->name_);
            }

            if ((this->flags_ & flags::overflow_if_full) != 0)
            {
                this->expand((this->slot_count_ >> 2) + 1);
            }
            else
            {
                assert(false);
            }
        }

        entry* result = this->free_;

        if (result != nullptr)
        {
            this->free_ = result->next;

            this->alloc_count_++;

            this->most_alloc_count_ = math::max(this->most_alloc_count_, this->alloc_count_);

            if ((this->flags_ & flags::zero_allocated_memory) != 0)
            {
                ::memset(result, 0, this->slot_size_);
            }
        }

        return result;
    }

    auto slot_pool::malloc_array(alloc_size_t count, entry** last_slot) -> entry*
    {
        if (count == 0u)
        {
            return nullptr;
        }

        if (this->free_slots() < count)
        {
            if ((this->flags_ & flags::warn_if_overflow) != 0)
            {
                ::printf("Warning: trying to allocate a slot at pool \"%s\" which is out of free entries!", this->name_);
            }

            if ((this->flags_ & flags::overflow_if_full) != 0)
            {
                this->expand(math::max(count, (this->slot_count_ >> 2) + 1));
            }
            else
            {
                assert(false);
            }
        }

        entry* result = this->free_;

        if (result == nullptr)
        {
            return nullptr;
        }

        entry* last = result;

        for (alloc_size_t i = count - 1; i > 0; --i)
        {
            last = last->next;

            if (last == nullptr)
            {
                return nullptr;
            }
        }

        this->free_ = last->next;

        last->next = nullptr;

        if (last_slot != nullptr)
        {
            *last_slot = last;
        }

        this->alloc_count_ += count;

        this->most_alloc_count_ = math::max(this->most_alloc_count_, this->alloc_count_);

        if ((this->flags_ & flags::zero_allocated_memory) != 0)
        {
            size_t size = static_cast<size_t>(this->slot_size_) - sizeof(entry);

            if (size > 0)
            {
                for (entry* i = result; i != nullptr; i = i->next)
                {
                    ::memset(i + 1, 0, size);
                }
            }
        }

        return result;
    }

    void slot_pool::free(void* ptr)
    {
        assert(this->get_slot_number(ptr) < this->slot_total_);

        this->alloc_count_--;

        reinterpret_cast<entry*>(ptr)->next = this->free_;

        this->free_ = reinterpret_cast<entry*>(ptr);
    }

    void slot_pool::free_array(entry* first_slot, entry** last_slot)
    {
        if (first_slot != nullptr)
        {
            if (last_slot != nullptr)
            {
                *last_slot = nullptr;
            }

            entry* last = first_slot;

            while (last->next != nullptr)
            {
                last = last->next;

                this->alloc_count_--;
            }

            this->alloc_count_--;

            last->next = this->free_;

            this->free_ = first_slot;
        }
    }

    void slot_pool::expand(alloc_size_t extra_slot_count)
    {
        slot_pool* pool = slot_pool_manager::new_slot_pool(this->slot_size_, extra_slot_count, this->name_, this->pool_type_);

        pool->flags_ = this->flags_;

        if (this->next_ == nullptr)
        {
            this->next_ = pool;
        }
        else
        {
            slot_pool* next = this->next_;

            while (next->next_ != nullptr)
            {
                next = next->next_;
            }

            next->next_ = pool;
        }

        uintptr_t last = reinterpret_cast<uintptr_t>(pool->slots_) + this->slot_size_ * (extra_slot_count - 1u);

        reinterpret_cast<entry*>(last)->next = this->free_;

        this->free_ = pool->free_;

        pool->free_ = nullptr;

        this->slot_total_ += extra_slot_count;
    }

    auto slot_pool::get_slot(alloc_size_t index) -> void*
    {
        if (index < this->slot_total_)
        {
            for (slot_pool* i = this; i != nullptr; i = i->next_)
            {
                if (index < i->slot_count_)
                {
                    return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(i->slots_) + i->slot_size_ * index);
                }

                index -= i->slot_count_;
            }
        }

        return nullptr;
    }

    auto slot_pool::get_slot_number(const void* ptr) const -> alloc_size_t
    {
        alloc_size_t offset = 0;

        for (const slot_pool* pool = this; pool != nullptr; pool = pool->next_)
        {
            uintptr_t difference = reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(pool->slots_);

            alloc_size_t index = static_cast<alloc_size_t>(difference / pool->slot_size_);

            if (index < pool->slot_count_)
            {
                return offset + index;
            }

            offset += pool->slot_count_;
        }

        return std::numeric_limits<alloc_size_t>::max();
    }

    auto slot_pool::get_allocated_slot(alloc_size_t index) -> void*
    {
        if (this->slot_total_ == 0u || index >= this->slot_total_)
        {
            return nullptr;
        }

        std::uint32_t params = memory::create_allocation_params(memory::pool_type::main, true, false, 0x00u, 0x00u);
#if defined(_DEBUG)
        span<bool> alloc_table(reinterpret_cast<bool*>(memory::malloc_debug(this->slot_total_, params, __FILE__, __LINE__)), this->slot_total_);
#else
        span<bool> alloc_table(reinterpret_cast<bool*>(memory::malloc(this->slot_total_, params)), this->slot_total_);
#endif
        alloc_table.clear();

        for (const entry* i = this->free_; i != nullptr; i = i->next)
        {
            alloc_table[this->get_slot_number(i)] = true;
        }

        for (size_t i = 0, k = 0; i < alloc_table.length(); ++i)
        {
            if (!alloc_table[i] && k++ == index)
            {
                const slot_pool* pool = this;

                while (i >= pool->slot_count_)
                {
                    i -= pool->slot_count_;

                    pool = pool->next_;
                }

                return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pool->slots_) + this->slot_size_ * i);
            }
        }

        return nullptr;
    }

    void slot_pool::trim()
    {
        if (this->next_ != nullptr)
        {
            for (slot_pool* i = this->next_; i != nullptr; i = i->next_)
            {
                i->alloc_count_ = i->slot_count_;
            }

            for (entry* e = this->free_; e != nullptr; e = e->next)
            {
                for (slot_pool* i = this->next_; i != nullptr; i = i->next_)
                {
                    uintptr_t difference = reinterpret_cast<uintptr_t>(e) - reinterpret_cast<uintptr_t>(i->slots_);

                    alloc_size_t index = static_cast<alloc_size_t>(difference / i->slot_size_);

                    if (index < i->slot_count_)
                    {
                        i->alloc_count_--;

                        break;
                    }
                }
            }

            slot_pool* prev = this;
            slot_pool* curr = this->next_;

            while (curr != nullptr)
            {
                if (curr->alloc_count_ == 0u)
                {
                    entry* free = this->free_;
                    entry** dst = &this->free_;

                    while (free != nullptr)
                    {
                        uintptr_t difference = reinterpret_cast<uintptr_t>(free) - reinterpret_cast<uintptr_t>(curr->slots_);

                        alloc_size_t index = static_cast<alloc_size_t>(difference / curr->slot_size_);

                        if (index < curr->slot_count_)
                        {
                            *dst = free->next;
                        }
                        else
                        {
                            dst = &free->next;
                        }

                        free = free->next;
                    }

                    this->slot_total_ -= curr->slot_count_;

                    prev->next_ = curr->next_;

                    memory::free(curr);

                    curr = prev->next_;
                }
                else
                {
                    prev = curr;
                    curr = curr->next_;
                }
            }
        }
    }

    auto slot_pool_manager::new_slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type) -> slot_pool*
    {
        slot_size = math::align_pow_2(slot_size, sizeof(uintptr_t));

        std::uint32_t params = memory::create_allocation_params(type, true, true, 0x10u, 0x00u);
#if defined(_DEBUG)
        slot_pool* pool = reinterpret_cast<slot_pool*>(memory::malloc_debug(slot_pool::header_size() + slot_size * slot_count, params, __FILE__, __LINE__));
#else
        slot_pool* pool = reinterpret_cast<slot_pool*>(memory::malloc(slot_pool::header_size() + slot_size * slot_count, params));
#endif
        if (pool != nullptr)
        {
            new (pool) slot_pool(slot_size, slot_count, name, type);
        }

        return pool;
    }

    void slot_pool_manager::delete_slot_pool(slot_pool* pool)
    {
        if (pool != nullptr)
        {
            if ((pool->get_flags() & slot_pool::flags::warn_if_nonempty_delete) != 0 && pool->alloc_slots() > 0u)
            {
                ::printf("Warning: deleting slot pool \"%s\" when it has 1 or more allocated slots!", pool->name());
            }

            do
            {
                slot_pool* next = pool->next();

                memory::free(pool);

                pool = next;

            } while (pool != nullptr);
        }
    }
}
