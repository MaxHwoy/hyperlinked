#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/memory.hpp>

namespace hyper
{
    class slot_pool : public linked_node<slot_pool>
    {
    public:
        struct entry
        {
            entry* next;
        };

        enum class flags : std::uint32_t
        {
            overflow_if_full        = 1u << 0,
            zero_allocated_memory   = 1u << 1,
            warn_if_overflow        = 1u << 2,
            warn_if_nonempty_delete = 1u << 3,
        };

    public:
        slot_pool();

        slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type);

        ~slot_pool();

        void flush();

        auto malloc() -> void*;

        auto malloc_array(alloc_size_t count, entry** last_slot) -> entry*;

        void free(void* ptr);

        void free_array(entry* first_slot, entry** last_slot);

        void expand(alloc_size_t extra_slot_count);
        
        auto get_slot(alloc_size_t index) -> void*;

        auto get_slot_number(const void* ptr) const -> alloc_size_t;

        auto get_allocated_slot(alloc_size_t index) -> void*;

        void trim();

        inline auto next() -> slot_pool*
        {
            return this->next_;
        }

        inline auto name() const -> const char*
        {
            return this->name_;
        }

        inline bool is_full() const
        {
            return this->slot_total_ == this->alloc_count_;
        }

        inline bool is_empty() const
        {
            return this->alloc_count_ == 0;
        }

        inline auto total_slots() const -> alloc_size_t
        {
            return this->slot_total_;
        }

        inline auto alloc_slots() const -> alloc_size_t
        {
            return this->alloc_count_;
        }

        inline auto free_slots() const -> alloc_size_t
        {
            return this->slot_total_ - this->alloc_count_;
        }

        inline auto get_flags() const -> flags
        {
            return this->flags_;
        }

        inline void set_flags(flags value)
        {
            this->flags_ = static_cast<flags>(static_cast<std::uint32_t>(this->flags_) | static_cast<std::uint32_t>(value));
        }

        inline void clear_flags(flags value)
        {
            this->flags_ = static_cast<flags>(static_cast<std::uint32_t>(this->flags_) & ~static_cast<std::uint32_t>(value));
        }

        inline bool overflowed() const
        {
            return this->slot_total_ > this->slot_count_;
        }

        inline bool is_in_pool(const void* ptr) const
        {
            return this->get_slot_number(ptr) != std::numeric_limits<alloc_size_t>::max();
        }

        constexpr static auto header_size() -> alloc_size_t;

    private:
        slot_pool* next_;
        const char* name_;
        entry* free_;
        flags flags_;
        alloc_size_t alloc_count_;
        alloc_size_t most_alloc_count_;
        memory::pool_type pool_type_;
        alloc_size_t slot_count_;
        alloc_size_t slot_size_;
        alloc_size_t slot_total_;
        entry slots_[1];
    };

    template <typename T> class instance_pool : public slot_pool
    {
    public:
        inline instance_pool(alloc_size_t slot_count, const char* name, memory::pool_type type) : slot_pool(sizeof(T), slot_count, name, type)
        {
        }

        inline auto get_allocated_instance(alloc_size_t index) -> T*
        {
            return reinterpret_cast<T*>(this->get_allocated_slot(index));
        }

        template <typename... Args> auto construct(Args&&... args) -> T*;

        void destruct(T* instance);
    };

    class slot_pool_manager final
    {
    public:
        static auto new_slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type) -> slot_pool*;

        static void delete_slot_pool(slot_pool* pool);

    public:
        inline slot_pool_manager() : initialized_(false), pools_()
        {
        }

        inline bool initialized()
        {
            return this->initialized_;
        }
        
        inline auto create_slot_pool(alloc_size_t slot_size, alloc_size_t slot_count, const char* name, memory::pool_type type) -> slot_pool*
        {
            slot_pool* pool = slot_pool_manager::new_slot_pool(slot_size, slot_count, name, type);

            if (pool != nullptr && this->initialized_)
            {
                this->pools_.add(pool);
            }

            return pool;
        }

        inline void remove_slot_pool(slot_pool* pool)
        {
            if (pool != nullptr)
            {
                if (this->initialized_)
                {
                    this->pools_.remove(pool);
                }

                slot_pool_manager::delete_slot_pool(pool);
            }
        }

        inline void cleanup_expanded_slot_pools()
        {
            if (this->initialized_)
            {
                for (slot_pool* i = this->pools_.begin(); i != this->pools_.end(); i = i->next())
                {
                    i->trim();
                }
            }
        }

        template <typename T> static auto new_slot_pool(alloc_size_t slot_count, const char* name, memory::pool_type type) -> instance_pool<T>*
        {
            return reinterpret_cast<instance_pool<T>*>(slot_pool_manager::new_slot_pool(static_cast<alloc_size_t>(sizeof(T)), slot_count, name, type));
        }

        template <typename T> static void delete_slot_pool(instance_pool<T>* pool)
        {
            slot_pool_manager::delete_slot_pool(reinterpret_cast<slot_pool*>(pool));
        }

    private:
        bool initialized_;
        linked_list<slot_pool> pools_;

    public:
        static inline slot_pool_manager& instance = *reinterpret_cast<slot_pool_manager*>(0x00A87C68);
    };

    template <typename T> template <typename... Args> auto instance_pool<T>::construct(Args&&... args) -> T*
    {
        void* ptr = this->malloc();
        
        if (ptr != nullptr)
        {
            return new (ptr) T(std::forward<Args>(args)...);
        }

        return nullptr;
    }

    template <typename T> void instance_pool<T>::destruct(T* instance)
    {
        instance->~T();

        this->free(instance);
    }

    CREATE_ENUM_FLAG_OPERATORS(slot_pool::flags);

    ASSERT_SIZE(slot_pool, 0x30 + sizeof(slot_pool::entry));
    ASSERT_SIZE(slot_pool_manager, 0x0C);
}
