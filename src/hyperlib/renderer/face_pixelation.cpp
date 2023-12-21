#include <hyperlib/gameplay/game_flow.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/face_pixelation.hpp>

namespace hyper
{
    void face_pixelation::update()
    {
        if (game_flow::manager::instance.current_state == game_flow::state::racing && face_pixelation::enabled)
        {
            vector3 screen_pos;

            this->view_->get_screen_position(face_pixelation::world_pos, screen_pos);

            this->screen_.x = screen_pos.x * face_pixelation::screen_scale_x / directx::resolution_x;
            this->screen_.y = screen_pos.y * face_pixelation::screen_scale_y / directx::resolution_y;
        }
    }

    void face_pixelation::render(::IDirect3DTexture9* back_buffer)
    {
        call_function<void(__cdecl*)(view::base*, ::IDirect3DTexture9*)>(0x00722FC0)(this->view_, back_buffer);
    }
}
