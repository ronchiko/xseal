#include "vertex_array.hpp"

namespace seal::gl {

	namespace 
	{
		void gl_delete_vertex_array(const GLuint id)
		{
			glDeleteVertexArrays(1, &id);
		}
	}

	vertex_array vertex_array::create_vertex_array()
	{
		GLuint id = 0;

		SEAL_GL_VERIFY(glGenVertexArrays(1, &id));
		SEAL_GL_VERIFY(glBindVertexArray(id));

		return vertex_array{ id };
	}

	void vertex_array::bind() const
	{
		glBindVertexArray(m_Id);
	}

	vertex_array::vertex_array(GLuint id)
		: m_Id({ id, gl_delete_vertex_array })
	{}
}
