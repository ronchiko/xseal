#include "pipeline/vao_context.hpp"

namespace seal::gl {

	vao_context::~vao_context()
	{
		glBindVertexArray(0);
	}


	void vao_context::bind_raw(const u32 attribute_location, const u32 size)
	{
		SEAL_GL_VERIFY(glEnableVertexAttribArray(attribute_location));

		// clang-format off
		SEAL_GL_VERIFY(glVertexAttribPointer(
			attribute_location,
			size,
			GL_FLOAT,
			GL_FALSE,
			static_cast<GLsizei>(m_VertexSize),
			reinterpret_cast<const void *>(static_cast<size_t>(m_Used * sizeof(f32)))));
		// clang-format on
		m_Used += size;
	}
}
