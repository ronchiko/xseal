#pragma once

#include <unordered_map>

#if SEAL_WEBGL
#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#define SEAL_GLES_3

#define SEAL_GL_NO_VAO

#else
#include <glad/glad.h>

#define SEAL_GL_4

#endif

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
			{ GL_STACK_OVERFLOW, "Stack overflow." },
			{ GL_STACK_UNDERFLOW, "Stack underflow." },
			{ GL_OUT_OF_MEMORY, "Out of memory." },
			{ GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid framebuffer operation." },
			{ GL_CONTEXT_LOST, "Context lost." },
#ifdef GL_TABLE_TOO_LARGE // Not in all version of open GL.
			{ GL_TABLE_TOO_LARGE, "Table too large." },
#endif
		};

		const auto iter = ERROR_MESSAGES.find(err);
		if(ERROR_MESSAGES.end() == iter) {
			return fmt::format("Unknown failure (Code: {})", err);
		}

		return iter->second;
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

	inline const char* get_string(GLenum _enum) {
		return reinterpret_cast<const char *>(glGetString(_enum));
	}

	namespace specs {
		inline i32 max_textures_per_fragment = 0;
		inline i32 max_textures_combined = 0;
	}
}

#define seal_gl_verify(expr)                                                                       \
	expr;                                                                                          \
	if(auto err = glGetError(); GL_NO_ERROR != err) {                                              \
		throw ::seal::gl::fail(err);                                                               \
	}
