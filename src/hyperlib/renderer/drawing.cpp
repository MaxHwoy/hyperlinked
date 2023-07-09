#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    bitset<static_cast<size_t>(view_id::count)> renderer::flare_mask_{};

    bitset<static_cast<size_t>(view_id::count)> renderer::view_to_flare_({
        static_cast<size_t>(view_id::player1),
        static_cast<size_t>(view_id::player2),
        static_cast<size_t>(view_id::player1_rvm),
        static_cast<size_t>(view_id::player1_reflection),
        static_cast<size_t>(view_id::player2_reflection),
        static_cast<size_t>(view_id::env_z_pos),
        static_cast<size_t>(view_id::env_x_pos),
        static_cast<size_t>(view_id::env_z_neg),
        static_cast<size_t>(view_id::env_x_neg),
        static_cast<size_t>(view_id::env_y_pos),
        static_cast<size_t>(view_id::env_y_neg),
    });

    auto renderer::create_flare_view_mask(view_id id) -> std::uint32_t
    {
        return (1u << static_cast<std::uint32_t>(id)) | ((id == view_id::player1) * 0x50u) | ((id == view_id::player2) * 0x80u);
    }

    bool renderer::can_render_flares_in_view(view_id id)
    {
        return renderer::view_to_flare_.get(id);
    }

    bool renderer::is_friend_flare_view_already_committed(view_id id)
    {
        std::uint32_t& mask = *reinterpret_cast<std::uint32_t*>(0x00B42F00);

        mask |= 1u << static_cast<std::uint32_t>(id);

        switch (id)
        {
            case view_id::player1:
                return (mask & (1u << static_cast<std::uint32_t>(view_id::player1_reflection))) != 0;
        
            case view_id::player2:
                return (mask & (1u << static_cast<std::uint32_t>(view_id::player2_reflection))) != 0;
        
            case view_id::player1_reflection:
                return (mask & (1u << static_cast<std::uint32_t>(view_id::player1))) != 0;
        
            case view_id::player2_reflection:
                return (mask & (1u << static_cast<std::uint32_t>(view_id::player2))) != 0;
        
            default:
                return false;
        }

        /*
        renderer::flare_mask_.set(id, true);

        switch (id)
        {
            case view_id::player1:
                return renderer::flare_mask_.get(view_id::player1_reflection);

            case view_id::player2:
                return renderer::flare_mask_.get(view_id::player2_reflection);

            case view_id::player1_reflection:
                return renderer::flare_mask_.get(view_id::player1);

            case view_id::player2_reflection:
                return renderer::flare_mask_.get(view_id::player2);

            default:
                return false;
        }
        */
    }

    auto renderer::get_next_light_flare_in_pool(std::uint32_t mask) -> flare::instance*
    {
        std::uint32_t& count = *reinterpret_cast<std::uint32_t*>(0x00B42EFC);

        if (count < 1000u)
        {
            renderer::flare_bits_[count] = mask;

            return &renderer::flare_pool_[count++];
        }

        return nullptr;
    }

    void renderer::remove_current_light_flare_in_pool()
    {
        --*reinterpret_cast<std::uint32_t*>(0x00B42EFC);
    }
}
