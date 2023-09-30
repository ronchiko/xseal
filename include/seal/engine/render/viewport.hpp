#pragma once

#include "seal/types.hpp"

namespace seal::viewport {
	/**
		Gets the size of the viewport.
	 */
	v2<f32> size();

	/**
		Returns the ratio between the width and height of the viewport.
	 */
	f32 ratio();
}
