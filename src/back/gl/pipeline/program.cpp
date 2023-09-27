#include "pipeline/program.hpp"

#include "texture/texture.hpp"
#include "pipeline/uniform_buffer_object.hpp"

namespace seal::gl {

	static constexpr u32 INVALID_UNIFORM_LOCATION = static_cast<u32>(-1);

	/**
	   Given a collection of shaders links them into a program.

	   \param shaders: The shaders to merge
	 */
	namespace {
		gl_id link_program(const std::span<shader> shaders)
		{
			gl_id program = { glCreateProgram(), glDeleteProgram };

			for(const shader& shader : shaders) {
				if(!glIsShader(shader.id())) {
					seal::log::error("Attempted to attach a non shader. skipping id: {}.",
									 shader.id());
					continue;
				}

				SEAL_GL_VERIFY(glAttachShader(program, shader.id()));
			}

			SEAL_GL_VERIFY(glLinkProgram(program));

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

				throw seal::failure("Failed to link program: {}", log.data());
			}

			return program;
		}
	}

	program program::link(std::span<shader> shaders,
						  const api::pipeline_stage_creation_options& description)
	{
		auto program_id = link_program(shaders);

		program uninitialized_program(std::move(program_id));
		uninitialized_program.bind_attributes(description.type);
		uninitialized_program.bind_uniforms(description.uniforms);

		return uninitialized_program;
	}

	void program::bind() const
	{
		if(!glIsProgram(m_Id)) {
			seal::log::error("Attempted to load a non program id as a program! id: {}", m_Id);
		}

		glUseProgram(m_Id);
	}

	void program::bind_shader_information(const GLuint vao)
	{
		m_Information.bind_to_vao(vao);
	}

	api::uniform_query_information program::query(const std::string& uniform_name) const
	{
		const auto result = std::ranges::find_if(m_Uniforms, [&uniform_name](const auto& val) {
			return val.is_ok() && val.name == uniform_name;
		});

		if(m_Uniforms.end() == result) {
			throw failure("No uniform named {}", uniform_name);
		}

		return api::uniform_query_information{
			static_cast<u32>(std::distance(m_Uniforms.begin(), result)), result->kind
		};
	}

	void program::set_uniform(api::uniform_update_options update) const
	{
		bind();

		seal_debug_only({
			// Give warning in debug mode.
			if(!m_Uniforms[update.location].is_ok()) {
				seal::log::warning("Invalid uniform at {}", update.location);
			}
		});

		// Match the uniform kind.
		switch(m_Uniforms[update.location].kind) {
		case api::uniform_kind::Texture2d:
		{
			const auto& texture = std::get<u32>(update.updated_value);
			SEAL_GL_VERIFY(glUniform1i(update.location, texture));
			return;
		}
		case api::uniform_kind::Buffer:

			return;
		}
		throw seal::failure("Invalid uniform kind!");
	}

	void program::bind_attributes(const api::pipeline_stage_type type)
	{
		if(type != api::pipeline_stage_type::GraphicsPipeline) {
			// Non-Graphics pipeline are not supported yet.
			throw seal::fail<failure::NotImplemented>();
		}

		const u32 vertex_handle = SEAL_GL_VERIFY(glGetAttribLocation(m_Id,
																	 api::shader::VERTEX_PARAM_NAME));
		const u32 uv_handle = SEAL_GL_VERIFY(glGetAttribLocation(m_Id, api::shader::UV_PARAM_NAME));
		const u32 tint_handle = SEAL_GL_VERIFY(glGetAttribLocation(m_Id,
																   api::shader::TINT_PARAM_NAME));

		m_Information.current = shader_information::graphics{
			vertex_handle,
			uv_handle,
			tint_handle,
		};
	}

	void program::bind_uniforms(const std::vector<api::uniform_declaration_options>& uniforms)
	{
		bind();

		for(const auto& uniform : uniforms) {
			// We need to treat buffer object differently because they use different API to be bound.
			if(uniform.kind == api::uniform_kind::Buffer) {
				const auto block_index = SEAL_GL_VERIFY(glGetUniformBlockIndex(m_Id,
																			   uniform.uniform_name.c_str()));
				if(GL_INVALID_INDEX == block_index) {
					log::error("Failed to find block index for: '{}'", uniform.uniform_name);
					throw gl::fail();
				}

				const auto& abstracted_ubo = std::get<api::abstract_t>(uniform.default_value);
				const auto* ubo = abstracted_ubo.acquire<gl::uniform_buffer_type>();

				SEAL_GL_VERIFY(glUniformBlockBinding(m_Id,
													 block_index,
													 ubo->bound_index()));
				continue;
			}

			const u32 location = glGetUniformLocation(m_Id, uniform.uniform_name.c_str());
			if(location == INVALID_UNIFORM_LOCATION) {
				seal::log::error("GL: Failed to bind named uniform: {}", uniform.uniform_name);
				throw seal::gl::fail();
			}

			if(m_Uniforms.size() <= location) {
				m_Uniforms.resize(location + 1);
			}

			m_Uniforms[location] = internal_uniform_info{ uniform.uniform_name, uniform.kind };
			set_uniform(api::uniform_update_options{ location, uniform.default_value });
		}
	}

	api::uniform_kind program::query_uniform_kind_by_location(const u32 location) const
	{
		const auto& uniform = m_Uniforms[location];
		if(!uniform.is_ok()) {
			throw seal::failure("No uniform at location {}", location);
		}

		return uniform.kind;
	}

}
