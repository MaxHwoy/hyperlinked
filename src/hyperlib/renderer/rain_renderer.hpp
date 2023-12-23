#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/renderer/poly_manager.hpp>
#include <hyperlib/renderer/view.hpp>

namespace hyper
{
    enum class curtain_status : std::uint32_t
    {
        inactive,
        active,
        turn_on,
        override,
    };

    enum class rain_wind_type : std::uint32_t
    {
        point,
        vector,
        count,
    };

    struct on_screen_rain_entry
    {
        float data[6];
        std::uint32_t index;
    };

    struct on_screen_rain
    {
        std::uint32_t entry_count;
        on_screen_rain_entry entries[20];
    };

    struct rain
    {
    public:
        void update_and_render();

    public:
        on_screen_rain screen_rain;
        __declspec(align(0x04)) bool no_rain;
        __declspec(align(0x04)) bool no_rain_ahead;
        __declspec(align(0x04)) bool in_tunnel;
        __declspec(align(0x04)) bool in_overpass;
        __declspec(align(0x04)) struct track_path_zone* zone;
        char padding_0x0244[8];
        vector3pad out_vex;
        vector2 position;
        char padding_0x0268[8];
        vector3pad local_cam_velocity;
        curtain_status rain_curtain;
        std::uint32_t render_count;
        view::instance* view;
        float rain_intensity;
        float cloud_intensity;
        std::uint32_t desired_active;
        std::uint32_t raindrop_count[2];
        vector3pad raindrop_positions[350][2];
        char old_swap_buffer[350][4];
        std::uint32_t rain_point_count;
        std::uint32_t num_of_type[2];
        std::uint32_t desired_num_of_type[2];
        float percentages[2];
        float precip_wind_effect[2][2];
        texture::info* textures[2];
        char padding_0x340C[4];
        vector3pad old_car_direction;
        vector3pad precip_radius[2];
        vector3pad precip_speed_range[2];
        float precip_z_constant[2];
        rain_wind_type wind_type[2];
        float camera_speed;
        char padding_0x3474[12];
        vector3pad wind_speed;
        vector3pad desired_wind_speed;
        float desired_wind_time;
        float wind_time;
        std::uint32_t fog_red;
        std::uint32_t fog_green;
        std::uint32_t fog_blue;
        vector2 bbox_max;
        vector2 bbox_min;
        char padding_0x34C4[12];
        e_poly precip_poly[2];
        matrix4x4 local_to_world;
        matrix4x4 world_to_local;
        float len_modifier;
        float desired_intensity;
        float desired_cloudyness;
        float desired_road_dampness;
        float road_dampness;
        float precip_percent[2];
        char padding_0x36AC[4];
        vector3pad prevail_wind_speed;
        float weather_time;
        float desired_weather_time;
        vector3pad velocities[2][10];
        vector2 ent0;
        vector2 ent1;
        vector2 ext0;
        vector2 ext1;
        char data_0x3828[6488];
    };

    class rain_renderer final
    {
    private:
        struct poly
        {
            struct {
                vector3 position;
                color32 color;
                vector2 uv;
                vector4 vector;
                float adder;
            } vertices[4];
        };

    public:
        rain_renderer(std::uint32_t vertex_format, std::uint32_t max_polies);

        ~rain_renderer();

        bool render(::IDirect3DSurface9* surface_dst, ::IDirect3DTexture9* texture_src);

    public:
        static void ctor(rain_renderer& renderer);

        static void dtor(rain_renderer& renderer);

    private:
        ::IDirect3DVertexBuffer9* vertex_buffer_[2];
        ::IDirect3DIndexBuffer9* index_buffer_;
        std::uint32_t max_vertices_[2];
        std::uint32_t unknown_[2];
        std::uint32_t vertex_count_[2];
        std::uint32_t min_vertex_index_[2];
        std::uint32_t vertex_size_;
        bool locked_[2];
        poly* polies_[2];
        std::uint32_t curr_vert_offset_[2];

    public:
        static inline rain_renderer& instance = *reinterpret_cast<rain_renderer*>(0x00AAF558);

    private:
        static inline std::uint32_t& max_rain_polies_ = *reinterpret_cast<std::uint32_t*>(0x009E7FD4);
    };

    ASSERT_SIZE(on_screen_rain_entry, 0x1C);
    ASSERT_SIZE(on_screen_rain, 0x234);
    ASSERT_SIZE(rain, 0x5180);
    ASSERT_SIZE(rain_renderer, 0x44);
}
