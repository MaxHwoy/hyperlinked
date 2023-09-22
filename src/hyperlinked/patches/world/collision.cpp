#include <hyperlib/world/collision.hpp>
#include <hyperlinked/patches/world/collision.hpp>

namespace hyper
{
    __declspec(naked) void detour_w_collision_assets_ctor()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to collision::assets

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            push ecx; // push reference to 'assets'

            call collision::assets::ctor; // call custom constructor

            add esp, 0x04; // since we pushed argument

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to WCollisionAssets::WCollisionAssets; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_w_collision_assets_dtor()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to collision::assets

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            push ecx; // push reference to 'assets'

            call collision::assets::dtor; // call custom destructor

            add esp, 0x04; // since we pushed argument

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to WCollisionAssets::~WCollisionAssets; note that this is a __thiscall
        }
    }

    void collision_patches::init()
    {
        // WCollisionAssets::WCollisionAssets
        hook::jump(0x0080E460, &detour_w_collision_assets_ctor);

        // WCollisionAssets::~WCollisionAssets
        hook::jump(0x0080E5D0, &detour_w_collision_assets_dtor);
    }
}
