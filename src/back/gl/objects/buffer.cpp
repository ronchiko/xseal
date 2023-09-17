#include "objects/buffer.hpp"

namespace seal::gl {
	namespace {
		void gl_delete_buffer(const GLuint buffer)
		{
			glDeleteBuffers(1, &buffer);
		}
	}

	buffer buffer::create_buffer(type type, usage usage, const size_t size)
	{
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

	buffer::buffer(const GLuint id, const type type, const size_t size)
		: m_BufferId(id, gl_delete_buffer)
		, m_Type(type)
		, m_Size(size)
	{}
}
