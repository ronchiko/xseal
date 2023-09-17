#include "pipeline/vao_context.hpp"

namespace seal::gl {

	void vao_context::bind_raw(const u32 attribute_location, const u32 size)
	{
#if defined(SEAL_GL_NO_VAO)
		seal_gl_verify(glEnableVertexAttribArray(attribute_location));
#else
		seal_gl_verify(glEnableVertexArrayAttrib(m_Vao, attribute_location));
#endif
		// clang-format off
		seal_gl_verify(glVertexAttribPointer(
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
