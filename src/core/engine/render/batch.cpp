#include "seal/engine/render/batch.hpp"

#include <algorithm>

#include "seal/types/finally.hpp"

namespace seal {

	batch::~batch()
	{
		if(!seal_is_error(m_Batch)) {
			api::free_batch(m_Batch);
		}
	}

	batch batch::create_triangular(const size_t vertex_count, const bool streamable /* = false */)
	{
		using api::batch_buffer_type::IndexBuffer;

		seal_wassert(0 == vertex_count % 4, "The vertex count is not divisable by 4");

		const auto indices_count = (vertex_count >> 2) * 6;
		auto batch_object = api::create_batch(api::batch_initialization{
			vertex_count,
			indices_count, // Each batch of 4 vertices is represented by 6 indices
			streamable ? api::batch_hint::KeepVertices : api::batch_hint::None,
		});

		// After we allocated the buffer, we should prepare its index buffer.
		{
			auto buffer = api::lock_batch_buffer(batch_object, IndexBuffer, false);
			finally buffer_unlock([&batch_object]() {
				api::unlock_batch_buffer(batch_object, IndexBuffer, true);
			});

			// Put values inside the index buffer, since for triangular batches it should not change
			// at all.
			auto view = buffer.view_as<u32>();
			ranges::generate(view, [n = 0]() mutable {
				constexpr static u32 offsets[] = { 0, 1, 2, 2, 3, 1 };

				// A quick explanation for the math here:
				// - (current / 6) << 2 = This is used to find the first vertex in quad.
				// - OFFSETS[current % 6] =  Get the vertex offset in this quad according to its index.
				const auto current = n++;
				return ((current / 6) << 2) + offsets[current % 6];
			});
		}

		return batch(std::move(batch_object));
	}

	void batch::publish(const size_t vertices) const
	{
		api::publish_batch(m_Batch, vertices);
	}

	void batch::link_with_pipeline(const pipeline_stage& pipeline) const
	{
		api::bind_batch_to_pipeline(m_Batch, pipeline);
	}
}
