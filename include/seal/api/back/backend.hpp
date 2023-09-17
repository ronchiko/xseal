#pragma once

#include "seal/types.hpp"
#include "seal/types/color.hpp"


namespace seal::api {
	/**
	   Initializes the seal backend.
	 */
	void initialize_backend();

	/**
	   Updates the current resolution.
	 */
	void update_resolution(const v2<u32>& resolution);

	/**
	   Starts a rendering context.
	 */
	void start_context(const color& flush_color);

	/**
	   Ends a rendering context.
	 */
	void end_context();
}
