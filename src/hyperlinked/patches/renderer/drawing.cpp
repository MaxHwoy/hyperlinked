#include <hyperlib/hook.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/drawing.hpp>
#include <hyperlinked/patches/renderer/drawing.hpp>

namespace hyper
{
    __declspec(naked) void detour_create_rendering_model()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'entry'
            // [esp + 0x08] is 'solid'
            // [esp + 0x0C] is 'flags'
            // [esp + 0x10] is 'effect'
            // [esp + 0x14] is 'textures'
            // [esp + 0x18] is 'trs'
            // [esp + 0x1C] is 'context'
            // [esp + 0x20] is 'material'
            // [esp + 0x24] is 'blend_trs'
            // [esp + 0x28] is 'pca'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'pca' is now at [esp + 0x2C]
            push ebx; // 'pca' is now at [esp + 0x30]
            push ecx; // 'pca' is now at [esp + 0x34]
            push edx; // 'pca' is now at [esp + 0x38]
            push esi; // 'pca' is now at [esp + 0x3C]
            push edi; // 'pca' is now at [esp + 0x40]

            push [esp + 0x40]; // repush 'pca'
            push [esp + 0x40]; // repush 'blend_trs'
            push [esp + 0x40]; // repush 'material'
            push [esp + 0x40]; // repush 'context'
            push [esp + 0x40]; // repush 'trs'
            push [esp + 0x40]; // repush 'textures'
            push [esp + 0x40]; // repush 'effect'
            push [esp + 0x40]; // repush 'flags'
            push [esp + 0x40]; // repush 'solid'
            push [esp + 0x40]; // repush 'entry'

            call renderer::create_rendering_model; // call custom create_rendering_model

            add esp, 0x28; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to CreateRenderingModel
        }
    }

    __declspec(naked) void detour_init_render_targets()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call renderer::init_render_targets; // call custom init_render_targets

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to InitRenderTargets
        }
    }

    __declspec(naked) void detour_set_current_render_target()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'target'
            // [esp + 0x08] is 'clear'
            // [esp + 0x0C] is 'clear_color'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'clear_color' is now at [esp + 0x10]
            push ebx; // 'clear_color' is now at [esp + 0x14]
            push ecx; // 'clear_color' is now at [esp + 0x18]
            push edx; // 'clear_color' is now at [esp + 0x1C]
            push esi; // 'clear_color' is now at [esp + 0x20]
            push edi; // 'clear_color' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'clear_color'
            push [esp + 0x24]; // repush 'clear'
            push [esp + 0x24]; // repush 'target'

            call renderer::set_render_target; // call custom set_render_target

            add esp, 0x0C; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SetCurrentRenderTarget
        }
    }

    __declspec(naked) void detour_update_render_views()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'return address' is now at [esp + 0x04]
            push ebx; // 'return address' is now at [esp + 0x08]
            push ecx; // 'return address' is now at [esp + 0x0C]
            push edx; // 'return address' is now at [esp + 0x10]
            push esi; // 'return address' is now at [esp + 0x14]
            push edi; // 'return address' is now at [esp + 0x18]

            call renderer::update_render_views; // call custom update_render_views

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to UpdateRenderViews
        }
    }

    void drawing_patches::init()
    {
        // CreateRenderingModel
        hook::jump(0x00727930, &detour_create_rendering_model);

        // InitRenderTargets
        hook::jump(0x0071AD20, &detour_init_render_targets);

        // SetCurrentRenderTarget
        hook::jump(0x0070DD30, &detour_set_current_render_target);

        // UpdateRenderViews
        hook::jump(0x0074EB90, &detour_update_render_views);
    }
}
