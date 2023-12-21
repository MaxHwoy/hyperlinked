#include <hyperlib/renderer/visual_treatment.hpp>

namespace hyper
{
    void visual_treatment::final_render(view_id id, ::IDirect3DTexture9* back_buffer, ::IDirect3DTexture9* depth_buffer)
    {
        call_function<void(__thiscall*)(visual_treatment*, view_id, ::IDirect3DTexture9*, ::IDirect3DTexture9*)>(0x00726AF0)(this, id, back_buffer, depth_buffer);
    }
}
