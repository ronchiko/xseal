#pragma once

#include <vector>

#include "seal/types.hpp"
#include "seal/types/result.hpp"

/**
 * API for frontends.
 */
namespace seal::api {
	/**
	 * Creates a new window.
	 *
	 * \param scale: The scale of the window
	 * \param title: The title of the window
	 *
	 * \return: The id of the created window
	 */
	extern result<id> create_window(vec2 scale, const std::string& title);

	/**
	 * Closes a window by its ID.
	 */
	extern void destroy_window(id);

	/**
	 * Gets all the existing windows.
	 */
	extern std::vector<id> query_windows();

	/**
	 * Checks if a window should close.
	 */
	extern bool should_window_close(id);
}
