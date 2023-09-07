#pragma once

#include <cstdint>

#include <hyperlib/math.hpp>

namespace hyper
{
    enum class block_id : std::uint32_t
    {
        empty                       = 0x00000000,

        smokeable_spawners          = 0x00034027,

        scenery_header              = 0x00034101,
        scenery_infos               = 0x00034102,
        scenery_instances           = 0x00034103,
        scenery_tree_nodes          = 0x00034105,
        scenery_override_hooks      = 0x00034106,
        scenery_preculler_infos     = 0x00034107,
        scenery_override_infos      = 0x00034108,
        scenery_groups              = 0x00034109,
        scenery_ngbbs               = 0x0003410A,
        model_hierarchy_instance    = 0x0003410C,
        light_texture_collections   = 0x0003410D,
        vectorized_cull_infos       = 0x0003410E,
        vectorized_inst_infos       = 0x0003410F,
        track_streaming_sections    = 0x00034110,
        track_streaming_infos       = 0x00034111,
        track_streaming_barriers    = 0x00034112,
        track_streaming_discs       = 0x00034113,

        track_position_markers      = 0x00034146,
        track_path_points           = 0x00034148,
        track_path_lanes            = 0x00034149,
        track_path_zones            = 0x0003414A,
        track_path_areas            = 0x0003414C,
        track_path_barriers         = 0x0003414D,

        visible_section_pack_header = 0x00034151,
        visible_section_boundaries  = 0x00034152,
        visible_section_drivables   = 0x00034153,
        visible_section_specifics   = 0x00034154,
        visible_section_loadings    = 0x00034155,
        visible_section_elev_polies = 0x00034156,
        visible_section_remap_table = 0x00034157,
        visible_section_overlay     = 0x00034158,
        heli_section_manager        = 0x00034159,

        track_object_bounds         = 0x00034191,

        track_infos                 = 0x00034201,
        sun_infos                   = 0x00034202,

        generic_regions             = 0x00034250,

        vectorized_cull_header      = 0x00034C03,

        event_trigger_pack_header   = 0x00036001,
        event_trigger_tree_nodes    = 0x00036002,
        event_trigger_instances     = 0x00036003,

        world_anim_header           = 0x00037220,
        world_anim_frames           = 0x00037240,
        world_anim_rtnode           = 0x00037250,
        world_anim_counts           = 0x00037260,
        world_anim_endptr           = 0x00037270,

        parameter_map_layer_header  = 0x0003B602,
        parameter_map_field_types   = 0x0003B603,
        parameter_map_field_offsets = 0x0003B604,
        parameter_map_param_data    = 0x0003B605,
        parameter_map_quad_data_8   = 0x0003B607,
        parameter_map_quad_data_16  = 0x0003B608,

        world_road_network          = 0x0003B800,
        world_collision_assets      = 0x0003B801,
        world_grid_maker            = 0x0003B802,

        event_system_instance       = 0x0003B811,
        
        collision_object            = 0x0003B901,

        emitter_system              = 0x0003BC00,



        texture_pack_info_header    = 0x33310001,
        texture_pack_info_entries   = 0x33310002,
        texture_pack_info_stream    = 0x33310003,
        texture_pack_info_textures  = 0x33310004,
        texture_pack_info_plats     = 0x33310005,
        texture_pack_anim_header    = 0x33312001,
        texture_pack_anim_frames    = 0x33312002,
        texture_pack_vram_header    = 0x33320001,
        texture_pack_vram_data      = 0x33320002,



        scenery_section             = 0x80034100,

        model_hierarchy_tree        = 0x8003410B,

        track_path_manager          = 0x80034147,

        visible_section_manager     = 0x80034150,

        event_trigger_pack          = 0x80036000,

        parameter_maps              = 0x8003B600,
        parameter_map_layer         = 0x8003B601,

        event_systems               = 0x8003B810,

        collision_volumes           = 0x8003B900,



        texture_pack                = 0xB3300000,
        texture_pack_info           = 0xB3310000,
        texture_pack_anim_pack      = 0xB3312000,
        texture_pack_anim_inst      = 0xB3312004,
        texture_pack_vram           = 0xB3320000,
    };

    class chunk
    {
    public:
        inline chunk(block_id id, std::uint32_t size) : id_(static_cast<std::uint32_t>(id)), size_(size)
        {
        }

        inline auto id() const -> block_id
        {
            return static_cast<block_id>(this->id_);
        }

        inline auto size() const -> std::uint32_t
        {
            return this->size_;
        }

        inline auto data() const -> void*
        {
            return const_cast<chunk*>(this) + 1;
        }

        inline auto aligned_data(size_t alignment) const -> void*
        {
            return reinterpret_cast<void*>(math::align_pow_2(reinterpret_cast<uintptr_t>(this->data()), alignment));
        }

        inline auto aligned_size(size_t alignment) const -> std::uint32_t
        {
            return this->size_ - static_cast<std::uint32_t>((reinterpret_cast<uintptr_t>(this->aligned_data(alignment)) - reinterpret_cast<uintptr_t>(this->data())));
        }

        inline auto end() const -> chunk*
        {
            return reinterpret_cast<chunk*>(reinterpret_cast<uintptr_t>(this->data()) + this->size_);
        }

        inline bool is_container() const
        {
            return (this->id_ & 0x80000000u) != 0u;
        }

        inline void set_id(block_id id)
        {
            this->id_ = static_cast<std::uint32_t>(id);
        }

        inline void set_size(std::uint32_t size)
        {
            this->size_ = size;
        }

    private:
        std::uint32_t id_;
        std::uint32_t size_;
    };
}
