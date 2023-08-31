#include <hyperlib/assets/loader.hpp>
#include <hyperlib/memory/memory.hpp>

namespace hyper
{
    void loader::service_resource_loading()
    {
        call_function<void(__cdecl*)()>(0x006B68D0)();
    }

    void loader::set_delayed_resource_callback(void(*callback)(void*), void* param, bool non_recursive)
    {
        call_function<void(__cdecl*)(void(*)(void*), void*, bool)>(0x006997A0)(callback, param, non_recursive);
    }

    void loader::load_chunks(void* memory, size_t size, const char* debug_name)
    {
        call_function<void(__cdecl*)(void*, size_t, const char*)>(0x006AD8F0)(memory, size, debug_name);
    }

    void loader::unload_chunks(void* memory, size_t size)
    {
        call_function<void(__cdecl*)(void*, size_t)>(0x006A83A0)(memory, size);
    }

    void loader::verify_chunk_integrity(chunk* block, size_t size, std::uint32_t depth)
    {
        if (++depth > 10u)
        {
            ::printf("Chunks are internally corrupted! Exceeding maximum recursive depth!");

            NFSU360IFY();
        }

        const chunk* end = reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(block) + size);

        if (block != end)
        {
            chunk* curr = block;

            while (true)
            {
                chunk* next = curr->end();

                if (curr->is_container())
                {
                    loader::verify_chunk_integrity(reinterpret_cast<chunk*>(curr->data()), curr->size(), depth);
                }

                if (next == end)
                {
                    return;
                }

                if (next < block || next > end)
                {
                    break;
                }

                curr = next;
            }

            std::uint32_t address = static_cast<std::uint32_t>(reinterpret_cast<uintptr_t>(curr) - reinterpret_cast<uintptr_t>(block));
            std::uint32_t chunkid = static_cast<std::uint32_t>(curr->id());
            std::uint32_t chunksz = static_cast<std::uint32_t>(curr->size());

            ::printf("Warning: chunks are corrupted around 0x%08X (chunk ID is 0x%08X, size is 0x%08X)\n", address, chunkid, chunksz);

            NFSU360IFY();
        }
    }

    auto loader::split_temp_perm_chunks(bool count_temp, chunk* block, size_t size, void* dst, size_t dst_offset, std::uint32_t depth) -> size_t
    {
        if (++depth > 10u)
        {
            ::printf("Chunks are internally corrupted! Exceeding maximum recursive depth!");

            NFSU360IFY();
        }

        const chunk* end = reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(block) + size);

        for (chunk* curr = block; curr < end; curr = curr->end())
        {
            bool is_temp = curr->id() == block_id::texture_pack_vram && loader::split_vram_data;

            if (is_temp == count_temp)
            {
                std::uint32_t diff = static_cast<std::uint32_t>((reinterpret_cast<uintptr_t>(curr) - (reinterpret_cast<uintptr_t>(dst) + dst_offset)) & 0x7F);

                if (diff != 0u && diff <= 8u)
                {
                    diff += 0x80u;
                }

                if (dst != nullptr && diff != 0u)
                {
                    *reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(dst) + dst_offset) = chunk(block_id::empty, diff - 8u);

                    ::memset(reinterpret_cast<char*>(dst) + dst_offset + sizeof(chunk), 0x11, diff - 8u);
                }

                dst_offset += diff;
            }

            if (!curr->is_container() || is_temp)
            {
                if (is_temp == count_temp)
                {
                    if (dst != nullptr)
                    {
                        ::memcpy(reinterpret_cast<char*>(dst) + dst_offset, curr, curr->size() + sizeof(chunk));
                    }

                    dst_offset += curr->size() + sizeof(chunk);
                }
            }
            else
            {
                size_t next_offset = loader::split_temp_perm_chunks(count_temp, reinterpret_cast<chunk*>(curr->data()), curr->size(), dst, dst_offset + sizeof(chunk), depth);

                if (next_offset != dst_offset + sizeof(chunk))
                {
                    if (dst != nullptr)
                    {
                        *reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(dst) + dst_offset) = chunk(curr->id(), next_offset - dst_offset - sizeof(chunk));
                    }

                    dst_offset = next_offset;
                }
            }
        }

        return dst_offset;
    }
    
    void loader::clobber_perm_chunks(chunk* block, size_t size)
    {
        const chunk* end = reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(block) + size);

        chunk* curr = block;

        while (curr < end)
        {
            chunk* next = curr->end();

            bool is_temp = curr->id() == block_id::texture_pack_vram && loader::split_vram_data;

            if (!is_temp)
            {
                if (curr->is_container())
                {
                    loader::clobber_perm_chunks(reinterpret_cast<chunk*>(curr->data()), curr->size());

                    curr->set_size(0u);
                }

                curr->set_id(block_id::empty);
            }

            curr = next;
        }
    }

    bool loader::load_temp_perm_chunks(void** memory, size_t* size, std::uint32_t alloc_params, const char* debug_name)
    {
        assert(memory != nullptr);
        assert(size != nullptr);

        if (*memory == nullptr || *size == 0u)
        {
            return false;
        }

        chunk* block = reinterpret_cast<chunk*>(*memory);
        size_t count = *size;

        loader::verify_chunk_integrity(block, count, 0u);

        size_t perm_size = loader::split_temp_perm_chunks(false, block, count, nullptr, 0u, 0u);

        if (loader::split_temp_perm_chunks(true, block, count, nullptr, 0u, 0u))
        {
            alloc_params = hyper::memory::overwrite_alignment(alloc_params, 0x80u); // 0x0045529B (AnimLoader::LoadResourceFile)
#if defined(_DEBUG)
            void* perm_memory = hyper::memory::malloc_debug(perm_size, alloc_params, debug_name, 0u);
#else
            void* perm_memory = hyper::memory::malloc(perm_size, alloc_params);
#endif
            loader::split_temp_perm_chunks(false, block, count, perm_memory, 0u, 0u);

            loader::clobber_perm_chunks(block, count);

            loader::load_chunks(block, count, debug_name);

            loader::load_chunks(perm_memory, perm_size, debug_name);

            loader::unload_chunks(block, count);

            hyper::memory::free(block);

            *memory = perm_memory;
            *size = perm_size;

            return true;
        }
        else
        {
            loader::load_chunks(block, count, debug_name);

            return false;
        }
    }

    void loader::post_load_fixup()
    {
        call_function<void(__cdecl*)()>(0x006995D0)();
    }
}
