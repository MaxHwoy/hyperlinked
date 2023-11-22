#include <hyperlib/hook.hpp>
#include <hyperlib/renderer/poly_manager.hpp>
#include <hyperlib/renderer/world_renderer.hpp>
#include <hyperlinked/patches/renderer/world_renderer.hpp>

namespace hyper
{
    __declspec(naked) void detour_render_world_in_game()
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

            call world_renderer::render_internal; // call custom render_internal

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to RenderWorldInGame
        }
    }

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

            call world_renderer::create_rendering_model; // call custom create_rendering_model

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

    __declspec(naked) void detour_sort_models_and_draw_world()
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

            call world_renderer::render; // call custom render

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to SortModelsAndDrawWorld
        }
    }

    void world_renderer_patches::init()
    {
        // RenderWorldInGame
        hook::jump(0x00727230, &detour_render_world_in_game);

        // CreateRenderingModel
        hook::jump(0x00727930, &detour_create_rendering_model);

        // SortModelsAndDrawWorld
        hook::jump(0x0072C9B0, &detour_sort_models_and_draw_world);
    }
}
