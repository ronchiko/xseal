#include "pipeline/shader.hpp"

#include <sstream>

#include "gl.hpp"
#include "gl_id.hpp"

namespace {
	constexpr GLint UNSUPPORTED_SHADER_TYPE = -1;
}

#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER UNSUPPORTED_SHADER_TYPE
#endif

namespace seal::gl {
	constexpr static GLint GL_SHADER_MAPPING[] = {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_COMPUTE_SHADER,
	};

	shader shader::from_resource(const resource& resource, const type type)
	{
		const auto shader_type = GL_SHADER_MAPPING[static_cast<u32>(type)];
		if(UNSUPPORTED_SHADER_TYPE == shader_type)
		{
			throw failure("Shader type not supported!");
		}

		gl_id shader = { glCreateShader(shader_type), glDeleteShader };

		// Load the resource
		const auto shader_source = resource->load_all();

		const auto* source_ptr = reinterpret_cast<const GLchar *>(shader_source.data());
		SEAL_GL_VERIFY(glShaderSource(shader, 1, &source_ptr, nullptr));
		SEAL_GL_VERIFY(glCompileShader(shader));

		// Handle errors in compilation
		GLint was_successful = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &was_successful);
		if(GL_FALSE == was_successful) {
			GLint log_length = 0, real_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			std::vector<GLchar> log;
			log.resize(log_length);

			glGetShaderInfoLog(shader, log_length, &real_length, log.data());

			seal::log::error("Failed to load shader from resource: {}", resource->path());
			if(real_length >= 0) {
				seal::log::error("{}", log.data());
			}

			throw failure("Failed to compile shader");
		}

		return gl::shader(std::move(shader));
	}
}
