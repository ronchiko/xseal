#include "seal/api/back/backend.hpp"

#include "gl.hpp"

#if defined(SEAL_DEBUG) && defined(SEAL_GL_4)
namespace {
	void APIENTRY gl_debug_callback(GLenum source,
									GLenum type,
									GLuint id,
									GLenum severity,
									GLsizei length,
									const GLchar *message,
									const void *userParam)
	{
		switch(type) {
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			seal::log::error("GL Deprecated Behavior: {}", message);
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			seal::log::error("GL Undefined Behavior: {}", message);
			break;

		case GL_DEBUG_TYPE_ERROR:
			seal::log::error("GL Debug Error: {}", message);
			break;
		default:
			seal::log::error("Error {}: {}", type, message);
			break;
		}
	}
}
#endif

namespace seal::api {
	void initialize_backend()
	{
#ifdef SEAL_WEBGL
		if(0 == gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
			throw seal::failure("Failed to load OpenGL");
		}
#else
		if(0 == gladLoadGL()) {
			throw failure("Failed to load OpenGL");
		}
#endif

		// Load some general information variables
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gl::specs::max_textures_per_fragment);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &gl::specs::max_textures_combined);
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
					  &gl::specs::uniform_buffer_offset_alignment);

#if defined(SEAL_DEBUG) && defined(SEAL_GL_4)
		log::info("Enabling seal debug mode");
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(&gl_debug_callback, nullptr);
#endif

		log::info("GL Version: {}", gl::get_string(GL_VERSION));
		log::info("GL Vendor: {}", gl::get_string(GL_VENDOR));
		log::info("GL Renderer: {}", gl::get_string(GL_RENDERER));
	}

	void update_resolution(const v2<u32>& resolution)
	{
		glViewport(0, 0, static_cast<GLsizei>(resolution.x), static_cast<GLsizei>(resolution.y));
	}

}
