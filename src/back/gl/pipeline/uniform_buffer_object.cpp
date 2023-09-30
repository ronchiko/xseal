#include "pipeline/uniform_buffer_object.hpp"

#include "seal/api/back/uniform_buffer.hpp"
#include "seal/api/tagged_object.hpp"

#include "seal/types/storage.hpp"

namespace seal {
	gl::uniform_buffer_type::uniform_buffer_type(const usage usage, const size_t size)
		: buffer(create_buffer(type::UniformBuffer, usage, size))
		, m_BoundIndex(g_LastUsedBufferIndex++)
	{
		// Make sure the bind the buffer to an index.
		log::info("Binding uniform buffer to index: {}", m_BoundIndex);
		SEAL_GL_VERIFY(glBindBufferBase(GL_UNIFORM_BUFFER, m_BoundIndex, id()));
	}

	inline storage<gl::uniform_buffer_type> g_UniformBuffers;

	api::abstract_t api::allocate_uniform_buffer(const size_t requested_size)
	{
		auto& buffer = g_UniformBuffers.emplace(gl::buffer::usage::DynamicDraw, requested_size);
		return abstract_t::bind(&buffer);
	}

	void api::free_uniform_buffer(abstract_t uniform_buffer)
	{
		g_UniformBuffers.erase(*uniform_buffer.acquire<gl::uniform_buffer_type>());
	}

	void api::update_uniform_buffer(abstract_t uniform_buffer, const void *data)
	{
		auto *buffer = uniform_buffer.acquire<gl::uniform_buffer_type>();

		buffer->bind();
		buffer->write(0, std::span{ static_cast<const char *>(data), buffer->size() });
	}

}
