#pragma once

#include "seal/types.hpp"

namespace seal {
	struct transform
	{
		transform() = default;

		v3<f32> position = {};
		quat rotation = {};
		v3<f32> scale = {};
	};
}
