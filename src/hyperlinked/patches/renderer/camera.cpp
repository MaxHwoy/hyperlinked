#include <hyperlib/renderer/camera.hpp>
#include <hyperlinked/patches/renderer/camera.hpp>

namespace hyper
{
    __declspec(naked) void detour_camera_set_camera_matrix()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'world_to_camera'
            // [esp + 0x08] is 'elapsed_time'
            // ecx contains pointer to camera

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'elapsed_time' is now at [esp + 0x0C]
            push ebx; // 'elapsed_time' is now at [esp + 0x10]
            push ecx; // 'elapsed_time' is now at [esp + 0x14]
            push edx; // 'elapsed_time' is now at [esp + 0x18]
            push esi; // 'elapsed_time' is now at [esp + 0x1C]
            push edi; // 'elapsed_time' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'elapsed_time'
            push [esp + 0x20]; // repush 'world_to_camera'

            call camera::set_camera_matrix; // call custom set_camera_matrix

            // no need to restore esp since 'set_camera_matrix' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to Camera::SetCameraMatrix; note that this is a __thiscall
        }
    }

    void camera_patches::init()
    {
        // Camera::SetCameraMatrix
        hook::jump(0x004822F0, &detour_camera_set_camera_matrix);
    }
}
