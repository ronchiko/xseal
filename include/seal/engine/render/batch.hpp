#pragma once

#include "seal/engine/render/pipeline/pipeline_stage.hpp"

#include "seal/api/back/vertex.hpp"

namespace seal {
	class batch
	{
	public:
		struct un_mapper
		{
			api::batch_buffer_type type;

			void operator()(const api::abstract_t *object) const
			{
				try {
					api::unlock_batch_buffer(*object, type, true);
				} catch(const std::exception& e) {
					seal::log::warning("Failed to un-map buffer batch: {}", e.what());
				}
			}
		};

		// Context type for locking a buffer.
		using mapped_buffer_context = std::unique_ptr<api::abstract_t, un_mapper>;

		// The result type for the batch::lock function
		template<typename ElementT>
		using lock_result = std::pair<std::span<ElementT>, mapped_buffer_context>;

		batch(const batch& other) = delete;
		batch& operator=(const batch& other) = delete;

		batch(batch&& other) = default;
		batch& operator=(batch&& other) = default;

		~batch();


		/**
		   Creates a new batch that contains triangles.
		   Note: The indices count will be calculated according to vertex_count.

		   \param vertex_count: The amount of vertices to allow in the batch.
		   \param streamable: Will the batch data be modified repeatedly?
		 */
		static batch create_triangular(size_t vertex_count, bool streamable = false);

		/**
		   Locks a memory of a buffer inside this buffer for editing.

		   \param type: The type of buffer to lock.
		 */
		template<typename ElementT>
		lock_result<ElementT> lock(api::batch_buffer_type type);

		/*
			Publishes the batch to the screen.
		*/
		void publish(size_t vertices) const;

		/*
			Links this batch with a pipeline
		*/
		void link_with_pipeline(const pipeline_stage& pipeline) const;

	private:
		explicit constexpr batch(api::abstract_t batch)
			: m_Batch(std::move(batch))
		{}

		api::abstract_t m_Batch;
	};
}

template<typename ElementT>
seal::batch::lock_result<ElementT> seal::batch::lock(const api::batch_buffer_type type)
{
	auto buffer = lock_batch_buffer(m_Batch, type, false);
	// We can keep a pointer to m_Batch because the context cannot outlive
	// batch, at since we have a custom deleter, we don't actually call
	// `delete` on it.
	std::unique_ptr<api::abstract_t, un_mapper> owning_resource{ &m_Batch, un_mapper{ type } };
	return std::make_pair(buffer.view_as<ElementT>(), std::move(owning_resource));
}