#pragma once

#include "seal/defenitions.h"
#include "seal/types/result.hpp"
#include "seal/api/back/pipeline.hpp"

#include "gl_id.hpp"
#include "tags.h"
#include "pipeline/shader.hpp"
#include "port/shader_information.hpp"

namespace seal::gl {
	class program
	{
	public:
		/**
		   Creates a program from a set of shaders.

		   \param shaders: The shader to make the program from.
		 */
		program(nullptr_t)
			: program(gl_id())
		{}

		program()
			: program(nullptr)
		{}

		static result<program> link(std::span<shader> shaders);

		/**
		   Sets up the attributes for this program.
		  
		   \param type
		 */
		result<void> setup_attributes(api::pipeline_description::type_t type);

		/**
		   Binds the program to the currently active one.
		 */
		void bind() const;

		constexpr id id() const
		{
			return static_cast<seal::id>(m_Id);
		}

		/**
		   The shader information held by this program.

		   @param vao: The VAO to bind the information to.
		 */
		result<void> bind_shader_information(GLuint vao);

	private:
		program(gl_id id) noexcept
			: m_Id(std::move(id))
			, m_Information()
		{}

		gl_id m_Id;
		shader_information m_Information;
	};
}

seal_tag_type(seal::gl::program, seal::gl::tags::Program);