#include "pipeline/program.hpp"

namespace seal::gles3 {

	result<program> program::link(std::span<shader> shaders) {
		gl_id program = { glCreateProgram(), glDeleteProgram };

		for(const shader& shader : shaders) {
			glAttachShader(program, shader.id());
		}

		glLinkProgram(program);

		GLint was_successful = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &was_successful);
		if(GL_FALSE == was_successful) {
			GLint log_length = 0, real_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

			std::vector<GLchar> log;
			log.resize(log_length);

			glGetProgramInfoLog(program, log_length, &real_length, log.data());

			seal::log::error("Failed to load link program.");
			if(real_length >= 0) {
				seal::log::error("{}", log.data());
			}

			return seal::failure("Failed to link program");
		}

		return { std::move(program) };
	}

	void program::bind() const {
		glUseProgram(m_Id);
	}
}
