#pragma once

#include <span>

#include "gl.hpp"
#include "gl_id.hpp"

#include "seal/types.hpp"
#include "seal/types/result.hpp"

namespace seal::gl {
	class buffer
	{
	public:
		explicit buffer() = default;

		enum class type
		{
			VertexBuffer = GL_ARRAY_BUFFER,
			IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
		};

		enum class usage
		{
			StaticDraw = GL_STATIC_DRAW,
			DynamicDraw = GL_DYNAMIC_DRAW,
		};

		/**
		   Creates a GL buffer with that is bound to a user buffer.

		   \param type: The type of the buffer.
		   \param usage: The intended usage of the buffer.
		   \param buffer: The buffer to bind to this buffer.
		 */
		template<typename T>
		static result<buffer> create_buffer(type type, usage usage, const std::span<T> buffer);

		/**
		   Creates a GL buffer with an empty buffer.

		   \param type: The type of the buffer.
		   \param usage: The intended usage of the buffer.
		   \param size: The size of the buffer in bytes.
		 */
		static result<buffer> create_buffer(type type, usage usage, size_t size);

		/**
		   Binds this buffer.
		 */
		void bind() const;

	private:
		buffer(GLuint id, type type);

		gl_id m_BufferId;
		type m_Type;
	};
}

template<typename T>
seal::result<seal::gl::buffer> seal::gl::buffer::create_buffer(type type,
	usage usage,
	const std::span<T> buffer)
{
	GLuint buffer_id = 0;

	seal_gl_verify(glGenBuffers(1, &buffer_id));
	seal_gl_verify(glBindBuffer(static_cast<GLenum>(type), buffer_id));
	seal_gl_verify(glBufferData(static_cast<GLenum>(type),
								buffer.size_bytes(),
								reinterpret_cast<const u8 *>(buffer.data()),
								static_cast<GLenum>(usage)));

	return ::seal::gl::buffer{ buffer_id, type };
}