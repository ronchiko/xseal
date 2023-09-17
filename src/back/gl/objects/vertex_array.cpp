#include "vertex_array.hpp"

namespace seal::gl {

#if defined(SEAL_GLES_3) || defined(SEAL_GL_NO_VAO)

	vertex_array vertex_array::create_vertex_array() {
		return vertex_array{};
	}

	void vertex_array::bind() const {}

#else
	namespace 
	{
		void gl_delete_vertex_array(const GLuint id)
		{
			glDeleteVertexArrays(1, &id);
		}
	}

	vertex_array vertex_array::create_vertex_array()
	{
		GLuint id;

		seal_gl_verify(glCreateVertexArrays(1, &id));
		seal_gl_verify(glBindVertexArray(id));

		return vertex_array{ id };
	}

	void vertex_array::bind() const
	{
		glBindVertexArray(m_Id);
	}

	vertex_array::vertex_array(GLuint id)
		: m_Id({ id, gl_delete_vertex_array })
	{}
#endif
}
