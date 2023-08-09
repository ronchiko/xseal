#include "seal/engine/render/batch.hpp"

#include <algorithm>

namespace seal {

	result<batch> batch::createTriangular(size_t vertex_count, bool streamable /* = false*/)
	{
		using api::batch_buffer_type::IndexBuffer;

		seal_wassert(0 == vertex_count % 4, "The vertex count is not divisable by 4");

		const auto indecies_count = (vertex_count >> 2) * 6;
		auto batch_object = api::create_batch(api::batch_initialization{
			vertex_count,
			indecies_count, // Each batch of 4 vertecies is represented by 6 indecies
			streamable ? api::batch_hint::KeepVertecies : api::batch_hint::None,
		});
		seal_verify_result(batch_object);

		// After we allocated the buffer, we should prepare its index buffer.
		auto buffer = api::lock_batch_buffer(*batch_object, IndexBuffer, false);
		seal_verify_result(buffer);

		// Put values inside the index buffer, since for triangular batches it should not change at
		// all.
		auto view = buffer->view_as<u32>();
		std::generate(view.begin(), view.end(), [n = 0]() mutable {
			constexpr static u32 OFFSETS[] = { 0, 1, 2, 2, 3, 1 };

			// A quick explaination for the math here:
			// - (current / 6) << 2 = This is used to find the first vertex in quad.
			// - OFFSETS[current % 6] =  Get the vertex offset in this quad according to its index.
			const auto current = n++;
			return ((current / 6) << 2) + OFFSETS[current % 6];
		});

		seal_verify_result(api::unlock_batch_buffer(*batch_object, IndexBuffer, true));

		return batch(std::move(*batch_object));
	}


	void batch::publish(size_t vertecies) const {
		api::publish_batch(m_Batcher, vertecies);
	}

	result<void> batch::link_with_pipeline(pipeline& pipeline) {
		return api::bind_batcher_to_pipeline(m_Batcher, pipeline);
	}
}
