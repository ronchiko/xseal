#include "seal/engine/render/pipeline/uniform_buffer.hpp"

namespace seal {
	uniform_buffer::uniform_buffer(api::abstract_t uniform_buffer)
		: m_UniformBuffer(std::move(uniform_buffer))
	{}
}
