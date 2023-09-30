#pragma once

#include "seal/types.hpp"

namespace seal::api
{
	/**
		Signals the engine core that the viewport was changed.
		Should be used only by front-ends to notify the engine that a window was resized.

		\param new_viewport_size: The new size of the viewport.
	 */
	void signal_viewport_change(v2<f32> new_viewport_size);

}
