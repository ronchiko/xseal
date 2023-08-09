#pragma once

#include "seal/types.hpp"
#include "seal/types/color.hpp"
#include "seal/types/result.hpp"


namespace seal::api {
	/**
	   Initializes the seal backend.
	 */
	result<void> initialize_backend();

	/**
	   Updates the current resolution.
	 */
	result<void> update_resolution(const v2<u32>& resolution);

	/**
	   Starts a rendering context.
	 */
	result<void> start_context(const color& flush_color);

	/**
	   Ends a rendering context.
	 */
	void end_context();
}
