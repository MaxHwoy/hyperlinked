#include <hyperlib/renderer/camera.hpp>
#include <hyperlib/renderer/targets.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    void render_target::ctor(bool act, render_target_id tid, hyper::view_id vid, std::uint32_t x, std::uint32_t y, ::IDirect3DSurface9* target, ::IDirect3DSurface9* depth_stencil)
    {
        this->active = act;
        this->target_id = tid;
        this->view_id = vid;
        this->resolution_x = x;
        this->resolution_y = y;
        this->d3d_target = target;
        this->d3d_depth_stencil = depth_stencil;
    }

    void render_view::update(const view::instance& view)
    {
        const camera* camera = view.camera;

        if (camera != nullptr)
        {
            this->has_camera = true;

            camera->get_forward_vector(this->camera_forward);
            camera->get_up_vector(this->camera_up);

            this->camera_position = camera->current_key.position.as_vector3();
            this->camera_prev_forward = camera->previous_key.direction.as_vector3();
            this->camera_velocity = camera->velocity_key.position.as_vector3();
            this->camera_view_matrix = camera->current_key.view_matrix;
            this->camera_focal_distance = camera->current_key.focal_distance;
            this->camera_depth_of_field = camera->current_key.depth_of_field;
            this->camera_dof_falloff = camera->current_key.dof_falloff;
            this->camera_dof_max_intensity = camera->current_key.dof_max_intensity;
        }
        else
        {
            this->has_camera = false;
        }

        if (view.camera_mover_list.empty())
        {
            this->has_camera_mover = false;
        }
        else
        {
            const camera_mover* mover = view.camera_mover_list.begin()->vtable_fix();

            this->has_camera_mover = true;
            this->camera_mover_type = mover->type;
            this->outside_view_type = mover->outside_pov();
        }

        if (view.pinfo != nullptr)
        {
            this->view_matrix = view.pinfo->view_matrix;
            this->projection_matrix = view.pinfo->projection_matrix;
            this->view_projection_matrix = view.pinfo->view_projection_matrix;
            this->non_jittered_view_projection_matrix = view.pinfo->non_jittered_view_projection_matrix;
        }
    }
}
