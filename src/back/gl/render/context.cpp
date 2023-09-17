#include "seal/api/back/backend.hpp"

#include "gl.hpp"

namespace seal::api {
	void start_context(const color& flush_color) {
		glClearColor(flush_color.r, flush_color.g, flush_color.b, flush_color.a);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void end_context() {
		// For OpenGL do nothing
	}
}
