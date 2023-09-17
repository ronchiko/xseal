#pragma once

#include <GLFW/glfw3.h>

#include "seal/types.hpp"
#include "seal/types/error_value.hpp"
#include "seal/types/failure.hpp"

namespace seal::glfw {
	/**
	   Returns a failure for the most recent glfw error.
	 */
	inline seal::failure fail(void)
	{
		const char *error_message;
		auto code = glfwGetError(&error_message);

		return seal::failure("GLFW Error, {} ({})", error_message, code);
	}
}

#define seal_glfw_verify(expr)                                                                     \
	if(auto result = (expr); ::seal::error_value_v<decltype(result)> == result) {                  \
		throw ::seal::glfw::fail();                                                               \
	}
