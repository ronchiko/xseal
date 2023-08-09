#pragma once

#include <vector>

#include "seal/api/back/vertex.hpp"

#include "objects/buffer.hpp"
#include "objects/controlled_buffer.hpp"
#include "objects/vertex_array.hpp"
#include "tags.h"

namespace seal::gl {
	class batch
	{
	public:
		batch() = default;

		static result<batch> create_batch(const seal::api::batch_initialization& initialize);

		result<seal::api::buffer> lock(seal::api::batch_buffer_type, bool read);

		result<void> unlock(seal::api::batch_buffer_type, bool write);

		result<void> publish(size_t vertecies);

		const vertex_array& vao() const
		{
			return m_Vao;
		}

	private:
		explicit batch(vertex_array vao,
					   flags<api::batch_hint> hint,
					   controlled_buffer<api::vertex> vbo,
					   controlled_buffer<u32> ibo) 
			: m_Vao(std::move(vao))
			, m_Hints(hint)
			, m_Vbo(std::move(vbo))
			, m_Ibo(std::move(ibo))
		{}

		vertex_array m_Vao;
		flags<api::batch_hint> m_Hints;

		controlled_buffer<api::vertex> m_Vbo;
		controlled_buffer<u32> m_Ibo;
	};

}

seal_tag_type(seal::gl::batch, seal::gl::tags::Batch);