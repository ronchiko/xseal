#pragma once

#include "seal/api/back/vertex.hpp"

#include "seal/types/result.hpp"

namespace seal {
	class batch
	{
	public:
		struct unmapper
		{
			api::batch_buffer_type type;

			void operator()(api::abstract_t *object) const
			{
				auto result = api::unlock_batch_buffer(*object, type, true);
				seal_debug_only(seal_wassert(!result.is_error(), "Failed to unmap buffer batch"));
			}
		};

		// Context type for locking a buffer.
		using mapped_buffer_context = std::unique_ptr<api::abstract_t, unmapper>;

		// The result type for the batch::lock function
		template<typename ElementT>
		using lock_result = result<std::pair<std::span<ElementT>, mapped_buffer_context>>;


		/**
		   Creates a new batch that contains triangles.
		   Note: The indecies count will be calculated according to vertex_count.

		   \param vertex_count: The amount of vertecies to allow in the batch.
		   \param streamable: Will the batch data be modified repeatedly?
		 */
		static result<batch> createTriangular(size_t vertex_count, bool streamable = false);

		/**
		   Locks a memory of a buffer inside this buffer for editing.

		   \param type: The type of buffer to lock.
		 */
		template<typename ElementT>
		lock_result<ElementT> lock(api::batch_buffer_type type);

		/*
			Publishes the batch to the screen.
		*/
		void publish(size_t vertecies) const;

	private:
		constexpr batch(api::abstract_t batch)
			: m_Batcher(std::move(batch))
		{}

		api::abstract_t m_Batcher;
	};
}

template<typename ElementT>
seal::batch::lock_result<ElementT> seal::batch::lock(api::batch_buffer_type type)
{
	auto buffer = api::lock_batch_buffer(m_Batcher, type, false);
	seal_verify_result(buffer);

	return std::make_pair(buffer->view_as<ElementT>(),
						  // We can keep a pointer to m_Batch because the context cannot outlive
						  // batch, at since we have a custom deleter, we dont actually call
						  // `delete` on it.
						  std::unique_ptr<api::abstract_t, unmapper>{ &m_Batcher, unmapper{ type } });
}