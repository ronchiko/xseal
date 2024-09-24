#pragma once

#include "seal/types.hpp"

namespace seal {
	/**
	   A single unit in the engine.
	 */
	inline constexpr f32 UNIT = 1.f;

	inline constexpr v3<f32> RIGHT = { UNIT, 0, 0 };
	inline constexpr v3<f32> UP = { 0, UNIT, 0 };
	inline constexpr v3<f32> FORWARD = { 0, 0, UNIT };

	struct transform
	{
		transform() = default;

		v3<f32> position = {0, 0, 0};
		quat rotation = glm::quat_identity<f32, glm::defaultp>();
		v3<f32> scale = {1, 1, 1};
	};

	using Transform = transform;
}
