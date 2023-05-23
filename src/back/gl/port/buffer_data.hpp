#pragma once

#include "seal/types/result.hpp"

#include "gl.hpp"

namespace seal::gl {

	namespace detail {
		template<GLbitfield Flags>
		struct buffer_mapper
		{
			result<void *> operator()(GLenum type, size_t start, size_t offset)
			{
				void *mapped_address = seal_gl_verify(glMapBufferRange(type, start, offset, Flags));
				return mapped_address;
			}
		};

		struct buffer_unmapper
		{
			result<void> operator()(void *, GLenum type) const
			{
				seal_gl_verify(glUnmapBuffer(type));
				return {};
			}
		};
	}

	/**
	   Basic management type of a buffer.
	 */
	template<typename ElementT, typename MapperT, typename UnmapperT>
	struct basic_buffer_data
	{
	public:
		static_assert(std::is_trivially_constructible_v<MapperT>,
					  "MapperT must be trivailly constructible");

		static_assert(std::is_trivially_constructible_v<UnmapperT>,
					  "UnmapperT must be trivailly constructible");

		static result<basic_buffer_data> map(GLenum type, size_t start, size_t size)
		{
			MapperT mapper;

			auto address = mapper(type, start, size);
			seal_verify_result(address);

			return basic_buffer_data{ type, reinterpret_cast<ElementT *>(*address) };
		}

		~basic_buffer_data() noexcept
		{
			if(nullptr == m_Data) {
				return;
			}

			seal_mute_exceptions({
				UnmapperT unmapper;
				if(!unmapper(m_Data, m_Type)) {
					seal::log::warning("Failed to unmap buffer");
				}
			});
		}

		basic_buffer_data(basic_buffer_data&& other) noexcept
			: m_Type(other.m_Type)
			, m_Data(other.m_Data)
		{
			other.m_Data = nullptr;
			other.m_Type = static_cast<GLenum>(0);
		}

		basic_buffer_data& operator=(basic_buffer_data&& other) noexcept
		{
			std::exchange(m_Type, other.m_Type);
			std::exchange(m_Data, other.m_Data);

			return *this;
		}

		basic_buffer_data(const basic_buffer_data& other) = delete;
		basic_buffer_data& operator=(const basic_buffer_data& other) = delete;

		constexpr ElementT *data()
		{
			return m_Data;
		}

	private:
		basic_buffer_data(GLenum type, ElementT *data)
			: m_Type(type)
			, m_Data(data)
		{}

		GLenum m_Type;
		ElementT *m_Data;
	};

	// A buffer data can only be read from
	template<typename ElementT>
	using readonly_buffer_data = basic_buffer_data<ElementT,
												   detail::buffer_mapper<GL_MAP_READ_BIT>,
												   detail::buffer_unmapper>;

	// A buffer data that can only be written to.
	template<typename ElementT>
	using writable_buffer_data = basic_buffer_data<
		ElementT,
		detail::buffer_mapper<GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT>,
		detail::buffer_unmapper>;
}
