#include "objects/buffer.hpp"

namespace seal::gl {
	static void _glDeleteBuffer(GLuint buffer)
	{
		glDeleteBuffers(1, &buffer);
	}

	result<buffer> buffer::create_buffer(type type, usage usage, size_t size) {
		GLuint buffer_id = 0;

		seal_gl_verify(glGenBuffers(1, &buffer_id));
		seal_gl_verify(glBindBuffer(static_cast<GLenum>(type), buffer_id));
		seal_gl_verify(glBufferData(static_cast<GLenum>(type),
									size,
									nullptr,
									static_cast<GLenum>(usage)));

		GLint value = INT_MAX;
		glGetBufferParameteriv(static_cast<GLenum>(type), GL_BUFFER_SIZE, &value);

		return ::seal::gl::buffer{ buffer_id, type, size };
	}

	void buffer::bind() const
	{
		glBindBuffer(static_cast<GLenum>(m_Type), m_BufferId);
	}

	buffer::buffer(GLuint id, type type, size_t size) 
		: m_BufferId(id, _glDeleteBuffer)
		, m_Type(type)
		, m_Size(size)
	{}
}
