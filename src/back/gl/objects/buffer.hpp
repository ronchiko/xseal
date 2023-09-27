#pragma once

#include <span>
#include <type_traits>

#include "seal/types.hpp"

#include "gl.hpp"
#include "gl_id.hpp"
#include "port/buffer_data.hpp"

namespace seal::gl {
	class buffer
	{
	public:
		explicit buffer() = default;

		enum class type
		{
			VertexBuffer = GL_ARRAY_BUFFER,
			IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
			UniformBuffer = GL_UNIFORM_BUFFER,
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
		static buffer create_buffer(type type, usage usage, const std::span<T> buffer);

		/**
		   Creates a GL buffer with an empty buffer.

		   \param type: The type of the buffer.
		   \param usage: The intended usage of the buffer.
		   \param size: The size of the buffer in bytes.
		 */
		static buffer create_buffer(type type, usage usage, size_t size);

		/**
		   Binds this buffer.
		 */
		void bind() const;
		void unbind() const;

		/**
		   Read memory from this buffer.
		 */
		template<typename ElementT>
		std::unique_ptr<ElementT[]> read(size_t offset, size_t size);

		/**
		   Reads the memory into an already existing buffer.

		   \param offset - The offset to read from
		   \param size - The amount of bytes to read
		   \param elements - The buffer to read into.
		 */
		template<typename ElementT>
		void read_into(size_t offset, size_t size, ElementT *elements);

		/**
		   Writes data in the buffer at an offset.
		 */
		template<typename ElementT>
		void write(size_t offset, std::span<ElementT> data);

		/**
		   The size of the buffer in bytes.
		 */
		[[nodiscard]] constexpr size_t size() const
		{
			return m_Size;
		}

		template<typename T>
		[[nodiscard]] constexpr size_t size_in() const
		{
			return m_Size / sizeof(T);
		}

		[[nodiscard]] constexpr u32 id() const noexcept
		{
			return m_BufferId;
		}

	private:
		friend struct fmt::formatter<seal::gl::buffer>;

		buffer(GLuint id, type type, size_t size);

		gl_id m_BufferId;
		type m_Type;
		size_t m_Size;
	};
}

template<typename T>
seal::gl::buffer seal::gl::buffer::create_buffer(type type, usage usage, const std::span<T> buffer)
{
	GLuint buffer_id = 0;

	SEAL_GL_VERIFY(glGenBuffers(1, &buffer_id));
	SEAL_GL_VERIFY(glBindBuffer(static_cast<GLenum>(type), buffer_id));
	SEAL_GL_VERIFY(glBufferData(static_cast<GLenum>(type),
								buffer.size_bytes(),
								reinterpret_cast<const u8 *>(buffer.data()),
								static_cast<GLenum>(usage)));

	return ::seal::gl::buffer{ buffer_id, type, buffer.size_bytes() };
}

template<typename T>
std::unique_ptr<T[]> seal::gl::buffer::read(const size_t offset, const size_t size)
{
	std::unique_ptr<T[]> output{ new T[size] };

	read_into<T>(offset, size, output.get());
	return output;
}

template<typename T>
void seal::gl::buffer::read_into(const size_t offset, const size_t size, T *elements)
{
	bind();

	{
		auto buffer_data = readonly_buffer_data<T>::map(static_cast<GLenum>(m_Type),
														offset * sizeof(T),
														size * sizeof(T));
		std::memcpy(elements, buffer_data.data(), size * sizeof(T));
	}
}

template<typename T>
void seal::gl::buffer::write(const size_t offset, std::span<T> data)
{
	bind();

	{
		auto buffer_data = writable_buffer_data<std::remove_const_t<T>>::map(static_cast<GLenum>(m_Type),
																			 offset * sizeof(T),
																			 data.size_bytes());
		std::memcpy(buffer_data.data(), data.data(), data.size_bytes());
	};
}

template<>
struct fmt::formatter<seal::gl::buffer>
{
	/// Fixed presentation
	char presentation = 'f';

	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
	{
		if(ctx.begin() != ctx.end()) {
			throw format_error("Invalid format!");
		}

		return ctx.end();
	}

	/**
	   Writes the failure to the an iterator.

	   \param buffer: The buffer to write
	   \param ctx: The iterator to write to
	 */
	template<typename FormatContext>
	auto format(const seal::gl::buffer& buffer, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(),
							  "Buffer(Id: {}, Size: {}, Type: {})",
							  static_cast<GLuint>(buffer.m_BufferId),
							  buffer.m_Size,
							  static_cast<seal::u32>(buffer.m_Type));
	}
};
