#pragma once

#include <variant>

#include "seal/engine/components/transform.hpp"
#include "seal/panic.h"
#include "seal/types.hpp"

namespace seal {
	// Forward declare the return type of the camera object.
	namespace system {
		struct camera_shader_uniform;
		class camera_manager;
	}

	enum class camera_state
	{
		Active,
		Inactive
	};

	/**
		The component containing a camera.
	 */
	class camera
	{
	public:
		static constexpr radians_t DEFAULT_FOV = glm::radians(45.f);
		static constexpr f32 DEFAULT_ZOOM = 1;

		static constexpr f32 DEFAULT_FAR_PLANE = 100.f;
		static constexpr f32 DEFAULT_NEAR_PLANE = .01f;

		struct perspective
		{
		public:
			radians_t fov;
			f32 near_plane;
			f32 far_plane;

			constexpr explicit perspective(const radians_t fov = DEFAULT_FOV,
										   const f32 near_plane = DEFAULT_NEAR_PLANE,
										   const f32 far_plane = DEFAULT_FAR_PLANE)
				: fov(fov)
				, near_plane(near_plane)
				, far_plane(far_plane)
			{
				seal_assert(near_plane < far_plane, "Near plane > Far Plane!");
			}

		private:
			friend class camera;

			/**
				Returns the camera's projection matrix.

				\param transform: The transform of the camera rendering the view.
			 */
			[[nodiscard]] m4<f32> compute_matrices(const transform& transform) const;
		};

		struct orthographic
		{
		public:
			f32 zoom;
			f32 near_plane;
			f32 far_plane;

			constexpr explicit orthographic(const f32 zoom = DEFAULT_ZOOM,
											const f32 near_plane = DEFAULT_NEAR_PLANE,
											const f32 far_plane = DEFAULT_FAR_PLANE)
				: zoom(zoom)
				, near_plane(near_plane)
				, far_plane(far_plane)
			{
				seal_assert(near_plane < far_plane, "Near plane > Far Plane!");
			}

		private:
			friend class camera;

			/**
				Returns the camera's projection matrix.

				\param transform: The transform of the camera rendering the view.
			 */
			[[nodiscard]] m4<f32> compute_matrices(const transform& transform) const;
		};

		camera() = default;

		/**
			Creates a new camera with perspective projection.

			\param perspective: The perspective projection information.
		 */
		static camera create(perspective perspective);

		/**
			Creates a new camera a orthographic projection.

			\param orthographic: The orthographic camera options.
		 */
		static camera create(orthographic orthographic);

		/**
			Checks if the camera is active.
		 */
		[[nodiscard]] constexpr bool is_active() const;

		/**
			Changes the camera state.

			\param state: The new camera state.
		 */
		void set_state(camera_state state);

	private:
		using variant_t = std::variant<perspective, orthographic>;

		friend class system::camera_manager;

		explicit camera(variant_t variant);

		/**
			Computes the projection matrix of the camera.

			\param transform: The transform of the camera.
		 */
		[[nodiscard]] m4<f32> compute_matrices(const transform& transform) const;

		bool m_Active = false;
		variant_t m_Camera = orthographic{};
	};
}

constexpr bool seal::camera::is_active() const
{
	return m_Active;
}
