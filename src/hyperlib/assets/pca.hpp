#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class pca final
    {
    public:
        struct channel_info
        {
            std::uint8_t type;
            std::uint8_t weight_count;
            std::uint16_t vector_buffer_offset;
            std::uint16_t weight_offset;
            std::uint32_t param_handle;
        };

        struct ucap_frame_weights
        {
            std::uint16_t frame_count;
            std::uint16_t weights_per_frame_count;
            std::uint16_t sample_count;
            std::uint16_t channel_count;
            channel_info channel_infos[9];
            std::uint32_t feature_heights_param_handle;
            vector4* feature_heights;
            float* min_values;
            float* ranges;
            float* vector_buffer;
            std::uint16_t* short_weights;
        };

        struct weights
        {
            char name[0x20];
            vector4 average; // ???
            std::uint16_t frame_count;
            std::uint16_t weights_per_frame_count;
            std::uint16_t sample_count;
            std::uint16_t channel_count;
            channel_info channel_infos[9];
            float* mean;
            float* frames;
            std::uint32_t pad;
        };

        struct blend_data
        {
            std::uint32_t curr_frame;
            std::uint32_t next_frame;
            float blend;
            ucap_frame_weights* ucap_weight;
            weights* pca_weight;
        };

    public:
        static auto blend_ucap(const ucap_frame_weights& weights, std::uint8_t type, std::uint32_t curr_frame, std::uint32_t next_frame, float blend) -> float*;
    };

    ASSERT_SIZE(pca::channel_info, 0x0C);
    ASSERT_SIZE(pca::ucap_frame_weights, 0x8C);
    ASSERT_SIZE(pca::weights, 0xB0);
    ASSERT_SIZE(pca::blend_data, 0x14);
}
