#pragma once

#include <map>

#include "seal/api/back/pipeline.hpp"
#include "seal/defenitions.h"
#include "seal/types/dynamic_struct.hpp"

#include "gl_id.hpp"
#include "pipeline/shader.hpp"
#include "port/shader_information.hpp"
#include "tags.h"

namespace seal::gl {
	class program
	{
	public:
		program(nullptr_t)
			: program(gl_id())
		{}

		program()
			: program(nullptr)
		{}

		/**
		   Links a couple of shaders into a program and initializes its arrays.

		   \param shaders: The shaders to initialize.
		   \param description: The description of the program.
		 */
		static program link(std::span<shader> shaders,
							const api::pipeline_stage_creation_options& description);

		/**
		   Binds the program to the currently active one.
		 */
		void bind() const;

		[[nodiscard]] constexpr id id() const
		{
			return static_cast<seal::id>(m_Id);
		}

		/**
		   The shader information held by this program.

		   \param vao: The VAO to bind the information to.
		 */
		void bind_shader_information(GLuint vao);

		/**
			Queries information about a uniform inside the program.

			\param uniform_name: The name of the uniform.
		 */
		[[nodiscard]]
		api::uniform_query_information query(const std::string& uniform_name) const;

		/**
			Updates the value inside a uniform.

			\param update: The update to do.
		 */
		void set_uniform(api::uniform_update_options update) const;

	private:
		struct internal_uniform_info
		{
			std::string name;
			api::uniform_kind kind;

			[[nodiscard]]
			constexpr bool is_ok() const
			{
				return !name.empty();
			}
		};

		/**
		   Binds the attributes of the shaders.
		 */
		void bind_attributes(api::pipeline_stage_type type);

		/**
		   Binds the uniforms passed down to this program.
		 */
		void bind_uniforms(const std::vector<api::uniform_declaration_options>& uniforms);

		api::uniform_kind query_uniform_kind_by_location(u32 location) const;

		explicit program(gl_id id)
			: m_Id(std::move(id))
			, m_Information()
			, m_Uniforms{}
		{}

		gl_id m_Id;
		shader_information m_Information;
		std::vector<internal_uniform_info> m_Uniforms;
	};
}

SEAL_TAG_TYPE(seal::gl::program, seal::gl::tags::Program);