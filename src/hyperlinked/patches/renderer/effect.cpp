#include <hyperlib/renderer/effect.hpp>
#include <hyperlinked/patches/renderer/effect.hpp>

namespace hyper
{
    void set_current_pass_internal(effect* eff, std::uint32_t pass, const char* technique, bool use_low_lod)
    {
        eff->set_current_pass(pass, technique, use_low_lod);
    }

    __declspec(naked) void detour_e_effect_dtor()
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

            call effect::~effect; // call custom dtor

            // no need to restore esp since 'dtor' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::~eEffect; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_handle_material_data()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'material'
            // [esp + 0x08] is 'flags'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'flags' is now at [esp + 0x0C]
            push ebx; // 'flags' is now at [esp + 0x10]
            push ecx; // 'flags' is now at [esp + 0x14]
            push edx; // 'flags' is now at [esp + 0x18]
            push esi; // 'flags' is now at [esp + 0x1C]
            push edi; // 'flags' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'flags'
            push [esp + 0x20]; // repush 'material'

            call effect::handle_material_data; // call custom handle_material_data

            // no need to restore esp since 'handle_material_data' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to eEffect::HandleMaterialData; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_transforms()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'local_to_world'
            // [esp + 0x08] is 'view'
            // [esp + 0x0C] is 'use_nonjittered'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'use_nonjittered' is now at [esp + 0x10]
            push ebx; // 'use_nonjittered' is now at [esp + 0x14]
            push ecx; // 'use_nonjittered' is now at [esp + 0x18]
            push edx; // 'use_nonjittered' is now at [esp + 0x1C]
            push esi; // 'use_nonjittered' is now at [esp + 0x20]
            push edi; // 'use_nonjittered' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'use_nonjittered'
            push [esp + 0x24]; // repush 'view'
            push [esp + 0x24]; // repush 'local_to_world'

            call effect::set_transforms; // call custom set_transforms

            // no need to restore esp since 'set_transforms' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 12; // return immediately to caller function, not back to eEffect::SetTransforms; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_draw_full_screen_quad()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'texture'
            // [esp + 0x08] is 'invert'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'invert' is now at [esp + 0x0C]
            push ebx; // 'invert' is now at [esp + 0x10]
            push ecx; // 'invert' is now at [esp + 0x14]
            push edx; // 'invert' is now at [esp + 0x18]
            push esi; // 'invert' is now at [esp + 0x1C]
            push edi; // 'invert' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'invert'
            push [esp + 0x20]; // repush 'texture'

            call effect::draw_full_screen_quad; // call custom draw_full_screen_quad

            // no need to restore esp since 'draw_full_screen_quad' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to eEffect::DrawFullScreenQuad; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_start()
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

            call effect::start; // call custom start

            // no need to restore esp since 'start' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::Start; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_initialize()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'input'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'input' is now at [esp + 0x08]
            push ebx; // 'input' is now at [esp + 0x0C]
            push ecx; // 'input' is now at [esp + 0x10]
            push edx; // 'input' is now at [esp + 0x14]
            push esi; // 'input' is now at [esp + 0x18]
            push edi; // 'input' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'input'

            call effect::initialize; // call custom initialize

            // no need to restore esp since 'initialize' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to eEffect::Initialize; note that this is a __thiscall
        }
    }

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

    __declspec(naked) void detour_e_effect_reset_filter_params()
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

            call effect::reset_filter_params; // call custom reset_filter_params

            // no need to restore esp since 'reset_filter_params' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::ResetFilterParams; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_load_effect_from_buffer()
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

            call effect::load_effect; // call custom load_effect

            // no need to restore esp since 'load_effect' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::LoadEffectFromBuffer; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_recompute_techniques_by_detail()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'detail_level'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'detail_level' is now at [esp + 0x08]
            push ebx; // 'detail_level' is now at [esp + 0x0C]
            push ecx; // 'detail_level' is now at [esp + 0x10]
            push edx; // 'detail_level' is now at [esp + 0x14]
            push esi; // 'detail_level' is now at [esp + 0x18]
            push edi; // 'detail_level' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'detail_level'

            call effect::recompute_techniques_by_detail; // call custom recompute_techniques_by_detail

            // no need to restore esp since 'recompute_techniques_by_detail' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to eEffect::RecomputeTechniquesByDetail; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_current_pass()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'pass'
            // [esp + 0x08] is 'technique'
            // [esp + 0x0C] is 'use_low_lod'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'use_low_lod' is now at [esp + 0x10]
            push ebx; // 'use_low_lod' is now at [esp + 0x14]
            push ecx; // 'use_low_lod' is now at [esp + 0x18]
            push edx; // 'use_low_lod' is now at [esp + 0x1C]
            push esi; // 'use_low_lod' is now at [esp + 0x20]
            push edi; // 'use_low_lod' is now at [esp + 0x24]

            push [esp + 0x24]; // repush 'use_low_lod'
            push [esp + 0x24]; // repush 'technique'
            push [esp + 0x24]; // repush 'pass'
            push ecx;          // push pointer to effect

            call set_current_pass_internal; // call custom set_current_pass

            add esp, 0x10; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 12; // return immediately to caller function, not back to eEffect::SetCurrentPass; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_pca_blend_data()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'data'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'data' is now at [esp + 0x08]
            push ebx; // 'data' is now at [esp + 0x0C]
            push ecx; // 'data' is now at [esp + 0x10]
            push edx; // 'data' is now at [esp + 0x14]
            push esi; // 'data' is now at [esp + 0x18]
            push edi; // 'data' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'data'

            call effect::set_pca_blend_data; // call custom set_pca_blend_data

            // no need to restore esp since 'set_pca_blend_data' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to eEffect::SetPCABlendData; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_blend_matrices()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'blend_matrices'
            // [esp + 0x08] is 'entry'
            // ecx needs pointer to current effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'entry' is now at [esp + 0x0C]
            push ebx; // 'entry' is now at [esp + 0x10]
            push ecx; // 'entry' is now at [esp + 0x14]
            push edx; // 'entry' is now at [esp + 0x18]
            push esi; // 'entry' is now at [esp + 0x1C]
            push edi; // 'entry' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'entry'
            push [esp + 0x20]; // repush 'blend_matrices'

            call effect::get_current_effect;

            mov ecx, eax;

            call effect::set_blend_matrices; // call custom set_blend_matrices

            // no need to restore esp since 'set_blend_matrices' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to eEffect::SetBlendMatrices
        }
    }

    __declspec(naked) void detour_e_effect_set_light_context()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'context'
            // [esp + 0x08] is 'local_to_world'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'local_to_world' is now at [esp + 0x0C]
            push ebx; // 'local_to_world' is now at [esp + 0x10]
            push ecx; // 'local_to_world' is now at [esp + 0x14]
            push edx; // 'local_to_world' is now at [esp + 0x18]
            push esi; // 'local_to_world' is now at [esp + 0x1C]
            push edi; // 'local_to_world' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'local_to_world'
            push [esp + 0x20]; // repush 'context'

            call effect::set_light_context; // call custom set_light_context

            // no need to restore esp since 'set_light_context' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to eEffect::SetLightContext; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_texture_maps()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'model'
            // [esp + 0x08] is 'flags'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'flags' is now at [esp + 0x0C]
            push ebx; // 'flags' is now at [esp + 0x10]
            push ecx; // 'flags' is now at [esp + 0x14]
            push edx; // 'flags' is now at [esp + 0x18]
            push esi; // 'flags' is now at [esp + 0x1C]
            push edi; // 'flags' is now at [esp + 0x20]

            push [esp + 0x20]; // repush 'flags'
            push [esp + 0x20]; // repush 'model'

            call effect::set_texture_maps; // call custom set_texture_maps

            // no need to restore esp since 'set_texture_maps' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 8; // return immediately to caller function, not back to eEffect::SetTextureMaps; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_set_texture_animation()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'info'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'info' is now at [esp + 0x08]
            push ebx; // 'info' is now at [esp + 0x0C]
            push ecx; // 'info' is now at [esp + 0x10]
            push edx; // 'info' is now at [esp + 0x14]
            push esi; // 'info' is now at [esp + 0x18]
            push edi; // 'info' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'info'

            call effect::set_texture_animation; // call custom set_texture_animation

            // no need to restore esp since 'set_texture_animation' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 4; // return immediately to caller function, not back to eEffect::SetTextureAnimation; note that this is a __thiscall
        }
    }

    __declspec(naked) void detour_e_effect_commit_and_draw_indexed()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'vertex_count'
            // [esp + 0x08] is 'index_start'
            // [esp + 0x0C] is 'index_count'
            // [esp + 0x10] is 'model'
            // ecx contains pointer to effect

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'model' is now at [esp + 0x14]
            push ebx; // 'model' is now at [esp + 0x18]
            push ecx; // 'model' is now at [esp + 0x1C]
            push edx; // 'model' is now at [esp + 0x20]
            push esi; // 'model' is now at [esp + 0x24]
            push edi; // 'model' is now at [esp + 0x28]

            push [esp + 0x28]; // repush 'model'
            push [esp + 0x28]; // repush 'index_count'
            push [esp + 0x28]; // repush 'index_start'
            push [esp + 0x28]; // repush 'vertex_count'

            call effect::commit_and_draw_indexed; // call custom commit_and_draw_indexed

            // no need to restore esp since 'commit_and_draw_indexed' is a __thiscall

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn 16; // return immediately to caller function, not back to eEffect::CommitAndDrawIndexed; note that this is a __thiscall
        }
    }


    
    __declspec(naked) void detour_shader_lib_init()
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

            call shader_lib::init; // call custom init

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to ShaderLib::Init
        }
    }

    __declspec(naked) void detour_shader_lib_close()
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

            call shader_lib::close; // call custom close

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to ShaderLib::Close
        }
    }

    __declspec(naked) void detour_shader_lib_reinit()
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

            call shader_lib::reinit; // call custom reinit

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to ShaderLib::Reinit
        }
    }

    __declspec(naked) void detour_shader_lib_recompute_techniques_by_detail()
    {
        __asm
        {
            // [esp + 0x00] is 'return address'
            // [esp + 0x04] is 'detail_level'

            // esp is auto-managed, non-incremental
            // ebp is auto-managed, restored on function return

            push eax; // 'detail_level' is now at [esp + 0x08]
            push ebx; // 'detail_level' is now at [esp + 0x0C]
            push ecx; // 'detail_level' is now at [esp + 0x10]
            push edx; // 'detail_level' is now at [esp + 0x14]
            push esi; // 'detail_level' is now at [esp + 0x18]
            push edi; // 'detail_level' is now at [esp + 0x1C]

            push [esp + 0x1C]; // repush 'detail_level'

            call shader_lib::recompute_techniques_by_detail; // call custom recompute_techniques_by_detail

            add esp, 0x04; // since we repushed all arguments

            pop edi; // restore saved register
            pop esi; // restore saved register
            pop edx; // restore saved register
            pop ecx; // restore saved register
            pop ebx; // restore saved register
            pop eax; // restore saved register

            retn; // return immediately to caller function, not back to ShaderLib::RecomputeTechniquesByDetail
        }
    }

    void effect_patches::init()
    {
        // eEffect::~eEffect
        hook::jump(0x0072C220, &detour_e_effect_dtor);

        // eEffect::HandleMaterialData
        hook::jump(0x0071E030, &detour_e_effect_handle_material_data);

        // eEffect::SetTransforms
        hook::jump(0x0071E660, &detour_e_effect_set_transforms);

        // eEffect::DrawFullScreenQuad
        hook::jump(0x0071E9B0, &detour_e_effect_draw_full_screen_quad);

        // eEffect::Start
        hook::jump(0x00749790, &detour_e_effect_start);

        // eEffect::Initialize
        hook::jump(0x0072F030, &detour_e_effect_initialize);

        // eEffect::ConnectParameters
        hook::jump(0x0072B5F0, &detour_e_effect_connect_parameters);

        // eEffect::ResetFilterParams
        hook::jump(0x00713EE0, &detour_e_effect_reset_filter_params);

        // eEffect::LoadEffectFromBuffer
        hook::jump(0x0072B660, &detour_e_effect_load_effect_from_buffer);

        // eEffect::RecomputeTechniquesByDetail
        hook::jump(0x00730250, &detour_e_effect_recompute_techniques_by_detail);

        // eEffect::SetCurrentPass
        hook::jump(0x00723600, &detour_e_effect_set_current_pass);

        // eEffect::SetPCABlendData
        hook::jump(0x0071E540, &detour_e_effect_set_pca_blend_data);

        // eEffect::SetBlendMatrices
        hook::jump(0x00714CC0, &detour_e_effect_set_blend_matrices);

        // eEffect::SetLightContext
        hook::jump(0x0071DFA0, &detour_e_effect_set_light_context);

        // eEffect::SetTextureMaps
        hook::jump(0x007234A0, &detour_e_effect_set_texture_maps);

        // eEffect::SetTextureAnimation
        hook::jump(0x0071DEB0, &detour_e_effect_set_texture_animation);

        // eEffect::CommitAndDrawIndexed
        hook::jump(0x0071ECB0, &detour_e_effect_commit_and_draw_indexed);



        // ShaderLib::Init
        hook::jump(0x00756AA0, &detour_shader_lib_init);

        // ShaderLib::Close
        hook::jump(0x0073E6A0, &detour_shader_lib_close);

        // ShaderLib::Reinit
        hook::jump(0x0073E740, &detour_shader_lib_reinit);

        // ShaderLib::RecomputeTechniquesByDetail
        hook::jump(0x0073E7E0, &detour_shader_lib_recompute_techniques_by_detail);
    }
}
