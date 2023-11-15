#include <hyperlib/assets/pca.hpp>

namespace hyper
{
    pca::ucap_binder pca::ucap_bind = nullptr;

    auto pca::blend_ucap(const ucap_frame_weights& weights, std::uint8_t type, std::uint32_t curr_frame, std::uint32_t next_frame, float blend) -> float*
    {
        const pca::channel_info& info = weights.channel_infos[type];

        float* vector_ptr = weights.vector_buffer + info.vector_buffer_offset;
        float* ranges = weights.ranges + info.weight_offset;
        float* min_values = weights.min_values + info.weight_offset;

        std::uint16_t* short_curr = weights.short_weights + info.weight_offset + curr_frame * weights.weights_per_frame_count;
        std::uint16_t* short_next = weights.short_weights + info.weight_offset + next_frame * weights.weights_per_frame_count;

        for (std::uint32_t i = 0u; i < info.weight_count; ++i)
        {
            vector_ptr[i] = min_values[i] + (math::lerp(static_cast<float>(short_curr[i]), static_cast<float>(short_next[i]), blend) * 0.000015259022f * ranges[i]);
        }

        return vector_ptr;
    }
}
