#include <hyperlib/assets/loader.hpp>

namespace hyper
{
    void loader::service_resource_loading()
    {
        call_function<void(__cdecl*)()>(0x006B68D0)();
    }

    void loader::load_chunks(void* memory, size_t size)
    {
        call_function<void(__cdecl*)(void*, size_t)>(0x006AD8F0)(memory, size);
    }

    void loader::unload_chunks(void* memory, size_t size)
    {
        call_function<void(__cdecl*)(void*, size_t)>(0x006A83A0)(memory, size);
    }
}
