#pragma once

#include "seal/defenitions.h"
#include "seal/types/result.hpp"

#include "gl_id.hpp"
#include "tags.h"
#include "pipeline/shader.hpp"

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
		   Binds the program to the currently active one.
		 */
		void bind() const;

		constexpr id id() const
		{
			return static_cast<seal::id>(m_Id);
		}

	private:
		program(gl_id id) noexcept
			: m_Id(std::move(id))
		{}

		gl_id m_Id;
	};
}

seal_tag_type(seal::gl::program, seal::gl::tags::Program);