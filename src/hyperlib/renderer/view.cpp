#include <hyperlib/assets/track.hpp>
#include <hyperlib/memory/frame_pool.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/targets.hpp>
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

	auto view::instance::calculate_pixelation(std::uint16_t horizontal_fov) -> float
	{
		float tan = math::tan(static_cast<std::uint16_t>(horizontal_fov >> 1));

		return tan == 0.0f ? 10.0f : (static_cast<float>(directx::resolution_x >> 1) / tan);
	}

	void view::instance::calculate_view_matrices(float near_clip, float far_clip, bool calculate_frustum)
	{
		if (directx::pixel_aspect_ratio == 1.0f)
		{
			directx::recalculate_pixel_ratio(false);
		}

		view::platform_info* info = this->pinfo;

		if (this->camera != nullptr)
		{
			info->view_matrix = this->camera->current_key.view_matrix;

			if (near_clip == 0.0f)
			{
				near_clip = this->camera->current_key.near_clip;
			}

			if (far_clip == 0.0f)
			{
				far_clip = this->camera->current_key.far_clip;
			}

			float aspect_xy;

			if (this->id == view_id::player1_reflection || this->id == view_id::player2_reflection)
			{
				render_target* target = view::instance::views[this->id - view_id::player1_reflection + view_id::player1].attached_target;

				aspect_xy = static_cast<float>(target->resolution_x) / static_cast<float>(target->resolution_y);
			}
			else
			{
				render_target* target = this->attached_target;

				aspect_xy = static_cast<float>(target->resolution_x) / static_cast<float>(target->resolution_y);
			}

			float w_divident = 1.0f;
			float h_divident = 1.0f;

			if (this->id == view_id::player1 || this->id == view_id::player1_reflection || this->id == view_id::player2 || this->id == view_id::player2_reflection)
			{
				w_divident = directx::pixel_aspect_ratio;
				h_divident = 1.1594204f;
			}

			float h = h_divident / math::tan(static_cast<std::uint16_t>(this->camera->current_key.horizontal_fov >> 1));
			float w = h / (w_divident * aspect_xy);

			math::create_projection_matrix(w, h, near_clip, far_clip, info->projection_matrix);
			math::create_projection_matrix(w, h, near_clip + 0.005f, far_clip + 0.005f, info->non_jittered_projection_matrix);

			this->pixelation = view::instance::calculate_pixelation(this->camera->current_key.horizontal_fov);

			math::multiply_matrix(info->view_matrix, info->projection_matrix, info->view_projection_matrix);
			
			if (calculate_frustum)
			{
				const matrix4x4& view_projection = info->view_projection_matrix;

				vector4& plane0 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::left)].as_vector4();
				vector4& plane1 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::right)].as_vector4();
				vector4& plane2 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::bottom)].as_vector4();
				vector4& plane3 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::top)].as_vector4();
				vector4& plane4 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::near)].as_vector4();
				vector4& plane5 = info->clipping_planes.planes[static_cast<std::uint32_t>(clipping_plane_type::far)].as_vector4();

				plane0.x = view_projection.m14 + view_projection.m11;
				plane0.y = view_projection.m24 + view_projection.m21;
				plane0.z = view_projection.m34 + view_projection.m31;
				plane0.w = view_projection.m44 + view_projection.m41;

				plane1.x = view_projection.m14 - view_projection.m11;
				plane1.y = view_projection.m24 - view_projection.m21;
				plane1.z = view_projection.m34 - view_projection.m31;
				plane1.w = view_projection.m44 - view_projection.m41;

				plane2.x = view_projection.m14 + view_projection.m12;
				plane2.y = view_projection.m24 + view_projection.m22;
				plane2.z = view_projection.m34 + view_projection.m32;
				plane2.w = view_projection.m44 + view_projection.m42;

				plane3.x = view_projection.m14 - view_projection.m12;
				plane3.y = view_projection.m24 - view_projection.m22;
				plane3.z = view_projection.m34 - view_projection.m32;
				plane3.w = view_projection.m44 - view_projection.m42;

				plane4.x = view_projection.m13;
				plane4.y = view_projection.m23;
				plane4.z = view_projection.m33;
				plane4.w = view_projection.m43;

				plane5.x = view_projection.m14 - view_projection.m13;
				plane5.y = view_projection.m24 - view_projection.m23;
				plane5.z = view_projection.m34 - view_projection.m33;
				plane5.w = view_projection.m44 - view_projection.m43;

				plane0 *= 1.0f / ::sqrt(plane0.x * plane0.x + plane0.y * plane0.y + plane0.z * plane0.z);
				plane1 *= 1.0f / ::sqrt(plane1.x * plane1.x + plane1.y * plane1.y + plane1.z * plane1.z);
				plane2 *= 1.0f / ::sqrt(plane2.x * plane2.x + plane2.y * plane2.y + plane2.z * plane2.z);
				plane3 *= 1.0f / ::sqrt(plane3.x * plane3.x + plane3.y * plane3.y + plane3.z * plane3.z);
				plane4 *= 1.0f / ::sqrt(plane4.x * plane4.x + plane4.y * plane4.y + plane4.z * plane4.z);
				plane5 *= 1.0f / ::sqrt(plane5.x * plane5.x + plane5.y * plane5.y + plane5.z * plane5.z);

				math::multiply_matrix(info->view_matrix, info->non_jittered_projection_matrix, info->non_jittered_view_projection_matrix);
			}
		}
		else
		{
			info->view_matrix = matrix4x4::identity();
			info->projection_matrix = matrix4x4::identity();
		}
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

	void view::instance::setup_world_light_context()
	{
		this->world_light_context = nullptr;

		lighting::dynamic_context* context = frame_pool::instance.malloc<lighting::dynamic_context>();

		if (context != nullptr)
		{
			const vector3& camera_pos = this->camera->current_key.position.as_vector3();
			const matrix4x4& camera_trs = this->camera->current_key.view_matrix;

			vector3 center;

			center.x = camera_pos.x - camera_trs.m13 * 10.0f; // direction.x denormalized
			center.y = camera_pos.y - camera_trs.m23 * 10.0f; // direction.y denormalized
			center.z = camera_pos.z - camera_trs.m33 * 10.0f + 4.0f; // direction.z denormalized

			lighting::setup_light_context(*context, lighting::shaper_light_rigorous::world, &matrix4x4::identity(), &view::instance::super_rotation, &center, this);

			math::create_look_at_matrix(sun_info::sun_origin, vector3::zero(), vector3::up(), context->local_eye);

			this->world_light_context = context;
		}
	}
}
