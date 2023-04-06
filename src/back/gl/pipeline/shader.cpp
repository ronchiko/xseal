#include "pipeline/shader.hpp"

#include <sstream>

#include "gl.hpp"
#include "gl_id.hpp"

namespace seal::gles3 {
	constexpr static GLint GL_SHADER_MAPPING[] = {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_COMPUTE_SHADER,
	};

	result<shader> shader::from_resource(resource resource, type type)
	{
		gl_id shader = { glCreateShader(GL_SHADER_MAPPING[static_cast<u32>(type)]), glDeleteShader };

		// Load the resource
		auto shader_source = resource->load_all();
		seal_verify_result(shader_source);

		const GLchar *source_ptr = reinterpret_cast<const GLchar *>(shader_source->data());
		glShaderSource(shader, 1, &source_ptr, nullptr);
		glCompileShader(shader);

		// Handl errors in compeletion
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

			return seal::failure("Failed to compile shader");
		}

		return { std::move(shader) };
	}
}
