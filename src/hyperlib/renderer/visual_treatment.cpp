#include <hyperlib/renderer/visual_treatment.hpp>

namespace hyper
{
    void visual_treatment::motion_blur(view_id id)
    {
        call_function<void(__thiscall*)(visual_treatment*, view_id)>(0x00726430)(this, id);
    }

    void visual_treatment::uves_over_cliff(view_id id)
    {
        call_function<void(__thiscall*)(visual_treatment*, view_id)>(0x007268E0)(this, id);
    }

    void visual_treatment::uves_over_cliff_darken(view_id id)
    {
        call_function<void(__thiscall*)(visual_treatment*, view_id)>(0x00726A00)(this, id);
    }

    void visual_treatment::final_render(view_id id, ::IDirect3DTexture9* back_buffer, ::IDirect3DTexture9* depth_buffer)
    {
        call_function<void(__thiscall*)(visual_treatment*, view_id, ::IDirect3DTexture9*, ::IDirect3DTexture9*)>(0x00726AF0)(this, id, back_buffer, depth_buffer);
    }

    void visual_treatment::update(const view::instance& view)
    {
        call_function<void(__thiscall*)(visual_treatment*, const view::instance&)>(0x007D2470)(this, view);
    }
}
