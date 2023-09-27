#pragma once

#include <unordered_map>

#include <glad/glad.h>

#ifdef SEAL_WEBGL
#include <GLFW/glfw3.h>

#define SEAL_GLES_3

#else

#define SEAL_GL_4

#endif

#include "seal/log/log.hpp"
#include "seal/types/result.hpp"


namespace seal::gl {
	inline GLenum last_err()
	{
		return glGetError();
	}

	/**
	   Given a GL error return the failure message associated with it.

	   \param err: The error value of get the message of.
	 */
	inline std::string failure_message(const GLenum err)
	{
		const static std::unordered_map<GLenum, std::string> ERROR_MESSAGES = {
			{ GL_INVALID_ENUM, "Invalid enum provided." },
			{ GL_INVALID_VALUE, "Invalid value provided." },
			{ GL_INVALID_OPERATION, "Invalid operation provided." },
#ifdef GL_STACK_OVERFLOW
			{ GL_STACK_OVERFLOW, "Stack overflow." },
#ifdef GL_STACK_UNDERFLOW
#endif
			{ GL_STACK_UNDERFLOW, "Stack underflow." },
#endif
			{ GL_OUT_OF_MEMORY, "Out of memory." },
			{ GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid frame buffer operation." },
#ifdef GL_CONTEXT_LOST
			{ GL_CONTEXT_LOST, "Context lost." },
#endif
#ifdef GL_TABLE_TOO_LARGE // Not in all version of open GL.
			{ GL_TABLE_TOO_LARGE, "Table too large." },
#endif
		};

		const auto iterator = ERROR_MESSAGES.find(err);
		if(ERROR_MESSAGES.end() == iterator) {
			return fmt::format("Unknown failure (Code: {})", err);
		}

		return iterator->second;
	}

	/**
	   Returns a failure according to a gl error.

	   \param err: The gl error to create the failure from
	 */
	inline auto fail(const GLenum err)
	{
		const auto message = failure_message(err);
		seal::log::error("GL Error: {}", message);
		return ::seal::failure("GL failure: {}", message);
	}

	/**
	   Creates a failure from the most recent gl error.
	 */
	inline auto fail()
	{
		return fail(last_err());
	}

	inline const char *get_string(const GLenum _enum)
	{
		return reinterpret_cast<const char *>(glGetString(_enum));
	}

	namespace specs {
		inline i32 max_textures_per_fragment = 0;
		inline i32 max_textures_combined = 0;
		inline i32 uniform_buffer_offset_alignment = 0;
	}
}

#define SEAL_GL_VERIFY(expr)                                                                       \
	expr;                                                                                          \
	if(auto err = glGetError(); GL_NO_ERROR != err) {                                              \
		::seal::log::error("GL Failure: {}", #expr);                                               \
		throw ::seal::gl::fail(err);                                                               \
	}
