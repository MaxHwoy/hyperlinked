#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
	array<view::platform_info, static_cast<size_t>(view_id::count)> view::platform_info::infos(0x00B1A780);

	array<view::instance, static_cast<size_t>(view_id::count)> view::instance::views(0x00B4AF10);

	auto view::platform_interface::get_visible_state_sb(const bounds& bounds) const -> visible_state
	{
		BENCHMARK();

		std::uint32_t lod_increment = 1;

		const clipping_planes& clipping = this->pinfo->clipping_planes;

		for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(clipping_plane_type::count); ++i)
		{
			const plane& plane = clipping.planes[i];

			float ext_dot =
				::fabs(plane.normal.x) * bounds.extent.x +
				::fabs(plane.normal.y) * bounds.extent.y +
				::fabs(plane.normal.z) * bounds.extent.z;

			float cen_dot =
				plane.normal.x * bounds.center.x +
				plane.normal.y * bounds.center.y +
				plane.normal.z * bounds.center.z +
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

	auto view::platform_interface::get_visible_state_sb(const vector3& point, const matrix4x4* trs) const -> visible_state
	{
		BENCHMARK();

		vector3 local(point);

		if (trs != nullptr)
		{
			math::transform_point(*trs, local);
		}

		const clipping_planes& clipping = this->pinfo->clipping_planes;

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

	auto view::platform_interface::get_visible_state_sb(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> visible_state
	{
		BENCHMARK();

		std::uint32_t lod_increment = 1;

		vector3 center(bbox_min);
		vector3 extent(bbox_max);

		if (trs != nullptr)
		{
			math::transform_bound(*trs, center, extent);
		}

		center = (center + extent) * 0.5f;
		extent = (extent - center);

		const clipping_planes& clipping = this->pinfo->clipping_planes;

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

	void view::platform_interface::get_screen_position(const vector3& world_pos, vector3& screen_pos) const
	{
		BENCHMARK();

		vector4 local(world_pos);

		math::transform_point(this->pinfo->view_projection_matrix, local);

		float inverse = 1.0f / local.w;

		screen_pos.x = (local.x * inverse + 1.0f) * directx::resolution_x * +0.5f;
		screen_pos.y = (local.y * inverse - 1.0f) * directx::resolution_y * -0.5f;
		screen_pos.z = (local.z * inverse);
	}

	auto view::instance::get_flare_intensity() const -> float
	{
		if (this->is_in_a_tunnel(true))
		{
			return 0.0f;
		}

		if (this->rain != nullptr)
		{
			return 1.0f - this->rain->cloud_intensity;
		}

		return 1.0f;
	}

	bool view::instance::is_in_a_tunnel(bool check_overpass) const
	{
		if (this->rain == nullptr)
		{
			return false;
		}

		return this->rain->in_tunnel || (check_overpass && this->rain->in_overpass);
	}

	auto view::instance::get_camera_mover() const -> camera_mover*
	{
		return call_function<camera_mover*(__thiscall*)(const hyper::view::instance*)>(0x00412260)(this);
	}

	auto view::instance::get_screen_depth(const vector3& point, const matrix4x4* trs) const -> float
	{
		vector4 local(point);

		if (trs != nullptr)
		{
			math::transform_point(*trs, local);
		}

		math::transform_point(this->pinfo->view_projection_matrix, local);

		return (this->camera->current_key.far_clip - local.z) * 100.0f;
	}

	auto view::instance::get_screen_depth(const vector3& bbox_min, const vector3& bbox_max, const matrix4x4* trs) const -> float
	{
		return this->get_screen_depth((bbox_min + bbox_max) * 0.5f, trs);
	}
}
