#include <hyperlib/renderer/view.hpp>

namespace hyper
{
	auto view_platform_interface::get_visible_state_sb(const vector3& point, const matrix4x4* trs) const -> visible_state
	{
		vector3 local(point);

		if (trs != nullptr)
		{
			math::transform_point(*trs, local);
		}

		const clipping_planes& clipping = this->info->clipping_planes;

		for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(clipping_plane_type::count); ++i)
		{
			const plane& plane = clipping.planes[i];

			float dot = 
				plane.normal.x * local.x +
				plane.normal.y * local.y +
				plane.normal.z * local.z +
				plane.distance;

			if (dot <= 0.0f)
			{
				return visible_state::outside;
			}
		}

		return visible_state::inside;
	}

    auto view_platform_interface::get_visible_state_sb(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> visible_state
    {
		std::uint32_t lod_increment = 1;

		vector3 center(bbox_min);
		vector3 extent(bbox_max);

		if (trs != nullptr)
		{
			math::transform_bound(*trs, center, extent);
		}

		center = (center + extent) * 0.5f;
		extent = (extent - center);

		const clipping_planes& clipping = this->info->clipping_planes;

		for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(clipping_plane_type::count); ++i)
		{
			const plane& plane = clipping.planes[i];

			float ext_dot = 
				::fabs(plane.normal.x) * extent.x +
				::fabs(plane.normal.y) * extent.y +
				::fabs(plane.normal.z) * extent.z;

			float cen_dot = 
				plane.normal.x * center.x +
				plane.normal.y * center.y +
				plane.normal.z * center.z +
				plane.distance;

			if (cen_dot + ext_dot < 0.0f)
			{
				return visible_state::outside;
			}

			if (cen_dot - ext_dot < 0.0f)
			{
				lod_increment = 0u;
			}
		}

		return static_cast<visible_state>(lod_increment + 1u);
    }
}
