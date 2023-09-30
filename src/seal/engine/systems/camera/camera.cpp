#include "seal/engine/systems/camera/camera.hpp"

#include "seal/engine/render/viewport.hpp"

namespace seal {
	m4<f32> camera::perspective::compute_matrices(const transform& transform) const
	{
		const auto size = viewport::size();

		return glm::perspective(fov, size.x / size.y, near_plane, far_plane);
	}

	m4<f32> camera::orthographic::compute_matrices(const transform& transform) const
	{
		const auto size = (viewport::size() * 0.5f) / zoom;

		return glm::ortho<f32>(-size.x, size.x , -size.y, size.y, near_plane, far_plane);
	}

	m4<f32> camera::compute_matrices(const transform& transform) const
	{
		// Just proxy the query to the underlying camera type.
		return std::visit([&transform](auto&& self) -> m4<f32> { return self.compute_matrices(transform); },
						  m_Camera);
	}

	camera camera::create(orthographic orthographic)
	{
		return camera{ orthographic };
	}

	camera camera::create(perspective perspective)
	{
		return camera{ perspective };
	}

	void camera::set_state(const camera_state state)
	{
		m_Active = state == camera_state::Active;
	}


	camera::camera(const variant_t variant)
		: m_Active(false)
		, m_Camera(variant)
	{}

}
