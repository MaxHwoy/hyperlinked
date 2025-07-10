#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/fast_mem.hpp>

namespace hyper
{
    bool fast_mem::create_block(std::uint32_t list_index)
    {
        return false;
    }

    auto fast_mem::core_alloc(alloc_size_t size, const char* debug_name) -> void*
    {
        if (this->allocator_ != nullptr)
        {
            ea_allocator::tag_value_pair pairs[3]{};

            pairs[0].next = &pairs[1];
            pairs[1].next = &pairs[2];

            pairs[0].tag = ea_allocator::tag_type::start_from_bottom;
            pairs[1].tag = ea_allocator::tag_type::debug_name;

            pairs[0].value.vint = 1;
            pairs[1].value.pointer = debug_name;

            return this->allocator_->alloc(size, pairs);
        }

#if defined(_DEBUG)
        return memory::malloc_debug(size, memory::create_allocation_params(memory::pool_type::main, false, false, 0u, 0u), debug_name, 0u);
#else
        return memory::malloc(size, memory::create_allocation_params(memory::pool_type::main, false, false, 0u, 0u));
#endif
    }

    auto fast_mem::malloc(alloc_size_t size, const char* debug_name) -> void*
    {
        fast_mem::free_block* block;

        if (size <= 0x400u)
        {
            std::uint32_t list_index = static_cast<std::uint32_t>((size - 1u) >> 4);

            if (this->free_lists_[list_index] == nullptr && !this->create_block(list_index))
            {
                HYPER_FAIL("Out of FastMem memory at list index %d when requesting allocation of size %d", list_index, size);
            }

            block = this->free_lists_[list_index];

            this->free_lists_[list_index] = block->next;
        }
        else
        {
            block = reinterpret_cast<fast_mem::free_block*>(this->core_alloc(size, debug_name));
        }

        ::memset(block, 0, size);

        return block;
    }

    void fast_mem::free(void* memory, alloc_size_t size, const char* debug_name)
    {
        call_function<void(__thiscall*)(fast_mem*, void*, alloc_size_t, const char*)>(0x00609E80)(this, memory, size, debug_name);
    }
}
