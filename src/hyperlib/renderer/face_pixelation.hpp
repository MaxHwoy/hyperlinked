#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    class face_pixelation final
    {
    public:
        void update();

        void render(::IDirect3DTexture9* back_buffer);

    private:
        view::base* view_;
        vector2 screen_;

    public:
        static inline bool& enabled = *reinterpret_cast<bool*>(0x00B74D94);

        static inline vector3& world_pos = *reinterpret_cast<vector3*>(0x00B779A0);

        static inline float& screen_scale_x = *reinterpret_cast<float*>(0x009E9B68);

        static inline float& screen_scale_y = *reinterpret_cast<float*>(0x009E8F84);
    };
}
