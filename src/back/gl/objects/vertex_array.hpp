#pragma once


#include "gl.hpp"
#include "gl_id.hpp"

namespace seal::gl {

#if defined(SEAL_GL_NO_VAO) || defined(SEAL_GLES_3)
	class vertex_array
	{
	public:
		// In GL ES 2 vertex array are no supported.

		explicit vertex_array() = default;

		/**
		   Creates a new vertex array object.
		 */
		static vertex_array create_vertex_array();

		/**
		   Binds this VAO.
		 */
		void bind() const;

		constexpr GLuint id() const
		{
			return 0;
		}

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
		static vertex_array create_vertex_array();

		/**
		   Binds this VAO.
		 */
		void bind() const;

		/**
		   The id of this vao.

		   \return
		 */
		[[nodiscard]] constexpr GLuint id() const
		{
			return m_Id;
		}

	private:
		explicit vertex_array(GLuint id);

		gl_id m_Id;
	};
#endif
}
