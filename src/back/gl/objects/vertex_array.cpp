#include "vertex_array.hpp"

namespace seal::gl {

#if defined(SEAL_GLES_2) || defined(SEAL_GL_NO_VAO)

	result<vertex_array> vertex_array::create_vertex_array() {
		return vertex_array{};
	}

	void vertex_array::bind() const {}

#else
	static void _glDeleteVertexArray(GLuint id)
	{
		glDeleteVertexArrays(1, &id);
	}

	result<vertex_array> vertex_array::create_vertex_array()
	{
		GLuint id;

		seal_gl_verify(glCreateVertexArrays(1, &id));
		seal_gl_verify(glBindVertexArray(id));

		return { id };
	}

	void vertex_array::bind() const
	{
		glBindVertexArray(m_Id);
	}

	vertex_array::vertex_array(GLuint id)
		: m_Id({ id, _glDeleteVertexArray })
	{}
#endif
}
