#include <hyperlib/renderer/effect.hpp>
#include <hyperlinked/patches/renderer/effect.hpp>

namespace hyper
{
    __declspec(naked) void detour_e_effect_connect_parameters()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call effect::connect_parameters; // call custom connect_parameters

            // no need to restore esp since 'connect_parameters' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::ConnectParameters; note that this is a __thiscall
        }
    }

    void effect_patches::init()
    {
        // eEffect::ConnectParameters
        hook::jump(0x0072B5F0, &detour_e_effect_connect_parameters);
    }
}
