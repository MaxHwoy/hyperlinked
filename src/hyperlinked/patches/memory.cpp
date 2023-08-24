#include <hyperlib/hook.hpp>
#include <hyperlib/math.hpp>
#include <hyperlib/memory/memory.hpp>
#include <hyperlib/memory/slot_pool.hpp>
#include <hyperlib/memory/ts_memory_pool.hpp>
#include <hyperlib/streamer/streamer.hpp>

#include <hyperlinked/patches/memory.hpp>
#include <hyperlinked/car_loader.hpp>

namespace hyper
{
    void init_car_loader_pool(void* __this, alloc_size_t size)
    {
        car_loader* loader = reinterpret_cast<car_loader*>(__this);

        loader->num_sponge_allocations = 0u;

#if defined(_DEBUG)
        loader->memory_pool_size = static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max());
        loader->memory_pool_heap = memory::malloc_debug(size, memory::create_allocation_params(memory::pool_type::streamer, false, false, 0x00u, 0x00u), __FILE__, __LINE__);
#else
        loader->memory_pool_size = static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max());
        loader->memory_pool_heap = memory::malloc(size, memory::create_allocation_params(memory::pool_type::streamer, false, false, 0x00u, 0x00u));
#endif
        memory::pool_type type = static_cast<memory::pool_type>(memory::get_free_memory_pool_number());

        memory::initialize_pool(type, loader->memory_pool_heap, size, reinterpret_cast<const char*>(0x009F264C));

        memory::set_memory_pool_limitness(type, true);
        memory::set_memory_pool_debug_fill(type, false);
        memory::set_memory_pool_top_direction(type, true);

        *reinterpret_cast<memory::pool_type*>(0x00A7974C) = type;
    }

    __declspec(naked) void detour_b_memory_init()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // save previous register
            push ebx; // save previous register
            push ecx; // save previous register
            push edx; // save previous register
            push esi; // save previous register
            push edi; // save previous register
            
            call memory::initialize; // call custom initialize
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bMemoryInit
        }
    }

    __declspec(naked) void detour_b_memory_close()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // save previous register
            push ebx; // save previous register
            push ecx; // save previous register
            push edx; // save previous register
            push esi; // save previous register
            push edi; // save previous register
            
            call memory::close; // call custom close
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bMemoryClose
        }
    }

    __declspec(naked) void detour_b_init_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'address'
            // [esp + 0x0C] is 'size'
            // [esp + 0x10] is 'debug_name'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'debug_name' is now at [esp + 0x14]
            push ebx; // 'debug_name' is now at [esp + 0x18]
            push ecx; // 'debug_name' is now at [esp + 0x1C]
            push edx; // 'debug_name' is now at [esp + 0x20]
            push esi; // 'debug_name' is now at [esp + 0x24]
            push edi; // 'debug_name' is now at [esp + 0x28]
            
            push [esp + 0x28]; // repush 'debug_name'
            push [esp + 0x28]; // repush 'size'
            push [esp + 0x28]; // repush 'address'
            push [esp + 0x28]; // repush 'type'
            
            call memory::initialize_pool; // call custom initialize_pool
            
            add esp, 0x10; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bInitMemoryPool
        }
    }

    __declspec(naked) void detour_b_close_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'type' is now at [esp + 0x08]
            push ebx; // 'type' is now at [esp + 0x0C]
            push ecx; // 'type' is now at [esp + 0x10]
            push edx; // 'type' is now at [esp + 0x14]
            push esi; // 'type' is now at [esp + 0x18]
            push edi; // 'type' is now at [esp + 0x1C]
            
            push [esp + 0x1C]; // repush 'type'
            
            call memory::close_pool; // call custom close_pool
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bCloseMemoryPool
        }
    }

    __declspec(naked) void detour_b_memory_create_persistent_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'size' is now at [esp + 0x08]
            push ebx; // 'size' is now at [esp + 0x0C]
            push ecx; // 'size' is now at [esp + 0x10]
            push edx; // 'size' is now at [esp + 0x14]
            push esi; // 'size' is now at [esp + 0x18]
            push edi; // 'size' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'size'

            call memory::create_persistent_pool; // call custom create_persistent_pool

            add esp, 0x04; // since we repushed all arguments

            mov dword ptr ds : [0x00A4E2AC], eax; // bMemoryPersistentPoolNumber
            mov dword ptr ds : [0x00A87C40], eax; // TheMemoryPersistentAllocator
            mov dword ptr ds : [0x00A87C28], eax; // ResourceAllocator.PoolNumber

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to bMemoryCreatePersistentPool
        }
    }

    __declspec(naked) void detour_b_memory_get_allocation_number()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // save previous register
            push ecx; // save previous register
            push edx; // save previous register
            push esi; // save previous register
            push edi; // save previous register
            
            call memory::get_allocation_count; // call custom get_allocation_count
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bMemoryGetAllocationNumber
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_override_info()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'override'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'override' is now at [esp + 0x0C]
            push ebx; // 'override' is now at [esp + 0x10]
            push ecx; // 'override' is now at [esp + 0x14]
            push edx; // 'override' is now at [esp + 0x18]
            push esi; // 'override' is now at [esp + 0x1C]
            push edi; // 'override' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'override'
            push [esp + 0x20]; // repush 'type'
            
            call memory::set_pool_override_info; // call custom set_pool_override_info
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolOverrideInfo
        }
    }

    __declspec(naked) void detour_b_reserve_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'type' is now at [esp + 0x08]
            push ebx; // 'type' is now at [esp + 0x0C]
            push ecx; // 'type' is now at [esp + 0x10]
            push edx; // 'type' is now at [esp + 0x14]
            push esi; // 'type' is now at [esp + 0x18]
            push edi; // 'type' is now at [esp + 0x1C]
            
            push [esp + 0x1C]; // repush 'type'
            
            call memory::reserve_memory_pool; // call custom reserve_memory_pool
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bReserveMemoryPool
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_debug_fill()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'fill'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'fill' is now at [esp + 0x0C]
            push ebx; // 'fill' is now at [esp + 0x10]
            push ecx; // 'fill' is now at [esp + 0x14]
            push edx; // 'fill' is now at [esp + 0x18]
            push esi; // 'fill' is now at [esp + 0x1C]
            push edi; // 'fill' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'fill'
            push [esp + 0x20]; // repush 'type'
            
            call memory::set_memory_pool_debug_fill; // call custom set_memory_pool_debug_fill
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolDebugFill
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_debug_tracing()
    {
        __asm
        {
            // this function is completely useless, return immediately
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolDebugTracing
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_top_direction()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'top_means_larger_address'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'top_means_larger_address' is now at [esp + 0x0C]
            push ebx; // 'top_means_larger_address' is now at [esp + 0x10]
            push ecx; // 'top_means_larger_address' is now at [esp + 0x14]
            push edx; // 'top_means_larger_address' is now at [esp + 0x18]
            push esi; // 'top_means_larger_address' is now at [esp + 0x1C]
            push edi; // 'top_means_larger_address' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'top_means_larger_address'
            push [esp + 0x20]; // repush 'type'
            
            call memory::set_memory_pool_top_direction; // call custom set_memory_pool_top_direction
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolTopDirection
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_default_alignment()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'alignment'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'alignment' is now at [esp + 0x0C]
            push ebx; // 'alignment' is now at [esp + 0x10]
            push ecx; // 'alignment' is now at [esp + 0x14]
            push edx; // 'alignment' is now at [esp + 0x18]
            push esi; // 'alignment' is now at [esp + 0x1C]
            push edi; // 'alignment' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'alignment'
            push [esp + 0x20]; // repush 'type'
            
            call memory::set_memory_pool_default_alignment; // call custom set_memory_pool_default_alignment
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolDefaultAlignment
        }
    }

    __declspec(naked) void detour_b_set_memory_pool_overflow_number()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'number'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'number' is now at [esp + 0x0C]
            push ebx; // 'number' is now at [esp + 0x10]
            push ecx; // 'number' is now at [esp + 0x14]
            push edx; // 'number' is now at [esp + 0x18]
            push esi; // 'number' is now at [esp + 0x1C]
            push edi; // 'number' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'number'
            push [esp + 0x20]; // repush 'type'
            
            call memory::set_memory_pool_overflow_number; // call custom set_memory_pool_overflow_number
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bSetMemoryPoolOverflowNumber
        }
    }

    __declspec(naked) void detour_b_verify_pool_integrity()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'type' is now at [esp + 0x08]
            push ebx; // 'type' is now at [esp + 0x0C]
            push ecx; // 'type' is now at [esp + 0x10]
            push edx; // 'type' is now at [esp + 0x14]
            push esi; // 'type' is now at [esp + 0x18]
            push edi; // 'type' is now at [esp + 0x1C]
            
            push [esp + 0x1C]; // repush 'type'
            
            call memory::verify_pool_integrity; // call custom verify_pool_integrity
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bVerifyPoolIntegrity
        }
    }

    __declspec(naked) void detour_b_is_in_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'ptr'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'ptr' is now at [esp + 0x0C]
            push ecx; // 'ptr' is now at [esp + 0x10]
            push edx; // 'ptr' is now at [esp + 0x14]
            push esi; // 'ptr' is now at [esp + 0x18]
            push edi; // 'ptr' is now at [esp + 0x1C]
            
            push [esp + 0x1C]; // repush 'ptr'
            push [esp + 0x1C]; // repush 'type'
            
            call memory::is_in_memory_pool; // call custom is_in_memory_pool
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bIsInMemoryPool
        }
    }
    
    __declspec(naked) void detour_b_get_malloc_size()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'ptr'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'ptr' is now at [esp + 0x08]
            push ecx; // 'ptr' is now at [esp + 0x0C]
            push edx; // 'ptr' is now at [esp + 0x10]
            push esi; // 'ptr' is now at [esp + 0x14]
            push edi; // 'ptr' is now at [esp + 0x18]
            
            push [esp + 0x18]; // repush 'ptr'
            
            call memory::get_malloc_size; // call custom get_malloc_size
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bGetMallocSize
        }
    }

    __declspec(naked) void detour_b_memory_get_allocations()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'allocations'
            // [esp + 0x0C] is 'max_allocations'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'max_allocations' is now at [esp + 0x10]
            push ecx; // 'max_allocations' is now at [esp + 0x14]
            push edx; // 'max_allocations' is now at [esp + 0x18]
            push esi; // 'max_allocations' is now at [esp + 0x1C]
            push edi; // 'max_allocations' is now at [esp + 0x20]
            
            push [esp + 0x20]; // repush 'max_allocations'
            push [esp + 0x20]; // repush 'allocations'
            push [esp + 0x20]; // repush 'type'
            
            call memory::get_pool_allocations; // call custom get_pool_allocations
            
            add esp, 0x0C; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bMemoryGetAllocations
        }
    }

    __declspec(naked) void detour_b_memory_print_allocations_by_address()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            // [esp + 0x08] is 'from_address' "unused"
            // [esp + 0x0C] is 'to_address' "unused"
            
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push eax; // 'type' is now at [esp + 0x08]
            push ebx; // 'type' is now at [esp + 0x0C]
            push ecx; // 'type' is now at [esp + 0x10]
            push edx; // 'type' is now at [esp + 0x14]
            push esi; // 'type' is now at [esp + 0x18]
            push edi; // 'type' is now at [esp + 0x1C]
            
            push [esp + 0x1C]; // repush 'type'
            
            call memory::print_pool_allocations; // call custom print_pool_allocations
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register
            
            retn; // return immediately to caller function, not back to bMemoryPrintAllocationsByAddress
        }
    }

    __declspec(naked) void detour_b_get_free_memory_pool_num()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // save previous register
            push ecx; // save previous register
            push edx; // save previous register
            push esi; // save previous register
            push edi; // save previous register

            call memory::get_free_memory_pool_number; // call custom get_free_memory_pool_number

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bGetFreeMemoryPoolNum
        }
    }

    __declspec(naked) void detour_b_get_memory_pool_debug_name()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'type' is now at [esp + 0x08]
            push ecx; // 'type' is now at [esp + 0x0C]
            push edx; // 'type' is now at [esp + 0x10]
            push esi; // 'type' is now at [esp + 0x14]
            push edi; // 'type' is now at [esp + 0x18]
            
            push [esp + 0x18]; // repush 'type'
            
            call memory::get_memory_pool_name; // call custom get_memory_pool_name
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bGetMemoryPoolDebugName
        }
    }

    __declspec(naked) void detour_b_get_memory_pool_num()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'name'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'name' is now at [esp + 0x08]
            push ecx; // 'name' is now at [esp + 0x0C]
            push edx; // 'name' is now at [esp + 0x10]
            push esi; // 'name' is now at [esp + 0x14]
            push edi; // 'name' is now at [esp + 0x18]
            
            push [esp + 0x18]; // repush 'name'
            
            call memory::get_memory_pool_index; // call custom get_memory_pool_index
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bGetMemoryPoolNum
        }
    }

    __declspec(naked) void detour_b_largest_malloc()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'params'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'params' is now at [esp + 0x08]
            push ecx; // 'params' is now at [esp + 0x0C]
            push edx; // 'params' is now at [esp + 0x10]
            push esi; // 'params' is now at [esp + 0x14]
            push edi; // 'params' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'params'

            call memory::largest_malloc; // call custom largest_malloc

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bLargestMalloc
        }
    }

    __declspec(naked) void detour_b_count_free_memory()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'type'
            
            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return
            
            push ebx; // 'type' is now at [esp + 0x08]
            push ecx; // 'type' is now at [esp + 0x0C]
            push edx; // 'type' is now at [esp + 0x10]
            push esi; // 'type' is now at [esp + 0x14]
            push edi; // 'type' is now at [esp + 0x18]
            
            push [esp + 0x18]; // repush 'type'
            
            call memory::count_free_memory; // call custom count_free_memory
            
            add esp, 0x04; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            
            retn; // return immediately to caller function, not back to bCountFreeMemory
        }
    }

    __declspec(naked) void detour_b_ware_malloc()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'
            // [esp + 0x08] is 'debug_text'
            // [esp + 0x0C] is 'debug_line'
            // [esp + 0x10] is 'params'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'params' is now at [esp + 0x14]
            push ecx; // 'params' is now at [esp + 0x18]
            push edx; // 'params' is now at [esp + 0x1C]
            push esi; // 'params' is now at [esp + 0x20]
            push edi; // 'params' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'params'
            push [esp + 0x18]; // repush 'return address'
            push [esp + 0x20]; // repush 'size'

            call memory::game_malloc; // call custom ware_malloc

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bWareMalloc
        }
    }

    __declspec(naked) void detour_b_malloc()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'size'
            // [esp + 0x08] is 'params'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'params' is now at [esp + 0x0C]
            push ecx; // 'params' is now at [esp + 0x10]
            push edx; // 'params' is now at [esp + 0x14]
            push esi; // 'params' is now at [esp + 0x18]
            push edi; // 'params' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'params'
            push [esp + 0x18]; // repush 'return address'
            push [esp + 0x20]; // repush 'size'

            call memory::game_malloc; // call custom malloc

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bMalloc
        }
    }

    __declspec(naked) void detour_b_free()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'ptr'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'ptr' is now at [esp + 0x08]
            push ebx; // 'ptr' is now at [esp + 0x0C]
            push ecx; // 'ptr' is now at [esp + 0x10]
            push edx; // 'ptr' is now at [esp + 0x14]
            push esi; // 'ptr' is now at [esp + 0x18]
            push edi; // 'ptr' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'ptr'

            call memory::free; // call custom free

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to bFree
        }
    }

    __declspec(naked) void detour_slot_pool_cleanup_expanded_slot_pools()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to slot pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call slot_pool::trim; // call custom trim

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SlotPool::CleanupExpandedSlotPools
        }
    }

    __declspec(naked) void detour_slot_pool_expand_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'extra_slot_count'
            // ecx contains pointer to slot pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'extra_slot_count' is now at [esp + 0x08]
            push ebx; // 'extra_slot_count' is now at [esp + 0x0C]
            push ecx; // 'extra_slot_count' is now at [esp + 0x10]
            push edx; // 'extra_slot_count' is now at [esp + 0x14]
            push esi; // 'extra_slot_count' is now at [esp + 0x18]
            push edi; // 'extra_slot_count' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'extra_slot_count'

            call slot_pool::expand; // call custom expand

            // no need to restore esp since 'expand' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to SlotPool::ExpandSlotPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_flush_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to slot pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call slot_pool::flush; // call custom flush

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SlotPool::FlushSlotPool
        }
    }

    __declspec(naked) void detour_slot_pool_get_allocated_slot()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'index'
            // ecx contains pointer to slot pool

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'index' is now at [esp + 0x08]
            push ecx; // 'index' is now at [esp + 0x0C]
            push edx; // 'index' is now at [esp + 0x10]
            push esi; // 'index' is now at [esp + 0x14]
            push edi; // 'index' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'index'

            call slot_pool::get_allocated_slot; // call custom get_allocated_slot

            // no need to restore esp since 'get_allocated_slot' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to SlotPool::GetAllocatedSlot; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_get_slot_number()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'ptr'
            // ecx contains pointer to slot pool

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'ptr' is now at [esp + 0x08]
            push ecx; // 'ptr' is now at [esp + 0x0C]
            push edx; // 'ptr' is now at [esp + 0x10]
            push esi; // 'ptr' is now at [esp + 0x14]
            push edi; // 'ptr' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'ptr'

            call slot_pool::get_slot_number; // call custom get_slot_number

            // no need to restore esp since 'get_slot_number' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 4; // return immediately to caller function, not back to SlotPool::GetSlotNumber; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_malloc_array()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'count'
            // [esp + 0x08] is 'last_slot'
            // ecx contains pointer to slot pool

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'last_slot' is now at [esp + 0x0C]
            push ecx; // 'last_slot' is now at [esp + 0x10]
            push edx; // 'last_slot' is now at [esp + 0x14]
            push esi; // 'last_slot' is now at [esp + 0x18]
            push edi; // 'last_slot' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'last_slot'
            push [esp + 0x1C]; // repush 'count'

            call slot_pool::malloc_array; // call custom malloc_array

            // no need to restore esp since 'malloc_array' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 8; // return immediately to caller function, not back to SlotPool::Malloc; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_malloc()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to slot pool

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'return address' is now at [esp + 0x04]
            push ecx; // 'return address' is now at [esp + 0x08]
            push edx; // 'return address' is now at [esp + 0x0C]
            push esi; // 'return address' is now at [esp + 0x10]
            push edi; // 'return address' is now at [esp + 0x14]

            call slot_pool::malloc; // call custom malloc

            // no need to restore esp since 'malloc' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to SlotPool::Malloc; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_manager_new_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_size'
            // [esp + 0x08] is 'slot_count'
            // [esp + 0x0C] is 'name'
            // [esp + 0x10] is 'type'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'type' is now at [esp + 0x14]
            push ecx; // 'type' is now at [esp + 0x18]
            push edx; // 'type' is now at [esp + 0x1C]
            push esi; // 'type' is now at [esp + 0x20]
            push edi; // 'type' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'type'
            push [esp + 0x24]; // repush 'name'
            push [esp + 0x24]; // repush 'slot_count'
            push [esp + 0x24]; // repush 'slot_size'

            call slot_pool_manager::new_slot_pool; // call custom new_slot_pool

            add esp, 0x10; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to SlotPoolManager::NewSlotPool
        }
    }

    __declspec(naked) void detour_slot_pool_manager_create_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_size'
            // [esp + 0x08] is 'slot_count'
            // [esp + 0x0C] is 'name'
            // [esp + 0x10] is 'type'
            // ecx contains pointer to slot pool

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'type' is now at [esp + 0x14]
            push ecx; // 'type' is now at [esp + 0x18]
            push edx; // 'type' is now at [esp + 0x1C]
            push esi; // 'type' is now at [esp + 0x20]
            push edi; // 'type' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'type'
            push [esp + 0x24]; // repush 'name'
            push [esp + 0x24]; // repush 'slot_count'
            push [esp + 0x24]; // repush 'slot_size'

            call slot_pool_manager::create_slot_pool; // call custom create_slot_pool

            // no need to restore esp since 'create_slot_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn 16; // return immediately to caller function, not back to SlotPoolManager::CreateSlotPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_slot_pool_manager_remove_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pool'
            // ecx contains pointer to slot pool

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pool' is now at [esp + 0x08]
            push ebx; // 'pool' is now at [esp + 0x0C]
            push ecx; // 'pool' is now at [esp + 0x10]
            push edx; // 'pool' is now at [esp + 0x14]
            push esi; // 'pool' is now at [esp + 0x18]
            push edi; // 'pool' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'pool'

            call slot_pool_manager::remove_slot_pool; // call custom remove_slot_pool

            // no need to restore esp since 'remove_slot_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to SlotPoolManager::RemoveSlotPool; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_b_malloc_slot()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'slot_pool' is now at [esp + 0x08]
            push ecx; // 'slot_pool' is now at [esp + 0x0C]
            push edx; // 'slot_pool' is now at [esp + 0x10]
            push esi; // 'slot_pool' is now at [esp + 0x14]
            push edi; // 'slot_pool' is now at [esp + 0x18]

            mov ecx, [esp + 0x18]; // make 'slot_pool' as 'this' pointer

            call slot_pool::malloc; // call custom malloc

            // no need to restore esp since 'malloc' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bMalloc
        }
    }

    __declspec(naked) void detour_b_o_malloc_slot()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'slot_pool' is now at [esp + 0x08]
            push ecx; // 'slot_pool' is now at [esp + 0x0C]
            push edx; // 'slot_pool' is now at [esp + 0x10]
            push esi; // 'slot_pool' is now at [esp + 0x14]
            push edi; // 'slot_pool' is now at [esp + 0x18]

            mov ecx, [esp + 0x18]; // make 'slot_pool' as 'this' pointer

            call slot_pool::malloc; // call custom malloc

            // no need to restore esp since 'malloc' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bMalloc
        }
    }

    __declspec(naked) void detour_b_free_slot()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'
            // [esp + 0x08] is 'ptr'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'ptr' is now at [esp + 0x0C]
            push ebx; // 'ptr' is now at [esp + 0x10]
            push ecx; // 'ptr' is now at [esp + 0x14]
            push edx; // 'ptr' is now at [esp + 0x18]
            push esi; // 'ptr' is now at [esp + 0x1C]
            push edi; // 'ptr' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'ptr'

            mov ecx, [esp + 0x20]; // make 'slot_pool' as 'this' pointer

            call slot_pool::free; // call custom free

            // no need to restore esp since 'free' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to bFree
        }
    }

    __declspec(naked) void detour_b_is_slot_pool_full()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'slot_pool' is now at [esp + 0x08]
            push ecx; // 'slot_pool' is now at [esp + 0x0C]
            push edx; // 'slot_pool' is now at [esp + 0x10]
            push esi; // 'slot_pool' is now at [esp + 0x14]
            push edi; // 'slot_pool' is now at [esp + 0x18]

            mov ecx, [esp + 0x18]; // make 'slot_pool' as 'this' pointer

            call slot_pool::is_full; // call custom is_full

            // no need to restore esp since 'is_full' is a __thiscall

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bIsSlotPoolFull
        }
    }

    __declspec(naked) void detour_b_count_free_slots()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'slot_pool' is now at [esp + 0x08]
            push ecx; // 'slot_pool' is now at [esp + 0x0C]
            push edx; // 'slot_pool' is now at [esp + 0x10]
            push esi; // 'slot_pool' is now at [esp + 0x14]
            push edi; // 'slot_pool' is now at [esp + 0x18]

            mov ecx, [esp + 0x18]; // make 'slot_pool' as 'this' pointer

            call slot_pool::free_slots; // call custom free_slots

            // no need to restore esp since 'free_slots' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bCountFreeSlots
        }
    }

    __declspec(naked) void detour_b_count_total_slots()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'slot_pool' is now at [esp + 0x08]
            push ecx; // 'slot_pool' is now at [esp + 0x0C]
            push edx; // 'slot_pool' is now at [esp + 0x10]
            push esi; // 'slot_pool' is now at [esp + 0x14]
            push edi; // 'slot_pool' is now at [esp + 0x18]

            mov ecx, [esp + 0x18]; // make 'slot_pool' as 'this' pointer

            call slot_pool::total_slots; // call custom total_slots

            // no need to restore esp since 'total_slots' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bCountTotalSlots
        }
    }

    __declspec(naked) void detour_b_new_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'slot_size'
            // [esp + 0x08] is 'slot_count'
            // [esp + 0x0C] is 'name'
            // [esp + 0x10] is 'type'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'type' is now at [esp + 0x14]
            push ecx; // 'type' is now at [esp + 0x18]
            push edx; // 'type' is now at [esp + 0x1C]
            push esi; // 'type' is now at [esp + 0x20]
            push edi; // 'type' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'type'
            push [esp + 0x24]; // repush 'name'
            push [esp + 0x24]; // repush 'slot_count'
            push [esp + 0x24]; // repush 'slot_size'

            mov ecx, slot_pool_manager::instance; // make it a __thiscall

            call slot_pool_manager::create_slot_pool; // call custom create_slot_pool

            // no need to restore esp since 'create_slot_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bNewSlotPool
        }
    }

    __declspec(naked) void detour_b_delete_slot_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pool'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'pool' is now at [esp + 0x08]
            push ecx; // 'pool' is now at [esp + 0x0C]
            push edx; // 'pool' is now at [esp + 0x10]
            push esi; // 'pool' is now at [esp + 0x14]
            push edi; // 'pool' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'pool'

            mov ecx, slot_pool_manager::instance; // make it a __thiscall

            call slot_pool_manager::remove_slot_pool; // call custom remove_slot_pool

            // no need to restore esp since 'remove_slot_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to bDeleteSlotPool
        }
    }

    __declspec(naked) void detour_car_loader_set_memory_pool_size()
    {
        __asm
        {
            // 'size' is already in ebx and pushed, meaning [esp + 0x00]
            // 'this' is in esi, push as well to avoid '__thiscall' conv

            push eax; // 'size' is now at [esp + 0x04]
            push ebx; // 'size' is now at [esp + 0x08]
            push ecx; // 'size' is now at [esp + 0x0C]
            push edx; // 'size' is now at [esp + 0x10]
            push esi; // 'size' is now at [esp + 0x14]
            push edi; // 'size' is now at [esp + 0x18]
            
            push ebp; // push 'size'
            push esi; // push 'this'
            
            call init_car_loader_pool; // call custom initializer
            
            add esp, 0x08; // since we repushed all arguments
            
            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            push 0x007E49F0;
            retn; // return
        }
    }

    __declspec(naked) void detour_track_streamer_init_memory_pool()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pool_size'
            // ecx contains pointer to streamer

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pool_size' is now at [esp + 0x08]
            push ebx; // 'pool_size' is now at [esp + 0x0C]
            push ecx; // 'pool_size' is now at [esp + 0x10]
            push edx; // 'pool_size' is now at [esp + 0x14]
            push esi; // 'pool_size' is now at [esp + 0x18]
            push edi; // 'pool_size' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'pool_size'

            call streamer::init_memory_pool; // call custom init_memory_pool

            // no need to restore esp since 'init_memory_pool' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to TrackStreamer::InitMemoryPool; note that this is a __thiscall
        }
    }

    void memory_patches::init()
    {
        // bMemoryInit
        hook::jump(0x004769E0, &detour_b_memory_init);

        // bMemoryClose
        hook::jump(0x00476A30, &detour_b_memory_close);

        // bInitMemoryPool
        hook::jump(0x00476970, &detour_b_init_memory_pool);

        // bCloseMemoryPool
        hook::jump(0x004769C0, &detour_b_close_memory_pool);

        // bMemoryCreatePersistentPool
        hook::jump(0x00477AA0, &detour_b_memory_create_persistent_pool);

        // bMemoryGetAllocationNumber
        hook::jump(0x00470FC0, &detour_b_memory_get_allocation_number);

        // bSetMemoryPoolOverrideInfo
        hook::jump(0x00470DC0, &detour_b_set_memory_pool_override_info);

        // bReserveMemoryPool
        hook::jump(0x00470DB0, &detour_b_reserve_memory_pool);

        // bSetMemoryPoolDebugFill
        hook::jump(0x00470DE0, &detour_b_set_memory_pool_debug_fill);

        // bSetMemoryPoolDebugTracing
        hook::jump(0x00477900, &detour_b_set_memory_pool_debug_tracing);

        // bSetMemoryPoolTopDirection
        hook::jump(0x00470E60, &detour_b_set_memory_pool_top_direction);

        // bSetMemoryPoolDefaultAlignment
        hook::jump(0x00470E80, &detour_b_set_memory_pool_default_alignment);

        // bSetMemoryPoolOverflowNumber
        hook::jump(0x00470EA0, &detour_b_set_memory_pool_overflow_number);

        // bVerifyPoolIntegrity
        hook::jump(0x00476A60, &detour_b_verify_pool_integrity);

        // bIsInMemoryPool
        hook::jump(0x00470E00, &detour_b_is_in_memory_pool);

        // bGetMallocSize
        hook::jump(0x00470EF0, &detour_b_get_malloc_size);

        // bMemoryGetAllocations
        hook::jump(0x00475AC0, &detour_b_memory_get_allocations);

        // bMemoryPrintAllocationsByAddress
        hook::jump(0x00475EA0, &detour_b_memory_print_allocations_by_address);

        // bGetFreeMemoryPoolNum
        hook::jump(0x00470D80, &detour_b_get_free_memory_pool_num);

        // bGetMemoryPoolDebugName
        hook::jump(0x00470F90, &detour_b_get_memory_pool_debug_name);

        // bGetMemoryPoolNum
        hook::jump(0x00475160, &detour_b_get_memory_pool_num);

        // bLargestMalloc
        hook::jump(0x004750F0, &detour_b_largest_malloc);

        // bCountFreeMemory
        hook::jump(0x004750C0, &detour_b_count_free_memory);

        // bWareMalloc
        hook::jump(0x00476B20, &detour_b_ware_malloc);

        // bMalloc
        hook::jump(0x00477230, &detour_b_malloc);

        // bFree
        hook::jump(0x00476D70, &detour_b_free);

        // SlotPool::CleanupExpandedSlotPools
        hook::jump(0x004770C0, &detour_slot_pool_cleanup_expanded_slot_pools);

        // SlotPool::ExpandSlotPool
        hook::jump(0x004775B0, &detour_slot_pool_expand_slot_pool);

        // SlotPool::FlushSlotPool
        hook::jump(0x0046F470, &detour_slot_pool_flush_slot_pool);

        // SlotPool::GetAllocatedSlot
        hook::jump(0x00477630, &detour_slot_pool_get_allocated_slot);

        // SlotPool::GetSlotNumber
        hook::jump(0x0046F4C0, &detour_slot_pool_get_slot_number);

        // SlotPool::Malloc
        hook::jump(0x004777A0, &detour_slot_pool_malloc_array);

        // SlotPool::Malloc
        hook::jump(0x00477740, &detour_slot_pool_malloc);

        // SlotPoolManager::NewSlotPool
        hook::jump(0x00477540, &detour_slot_pool_manager_new_slot_pool);

        // SlotPoolManager::CreateSlotPool
        hook::jump(0x00477860, &detour_slot_pool_manager_create_slot_pool);

        // SlotPoolManager::RemoveSlotPool
        hook::jump(0x004778B0, &detour_slot_pool_manager_remove_slot_pool);

        // bMalloc
        hook::jump(0x0046F400, &detour_b_malloc_slot);

        // bOMalloc
        hook::jump(0x00477BE0, &detour_b_o_malloc_slot);

        // bFree
        hook::jump(0x0046F420, &detour_b_free_slot);

        // bIsSlotPoolFull
        hook::jump(0x0046F440, &detour_b_is_slot_pool_full);

        // bCountFreeSlots
        hook::jump(0x0046F450, &detour_b_count_free_slots);

        // bCountTotalSlots
        hook::jump(0x0046F460, &detour_b_count_total_slots);

        // bNewSlotPool
        hook::jump(0x00477B80, &detour_b_new_slot_pool);

        // bDeleteSlotPool
        hook::jump(0x00477BD0, &detour_b_delete_slot_pool);

        // CarLoader::SetMemoryPoolSize
        hook::jump(0x007E49A5, &detour_car_loader_set_memory_pool_size);

        // TrackStreamer::InitMemoryPool
        hook::jump(0x007A42E0, &detour_track_streamer_init_memory_pool);
    }
}
