#include "render/batch.hpp"

#include "seal/types/storage.hpp"

inline static seal::storage<seal::gl::batch> m_Batches;

namespace seal {
	namespace gl {
		/**
		   Creates a controlled buffer.
		 */
		template<buffer::type Type, api::batch_hint ModeHint, typename BufferTypeT>
		result<controlled_buffer<BufferTypeT>> create_buffer(flags<api::batch_hint> hints,
															 size_t size)
		{
			using buffer::usage::DynamicDraw;
			using buffer::usage::StaticDraw;

			const auto buffer_mode = hints.is_active(ModeHint) ? DynamicDraw : StaticDraw;
			auto new_buffer = buffer::create_buffer(Type, buffer_mode, size * sizeof(BufferTypeT));
			seal_verify_result(new_buffer);

			return controlled_buffer<BufferTypeT>(std::move(*new_buffer));
		}

		result<batch> batch::create_batch(const api::batch_initialization& initialize)
		{
			// Create the vertex array
			auto vertex_array = vertex_array::create_vertex_array();
			seal_verify_result(vertex_array);

			// Create the vertex buffer.
			auto vertex_buffer = create_buffer<buffer::type::VertexBuffer,
											   api::batch_hint::KeepVertecies,
											   api::vertex>(initialize.hints, initialize.vertecies);
			seal_verify_result(vertex_buffer);

			// Create the index buffer
			auto index_buffer = create_buffer<buffer::type::IndexBuffer,
											  api::batch_hint::KeepIndecies,
											  u32>(initialize.hints, initialize.indecies);
			seal_verify_result(index_buffer);

			return batch{
				std::move(*vertex_array),
				initialize.hints,
				std::move(*vertex_buffer),
				std::move(*index_buffer),
			};
		}

		result<api::buffer> batch::lock(const api::batch_buffer_type type)
		{
			switch(type) {
			case seal::api::batch_buffer_type::VertexBuffer:
			{
				auto view = m_Vbo.load();
				seal_verify_result(view);
				return seal::api::buffer::bind(*view);
			}

			case seal::api::batch_buffer_type::IndexBuffer:
			{
				auto view = m_Ibo.load();
				seal_verify_result(view);
				return seal::api::buffer::bind(*view);
			}

			default:
				return seal::failure("Cant lock unsupported batch buffer type.");
			}
		}

		result<void> batch::unlock(const api::batch_buffer_type type)
		{
			switch(type) {
			case seal::api::batch_buffer_type::VertexBuffer:
				// If we are hinted the buffer wont be changed, we should free it.
				if(!m_Hints.is_active(api::batch_hint::KeepVertecies)) {
					m_Vbo.unload();
				}
				return {};

			case seal::api::batch_buffer_type::IndexBuffer:
				// If we shouldn't keep the indecies we can free them now.
				if(!m_Hints.is_active(api::batch_hint::KeepIndecies)) {
					m_Ibo.unload();
				}
				return {};

			default:
				return seal::failure("Cant lock unsupported batch buffer type.");
			}
		}
	}

	result<api::abstract_t> api::create_batch(const api::batch_initialization& init)
	{
		auto batch = gl::batch::create_batch(init);
		seal_verify_result(batch);

		return api::abstract_t::bind(m_Batches.store(std::move(*batch)));
	}

	void api::free_batch(api::abstract_t batch)
	{
		auto *batch_obj = batch.acquire<gl::batch>();
		m_Batches.erase(*batch_obj);
	}

	result<api::buffer> api::lock_batch_buffer(api::abstract_t batch, api::batch_buffer_type type)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->lock(type);
	}

	result<void> api::unlock_batch_buffer(api::abstract_t batch, api::batch_buffer_type type)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->unlock(type);
	}
}
