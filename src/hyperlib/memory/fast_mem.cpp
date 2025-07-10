#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/fast_mem.hpp>

namespace hyper
{
    void fast_mem::emergency_initialization(std::uint32_t& bytes, const char*& name, std::uint32_t& expansion_size, std::uint32_t& track_max)
    {
        bytes = 0x232000u;
        name = "gFastMem";
        expansion_size = 0x2000u;
        track_max = 0x19Cu;
    }

    void fast_mem::core_setup()
    {
        for (std::uint32_t i = 0u; i < std::size(this->free_lists_); ++i)
        {
            this->free_lists_[i] = nullptr;
            this->alloc_[i] = 0u;
            this->avail_[i] = 0u;
        }

        this->block_ = reinterpret_cast<fast_mem::block_desc*>(this->core_alloc(this->bytes_, this->name_));

        if (this->expansion_size_ != 0u)
        {
            fast_mem::block_desc* desc = this->block_;

            desc->next = nullptr;
            desc->size = this->bytes_;

            this->used_ = sizeof(fast_mem::block_desc);
        }

        this->inited_ = true;
    }

    void fast_mem::init()
    {
        if (!this->inited_)
        {
            fast_mem::emergency_initialization(this->bytes_, this->name_, this->expansion_size_, this->track_max_);

            this->core_setup();
        }
    }

    void fast_mem::deinit()
    {
        if (this->inited_)
        {
            for (fast_mem::block_desc* desc = this->block_; desc != nullptr; /* empty */)
            {
                fast_mem::block_desc* next = nullptr;

                if (this->expansion_size_ != 0u)
                {
                    next = desc->next;
                }

                this->core_free(desc);

                desc = next;
            }

            this->alloc_over_ = 0u;
            this->name_ = nullptr;
            this->block_ = nullptr;
            this->bytes_ = 0u;
            this->used_ = 0u;
            this->donated_ = 0u;
            this->expansion_size_ = 0u;

            for (std::uint32_t i = 0u; i < std::size(this->free_lists_); ++i)
            {
                this->free_lists_[i] = nullptr;
                this->alloc_[i] = 0u;
                this->avail_[i] = 0u;
            }
        }
    }

    bool fast_mem::assign_to_free(std::uint32_t size)
    {
        if (this->bytes_ < this->used_ + size)
        {
            if (this->bytes_ - this->used_ > 0x10u && !math::is_aligned(this->used_, 0x20u))
            {
                this->assign_to_free(0x10u);
            }

            while (this->bytes_ - this->used_ > 0x20u)
            {
                this->assign_to_free(0x20u);
            }

            if (!this->split_or_expand(size))
            {
                return false;
            }
        }

        std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(this->block_) + this->used_;

        this->used_ += size;

        this->free(ptr, size, nullptr);
    }

    bool fast_mem::split_or_expand(std::uint32_t size)
    {
        if (this->expansion_size_ != 0u && this->locks_ == 0u)
        {
            fast_mem::block_desc* desc = reinterpret_cast<fast_mem::block_desc*>(this->core_alloc(this->expansion_size_, this->name_));

            if (desc != nullptr)
            {
                desc->next = this->block_;
                desc->size = this->expansion_size_;

                this->block_ = desc;
                this->bytes_ = this->expansion_size_;
                this->used_ = sizeof(fast_mem::block_desc);

                return true;
            }
        }

        return false;
    }

    bool fast_mem::create_block(std::uint32_t list_index)
    {
        this->init();

        std::uint32_t block_size = (list_index + 1u) << 4;

        if (!math::is_pow_2(block_size))
        {
            return this->assign_to_free(block_size);
        }

        if (list_index == 0u || math::is_aligned(this->used_, 0x20u) || this->assign_to_free(0x10u))
        {
            if (list_index > 1u)
            {
                for (std::uint32_t i = 0u; i < 3u && !math::is_aligned(this->used_, 0x40u); ++i)
                {
                    if (!this->assign_to_free(0x20u))
                    {
                        return false;
                    }
                }
            }

            if (list_index > 3u)
            {
                std::uint32_t align = ((block_size - 1u) & 0x7Fu) + 1u; // this just works?

                if (!math::is_aligned(this->used_, align))
                {
                    while (this->assign_to_free(0x40u))
                    {
                        if (math::is_aligned(this->used_, align))
                        {
                            return this->assign_to_free(block_size);
                        }
                    }

                    return false;
                }
            }

            return this->assign_to_free(block_size);
        }

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

    void fast_mem::core_free(void* ptr)
    {
        if (this->allocator_ == nullptr)
        {
            memory::free(ptr);
        }
        else
        {
            this->allocator_->free(ptr, 0u);
        }
    }

    fast_mem::~fast_mem()
    {
        if (this->block_ != nullptr)
        {
            this->deinit();
        }
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

    void fast_mem::free(void* ptr, alloc_size_t size, const char* debug_name)
    {
        if (size <= 0x400u)
        {
            std::uint32_t list_index = static_cast<std::uint32_t>((size - 1u) >> 4);

            fast_mem::free_block* block = reinterpret_cast<fast_mem::free_block*>(ptr);

            block->next = this->free_lists_[list_index];

            this->free_lists_[list_index] = block;
        }
        else
        {
            this->core_free(ptr);
        }
    }
}
