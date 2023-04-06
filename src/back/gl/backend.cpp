#include "seal/api/back/backend.hpp"

#include "seal/panic.h"

#include "gl.hpp"

namespace seal::api {
	result<void> initialize_backend()
	{
#ifdef SEAL_WEBGL
		if (0 == gladLoadGLES2(glfwGetProcAddress)) {
			return seal::failure("Failed to load OpenGL");
		}
#else
		if(0 == gladLoadGL()) {
			return seal::failure("Failed to load OpenGL");
		}
#endif

		return {};
	}

}
