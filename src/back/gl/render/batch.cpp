#include "render/batch.hpp"

#include "seal/types/storage.hpp"

#include "pipeline/program.hpp"

inline static seal::storage<seal::gl::batch> m_Batches;

namespace seal {
	namespace gl {
		/**
		   Creates a controlled buffer.

		   @tparam Type: The type of the buffer to create
		   @tparam ModeHint: The bit to check to decide if our buffer is dynamic or static.
		   @tparam BufferType: The type held inside the buffer.

		   @param hints: The hints passed down from the caller.
		   @param size: The amount of elements in the buffer (in BufferType's).
		 */
		template<buffer::type Type, api::batch_hint ModeHint, typename BufferType>
		result<controlled_buffer<BufferType>> create_buffer(flags<api::batch_hint> hints,
															 size_t size)
		{
			using buffer::usage::DynamicDraw;
			using buffer::usage::StaticDraw;

			const auto buffer_mode = hints.is_active(ModeHint) ? DynamicDraw : StaticDraw;
			auto new_buffer = buffer::create_buffer(Type, buffer_mode, size * sizeof(BufferType));
			seal_verify_result(new_buffer);

			return controlled_buffer<BufferType>(std::move(*new_buffer));
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

		result<api::buffer> batch::lock(const api::batch_buffer_type type, const bool read)
		{
			switch(type) {
			case seal::api::batch_buffer_type::VertexBuffer:
			{
				seal_verify_result(m_Vbo.load());
				if (read) {
					seal_verify_result(m_Vbo.read());
				}

				return seal::api::buffer::bind(m_Vbo.view());
			}

			case seal::api::batch_buffer_type::IndexBuffer:
			{
				seal_verify_result(m_Ibo.load());
				if(read) {
					seal_verify_result(m_Ibo.read());
				}

				return seal::api::buffer::bind(m_Ibo.view());
			}

			default:
				return seal::failure("Cant lock unsupported batch buffer type.");
			}
		}

		result<void> batch::unlock(const api::batch_buffer_type type, const bool write)
		{
			switch(type) {
			case seal::api::batch_buffer_type::VertexBuffer:
				if (write) {
					m_Vbo.write();
				}

				// If we are hinted the buffer wont be changed, we should free it.
				if(!m_Hints.is_active(api::batch_hint::KeepVertecies)) {
					m_Vbo.unload();
				}
				return {};

			case seal::api::batch_buffer_type::IndexBuffer:
				if(write) {
					m_Ibo.write();
				}

				// If we shouldn't keep the indecies we can free them now.
				if(!m_Hints.is_active(api::batch_hint::KeepIndecies)) {
					m_Ibo.unload();
				}
				return {};

			default:
				return seal::failure("Cant lock unsupported batch buffer type.");
			}
		}

		result<void> batch::publish(size_t vertecies) {

			m_Vao.bind();

			m_Vbo->bind();
			m_Ibo->bind();

			m_Vbo.flush();
			m_Ibo.flush();

			seal_gl_verify(glDrawElements(GL_TRIANGLES, vertecies, GL_UNSIGNED_INT, nullptr));
			return {};
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

	result<api::buffer> api::lock_batch_buffer(api::abstract_t batch, api::batch_buffer_type type, bool read)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->lock(type, read);
	}

	result<void> api::unlock_batch_buffer(api::abstract_t batch, api::batch_buffer_type type, bool write)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->unlock(type, write);
	}

	result<void> api::publish_batch(api::abstract_t batch, size_t vertecies) {
		auto *batch_obj = batch.acquire<gl::batch>();
		
		return batch_obj->publish(vertecies);
	}

	result<void> api::bind_batcher_to_pipeline(api::abstract_t batch, api::abstract_t pipeline) {
		auto *pipeline_obj = pipeline.acquire<gl::program>();
		auto *batch_obj = batch.acquire<gl::batch>();

		pipeline_obj->bind();

		pipeline_obj->bind_shader_information(batch_obj->vao().id());
		return {};
	}
}
