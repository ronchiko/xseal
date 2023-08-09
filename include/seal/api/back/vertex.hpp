#pragma once

#include "seal/types.hpp"
#include "seal/types/color.hpp"
#include "seal/types/flags.hpp"
#include "seal/types/result.hpp"

#include "seal/api/buffer.hpp"
#include "seal/api/tagged_object.hpp"


namespace seal::api {
#pragma pack(push, 1)

	/**
	   The structure of a vertex passed to a shader.
	 */
	struct vertex
	{
		constexpr vertex() = default;

		constexpr vertex(v3<f32> position, v2<f32> uv, color tint)
			: position(position)
			// , uv(uv) TODO: When textures
			, tint(tint)
		{}

		v3<f32> position = v3<f32>{ 0, 0, 0 };
		// v2<f32> uv = v2<f32>{ 0, 0 }; TODO: When textures
		color tint = WHITE;
	};

#pragma pack(pop)

	enum class batch_hint : u32
	{
		None = 0,

		KeepIndecies = bit(0),	 // Hints the batch should keep the indecies in memory
		KeepVertecies = bit(1), // Hints the batch should keep the vertecies in memory
	};

	/**
	   Parameters for initializing a batch.
	 */
	struct batch_initialization
	{
		size_t vertecies;
		size_t indecies;

		flags<batch_hint> hints{};
	};

	/**
	   The types of buffers a batch could hold.
	 */
	enum class batch_buffer_type
	{
		VertexBuffer,
		IndexBuffer
	};

	/**
	   Creates a new batch with a desired size.

	   \param initialize: The initialization arguments to the batch.

	   \returns: The id of the batch.
	 */
	result<abstract_t> create_batch(const batch_initialization& initialize);

	/**
	   Frees a batch.

	   \param batch: The batch to free.
	 */
	void free_batch(abstract_t batch);

	/**
	   Locks the memory associated with a batch, allowing reading and writing from it.
	   Note: Locks dont stack! Doesn't matter how much you call this function, one unlock will unlock the buffer.

	   \param batch: The the batch to lock the buffer from.
	   \param type: The buffer indside the batch to lock.
	   \param read: Should read the data into the current buffer.

	   \return The buffer type associated with the batch.
	 */
	result<buffer> lock_batch_buffer(abstract_t batch, batch_buffer_type type, bool read);

	/**
	   Unlocks the memory associated with a batch (locked with lock_batch_buffer).
	   Note: Unlocking an unlocked buffer wont do anthing.

	   \param batch: The batch the buffer was locked from.
	   \param type: The type of buffer to unlock.
	   \param write: Write the changes to the buffer

	   \return The result of the operation.
	 */
	result<void> unlock_batch_buffer(abstract_t batch, batch_buffer_type type, bool write);

	/**
	   Binds a batcher to a pipline, generating the nessecary information for the binding.

	   \param batch: The batch to bind
	   \param pipline: The pipeline to bind to
	 */
	result<void> bind_batcher_to_pipeline(abstract_t batch, abstract_t pipeline);

	/**
	   Renderers a batch to the screen.
	 */
	result<void> publish_batch(abstract_t batch, size_t vertecies);

}
