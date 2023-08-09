#include "pipeline/vao_context.hpp"

namespace seal::gl {

	result<void> vao_context::bind_raw(u32 location, u32 size)
	{
#if defined(SEAL_GL_NO_VAO)
		seal_gl_verify(glEnableVertexAttribArray(location));
#else
		seal_gl_verify(glEnableVertexArrayAttrib(m_Vao, location));
#endif
		// clang-format off
		seal_gl_verify(glVertexAttribPointer(
			location,
			size,
			GL_FLOAT,
			GL_FALSE,
			m_VertexSize,
			reinterpret_cast<const void *>(static_cast<size_t>(m_Used * sizeof(f32)))));
		// clang-format on
		m_Used += size;
		return {};
	}
}
