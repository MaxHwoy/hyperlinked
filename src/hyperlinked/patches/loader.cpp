#include <hyperlib/assets/loader.hpp>
#include <hyperlinked/patches/loader.hpp>

namespace hyper
{
    __declspec(naked) void detour_endian_swap_chunks_if_needed()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'
            // [esp + 0x08] is 'size'
            // [esp + 0x0C] is 'debug_name'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'debug_name' is now at [esp + 0x10]
            push ebx; // 'debug_name' is now at [esp + 0x14]
            push ecx; // 'debug_name' is now at [esp + 0x18]
            push edx; // 'debug_name' is now at [esp + 0x1C]
            push esi; // 'debug_name' is now at [esp + 0x20]
            push edi; // 'debug_name' is now at [esp + 0x24]

            push 0;            // push   'depth'
            push [esp + 0x24]; // repush 'size'
            push [esp + 0x24]; // repush 'block'

            call loader::verify_chunk_integrity; // call custom verify_chunk_integrity

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to EndianSwapChunksIfNeeded
        }
    }

    __declspec(naked) void detour_load_temp_perm_chunks()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'memory'
            // [esp + 0x08] is 'size'
            // [esp + 0x0C] is 'alloc_params'
            // [esp + 0x10] is 'debug_name'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'debug_name' is now at [esp + 0x14]
            push ecx; // 'debug_name' is now at [esp + 0x18]
            push edx; // 'debug_name' is now at [esp + 0x1C]
            push esi; // 'debug_name' is now at [esp + 0x20]
            push edi; // 'debug_name' is now at [esp + 0x24]
            
            push [esp + 0x24]; // repush 'debug_name'
            push [esp + 0x24]; // repush 'alloc_params'
            push [esp + 0x24]; // repush 'size'
            push [esp + 0x24]; // repush 'memory'

            call loader::load_temp_perm_chunks; // call custom load_temp_perm_chunks
            
            add esp, 0x10; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to LoadTempPermChunks
        }
    }

    void loader_patches::init()
    {
        // EndianSwapChunksIfNeeded
        hook::jump(0x006AD870, &detour_endian_swap_chunks_if_needed);

        // LoadTempPermChunks
        hook::jump(0x006ADBC0, &detour_load_temp_perm_chunks);
    }
}
