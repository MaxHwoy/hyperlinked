#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/memory/memory.hpp>

namespace hyper
{
    class bstl final
    {
    public:
        class allocator_base
        {
        public:
            inline auto get_name() const -> const char*
            {
                return this->name_;
            }

            inline void set_name(const char* name)
            {
                this->name_ = name;
            }
            
        private:
            const char* name_;
        };

        class allocator : public allocator_base
        {
        public:
            inline allocator() = default;

            inline allocator(const allocator&) = default;

            inline allocator(allocator&&) = default;

            inline allocator& operator=(const allocator&) = default;

            inline allocator& operator=(allocator&&) = default;

            inline allocator(const char* name)
            {
                this->set_name(name);
            }

            auto allocate(alloc_size_t size, std::uint32_t flags) -> void*;

            auto allocate(alloc_size_t size, alloc_size_t alignment, alloc_size_t offset, std::uint32_t flags) -> void*;

            void deallocate(void* memory, alloc_size_t size);

            inline bool operator==(const allocator& other)
            {
                static_cast<void>(other);

                return true;
            }

            inline bool operator!=(const allocator& other)
            {
                static_cast<void>(other);

                return false;
            }
        };
    };
}
