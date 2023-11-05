#include <hyperlib/assets/world_anims.hpp>
#include <hyperlinked/patches/assets/world_anim.hpp>

namespace hyper
{
    __declspec(naked) void detour_world_anim_loader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call world_anim::loader; // call custom loader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to WorldAnimLoader
        }
    }

    __declspec(naked) void detour_world_anim_unloader()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'block'

            // eax gets overwritten regardless
            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push ebx; // 'block' is now at [esp + 0x08]
            push ecx; // 'block' is now at [esp + 0x0C]
            push edx; // 'block' is now at [esp + 0x10]
            push esi; // 'block' is now at [esp + 0x14]
            push edi; // 'block' is now at [esp + 0x18]

            push [esp + 0x18]; // repush 'block'

            call world_anim::unloader; // call custom unloader

            add esp, 0x04; // since we repushed all arguments

            and eax, 0x000000FF; // hack !!!

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register

            retn; // return immediately to caller function, not back to WorldAnimLoader
        }
    }

    void world_anim_patches::init()
    {
        // WorldAnimLoader
        hook::jump(0x0045C7F0, &detour_world_anim_loader);

        // WorldAnimUnloader
        hook::jump(0x0045DD00, &detour_world_anim_unloader);
    }
}
