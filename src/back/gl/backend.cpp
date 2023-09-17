#include "seal/api/back/backend.hpp"

#include "seal/panic.h"

#include "gl.hpp"

namespace seal::api {
	void initialize_backend()
	{
#ifdef SEAL_WEBGL
		if (0 == gladLoadGLES2(glfwGetProcAddress)) {
			throw seal::failure("Failed to load OpenGL");
		}
#else
		if(0 == gladLoadGL()) {
			throw seal::failure("Failed to load OpenGL");
		}
#endif

		// Load some general information variables
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gl::specs::max_textures_per_fragment);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &gl::specs::max_textures_combined);

		seal::log::info("GL Version: {}", gl::get_string(GL_VERSION));
		seal::log::info("GL Vendor: {}", gl::get_string(GL_VENDOR));
		seal::log::info("GL Renderer: {}", gl::get_string(GL_RENDERER));

	}

	void update_resolution(const v2<u32>& resolution) {
		glViewport(0, 0, resolution.x, resolution.y);
	}

}
