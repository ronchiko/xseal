#pragma once

#include "objects/buffer.hpp"
#include "tags.h"

namespace seal::gl {
	class uniform_buffer_type : public buffer
	{
	public:
		explicit uniform_buffer_type(usage usage, size_t size);

		[[nodiscard]] constexpr auto bound_index() const noexcept
		{
			return m_BoundIndex;
		}

	private:
		static inline u32 g_LastUsedBufferIndex = 0;

		u32 m_BoundIndex;
	};

}

SEAL_TAG_TYPE(seal::gl::uniform_buffer_type, gl::tags::UniformBuffer);
