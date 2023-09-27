#include "render/batch.hpp"

#include "seal/types/storage.hpp"

#include "pipeline/program.hpp"


namespace seal {

	namespace {
		inline storage<gl::batch> g_Batches;
	}


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
		controlled_buffer<BufferType> create_buffer(const flags<api::batch_hint> hints,
													const size_t size)
		{
			using buffer::usage::DynamicDraw;
			using buffer::usage::StaticDraw;

			const auto buffer_mode = hints.is_active(ModeHint) ? DynamicDraw : StaticDraw;
			auto new_buffer = buffer::create_buffer(Type, buffer_mode, size * sizeof(BufferType));

			return controlled_buffer<BufferType>(std::forward<buffer>(new_buffer));
		}

		batch batch::create_batch(const api::batch_initialization& initialize)
		{
			// Create the vertex array
			auto vertex_array = vertex_array::create_vertex_array();

			// Create the vertex buffer.
			auto vertex_buffer = create_buffer<buffer::type::VertexBuffer,
											   api::batch_hint::KeepVertices,
											   api::vertex>(initialize.hints, initialize.vertices);

			// Create the index buffer
			auto index_buffer = create_buffer<buffer::type::IndexBuffer,
											  api::batch_hint::KeepIndices,
											  u32>(initialize.hints, initialize.indices);

			return batch{
				std::move(vertex_array),
				initialize.hints,
				std::move(vertex_buffer),
				std::move(index_buffer),
			};
		}

		api::buffer batch::lock(const api::batch_buffer_type type, const bool read)
		{
			switch(type) {
			case seal::api::batch_buffer_type::VertexBuffer:
			{
				m_Vbo.load();
				if(read) {
					m_Vbo.read();
				}

				return api::buffer::bind(m_Vbo.view());
			}

			case api::batch_buffer_type::IndexBuffer:
			{
				m_Ibo.load();
				if(read) {
					m_Ibo.read();
				}

				return api::buffer::bind(m_Ibo.view());
			}

			default:
				throw seal::failure("Cant lock unsupported batch buffer type.");
			}
		}

		void batch::unlock(const api::batch_buffer_type type, const bool write)
		{
			switch(type) {
			case api::batch_buffer_type::VertexBuffer:
				if(write) {
					m_Vbo.write();
				}

				// If we are hinted the buffer wont be changed, we should free it.
				if(!m_Hints.is_active(api::batch_hint::KeepVertices)) {
					m_Vbo.unload();
				}
				return;

			case api::batch_buffer_type::IndexBuffer:
				if(write) {
					m_Ibo.write();
				}

				// If we shouldn't keep the indices we can free them now.
				if(!m_Hints.is_active(api::batch_hint::KeepIndices)) {
					m_Ibo.unload();
				}
				return;

			default:
				throw failure("Cant lock unsupported batch buffer type.");
			}
		}

		void batch::publish(const size_t vertices)
		{
			m_Vao.bind();

			m_Vbo->bind();
			m_Ibo->bind();

			m_Vbo.flush();
			m_Ibo.flush();

			SEAL_GL_VERIFY(glDrawElements(GL_TRIANGLES,
										  static_cast<GLsizei>(vertices),
										  GL_UNSIGNED_INT,
										  nullptr));
		}
	}

	api::abstract_t api::create_batch(const api::batch_initialization& initialize)
	{
		auto batch = gl::batch::create_batch(initialize);

		auto& allocated_batch = g_Batches.store(std::forward<gl::batch>(batch));
		return abstract_t::bind(&allocated_batch);
	}

	void api::free_batch(api::abstract_t batch)
	{
		auto *batch_obj = batch.acquire<gl::batch>();
		g_Batches.erase(*batch_obj);
	}

	api::buffer api::lock_batch_buffer(api::abstract_t batch, const api::batch_buffer_type type, const bool read)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->lock(type, read);
	}

	void api::unlock_batch_buffer(api::abstract_t batch,
								  const batch_buffer_type type,
								  const bool write)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->unlock(type, write);
	}

	void api::publish_batch(api::abstract_t batch, const size_t vertices)
	{
		auto *batch_obj = batch.acquire<gl::batch>();

		return batch_obj->publish(vertices);
	}

	void api::bind_batch_to_pipeline(api::abstract_t batch, api::abstract_t pipeline)
	{
		auto *pipeline_obj = pipeline.acquire<gl::program>();
		const auto *batch_obj = batch.acquire<gl::batch>();

		pipeline_obj->bind();

		pipeline_obj->bind_shader_information(batch_obj->vao().id());
	}
}
