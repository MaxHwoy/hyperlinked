#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    struct car_loader
    {
        void* callback;
        void* param;
        int loading_mode;
        int in_front_end_flag;
        int two_player_flag;
        int loading_in_progress;
        float start_loading_time;
        int num_loaded_ride_infos;
        int num_allocated_ride_infos;
        int may_need_defragmentation;
        void* loaded_solidPackList[2];
        void* loaded_texture_pack_list[2];
        void* loaded_skin_layer_list[2];
        void* loaded_vinyl_layer_list[2];
        void* loaded_skin_list[2];
        void* loaded_car_list[2];
        int wtf;
        void* loaded_ride_info_list[2];
        void* loaded_ride_info;
        void* memory_pool_heap;
        std::uint32_t memory_pool_size;
        std::uint32_t num_sponge_allocations;
        void* sponge_allocations[2];
    };
}
