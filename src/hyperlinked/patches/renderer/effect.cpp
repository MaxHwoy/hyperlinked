#include <hyperlib/renderer/effect.hpp>
#include <hyperlinked/patches/renderer/effect.hpp>

namespace hyper
{
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

        // eEffect::RecomputeTechniquesByDetail
        hook::jump(0x00730250, &detour_e_effect_recompute_techniques_by_detail);

        // eEffect::SetPCABlendData
        hook::jump(0x0071E540, &detour_e_effect_set_pca_blend_data);



        // ShaderLib::Init
        hook::jump(0x00756AA0, &detour_shader_lib_init);

        // ShaderLib::Close
        hook::jump(0x0073E6A0, &detour_shader_lib_close);

        // ShaderLib::RecomputeTechniquesByDetail
        hook::jump(0x0073E7E0, &detour_shader_lib_recompute_techniques_by_detail);
    }
}
