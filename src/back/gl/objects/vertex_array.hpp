#pragma once

#include "gl.hpp"
#include "gl_id.hpp"

#include "seal/types/result.hpp"

namespace seal::gl {

#if defined(SEAL_GL_NO_VAO) || defined(SEAL_GLES_2)
	class vertex_array
	{
	public:
		// In GLES 2 vertex array are no supported.

		explicit vertex_array() = default;

		/**
		   Creates a new vertex array object.
		 */
		static result<vertex_array> create_vertex_array();

		/**
		   Binds this VAO.
		 */
		void bind() const;

	private:
	};

#else
	class vertex_array
	{
	public:
		explicit vertex_array() = default;

		/**
		   Creates a new vertex array object.
		 */
		static result<vertex_array> create_vertex_array();

		/**
		   Binds this VAO.
		 */
		void bind() const;

	private:
		vertex_array(GLuint id);

		gl_id m_Id;
	};
#endif
}
