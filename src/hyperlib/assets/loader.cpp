#include <hyperlib/assets/loader.hpp>

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

    void loader::load_chunks(void* memory, size_t size)
    {
        call_function<void(__cdecl*)(void*, size_t)>(0x006AD8F0)(memory, size);
    }

    void loader::unload_chunks(void* memory, size_t size)
    {
        call_function<void(__cdecl*)(void*, size_t)>(0x006A83A0)(memory, size);
    }

    void loader::load_temp_perm_chunks(void** memory, size_t* size, std::uint32_t alloc_params, const char* debug_name)
    {
        call_function<void(__cdecl*)(void**, size_t*, std::uint32_t, const char*)>(0x006ADBC0)(memory, size, alloc_params, debug_name);
    }
}
