#pragma once

#include <unordered_map>

#if SEAL_WEBGL
#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#define SEAL_GLES_2

#define SEAL_GL_NO_VAO

#else
#include <glad/glad.h>

#define SEAL_GL_4

#endif

#include "seal/types/result.hpp"

namespace seal::gl {
	inline auto fail(const GLenum err)
	{
		const static std::unordered_map<GLenum, const char *> ERROR_MESSAGES = {
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
			return ::seal::failure("Unexpected GL failure");
		}

		return ::seal::failure("GL failure: {}", iter->second);
	}
}

#define seal_gl_verify(expr)                                                                       \
	expr;                                                                                          \
	if(auto err = glGetError(); GL_NO_ERROR == err) {                                              \
		return ::seal::gl::fail(err);                                                              \
	}
