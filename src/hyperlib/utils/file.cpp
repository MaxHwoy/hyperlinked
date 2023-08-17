#include <hyperlib/utils/file.hpp>

namespace hyper
{
    void file::add_queued_file(void* buffer, const char* filename, size_t file_pos, size_t num_bytes, void(*callback)(void*, bool), void* callback_param, queued_file_params* params)
    {
        call_function<void(__cdecl*)(void*, const char*, size_t, size_t, void(*)(void*, bool), void*, queued_file_params*)>(0x006B48B0)(buffer, filename, file_pos, num_bytes, callback, callback_param, params);
    }
}
