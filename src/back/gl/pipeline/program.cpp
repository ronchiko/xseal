#include "pipeline/program.hpp"

namespace seal::gl {

	result<program> program::link(std::span<shader> shaders)
	{
		gl_id program = { glCreateProgram(), glDeleteProgram };

		for(const shader& shader : shaders) {
			if (!glIsShader(shader.id())) {
				seal::log::error("Attempted to attach a non shader object. id: {}", shader.id());
				continue;
			}

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

	void program::bind() const
	{
		if (!glIsProgram(m_Id)) {
			seal::log::error("Attemped to load a non program id as a program! id: {}", m_Id);
		}

		glUseProgram(m_Id);
	}

	result<void> program::setup_attributes(const api::pipeline_description::type_t type)
	{
		if(type != api::pipeline_description::GraphicsPipeline) {
			// Non-Graphics pipeline are not supported yet.
			return seal::fail<failure::NotImplemented>();
		}

		const u32 vertex_handle = seal_gl_verify(glGetAttribLocation(m_Id,
																	 api::shader::VERTEX_PARAM_NAME));
		const u32 uv_handle = seal_gl_verify(glGetAttribLocation(m_Id, api::shader::UV_PARAM_NAME));
		const u32 tint_handle = seal_gl_verify(glGetAttribLocation(m_Id,
																   api::shader::TINT_PARAM_NAME));

		m_Information.m_Current = shader_information::graphics{
			vertex_handle,
			// uv_handle, TODO: When textures
			tint_handle,
		};

		return {};
	}

	result<void> program::bind_shader_information(GLuint vao)
	{
		return m_Information.bind_to_vao(vao);
	}
}
