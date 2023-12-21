#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    enum class view_id : std::uint32_t;
    
    class depth_of_field final
    {
    public:
        depth_of_field();

        ~depth_of_field();

        bool active(view_id id);

        void update(::IDirect3DTexture9* back_buffer, bool split_screen);

    public:
        static void ctor(depth_of_field& dof);

        static void dtor(depth_of_field& dof);

    private:
        ::IDirect3DTexture9* texture_;

    public:
        static inline depth_of_field& instance = *reinterpret_cast<depth_of_field*>(0x00AB2140);

        static inline float& override_focal_distance = *reinterpret_cast<float*>(0x00AB0B54);

        static inline float& override_dof_value = *reinterpret_cast<float*>(0x00AB0B58);
    };
}
