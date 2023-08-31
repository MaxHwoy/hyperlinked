#include <hyperlib/memory/fast_mem.hpp>

namespace hyper
{
    auto fast_mem::malloc(alloc_size_t size, const char* debug_name) -> void*
    {
        return call_function<void*(__thiscall*)(fast_mem*, alloc_size_t, const char*)>(0x0060BA70)(this, size, debug_name);
    }

    void fast_mem::free(void* memory, alloc_size_t size, const char* debug_name)
    {
        call_function<void(__thiscall*)(fast_mem*, void*, alloc_size_t, const char*)>(0x00609E80)(this, memory, size, debug_name);
    }
}
