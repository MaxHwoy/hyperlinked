#include <hyperlib/renderer/post_process.hpp>
#include <hyperlib/renderer/targets.hpp>

namespace hyper
{
    post_process::post_process() : surfaces_{}, textures_{}, data_{}
    {
        std::uint32_t x = vt_render_target::resolution_x = directx::resolution_x;
        std::uint32_t y = vt_render_target::resolution_y = directx::resolution_y;

        bool enable = options::motion_blur_enabled || options::rain_enabled || options::visual_treatment;

        if (enable)
        {
            bool failed = false;

            for (std::uint32_t i = 0u; i < post_process::target_count; ++i)
            {
                if (FAILED(directx::device()->CreateTexture(x, y, 1u, D3DUSAGE_RENDERTARGET, ::D3DFMT_A8R8G8B8, ::D3DPOOL_DEFAULT, &this->textures_[i], nullptr)))
                {
                    failed = true;

                    break;
                }

                if (FAILED(this->textures_[i]->GetSurfaceLevel(0u, &this->surfaces_[i])))
                {
                    failed = true;

                    break;
                }
            }

            if (failed)
            {
                post_process::dtor(*this);

                options::rain_enabled = false;
                options::motion_blur_enabled = false;
                options::pc_rain_option = 0u;
                options::pc_motion_blur_option = 0u;
            }
        }

        if (!post_process::inited)
        {
            post_process::inited = true; // blackbox???
        }
    }

    post_process::~post_process()
    {
        for (std::uint32_t i = 0u; i < post_process::target_count; ++i)
        {
            if (this->surfaces_[i] != nullptr)
            {
                this->surfaces_[i]->Release();

                this->surfaces_[i] = nullptr;
            }

            if (this->textures_[i] != nullptr)
            {
                this->textures_[i]->Release();

                this->textures_[i] = nullptr;
            }
        }
    }

    void post_process::ctor(post_process& process)
    {
        new (&process) post_process();
    }

    void post_process::dtor(post_process& process)
    {
        process.~post_process();
    }
}
