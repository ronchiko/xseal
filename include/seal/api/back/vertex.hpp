#pragma once

#include "seal/types.hpp"
#include "seal/types/color.hpp"

#include "seal/api/buffer.hpp"
#include "seal/api/tagged_object.hpp"

namespace seal::api {
#pragma pack(push, 1)

	/**
	   The structure of a vertex passed to a shader.
	 */
	struct vertex
	{
		v3<f32> position;
		v2<f32> uv;
		id texture;
		color tint;
	};

#pragma pack(pop)

	struct batch_initialization
	{
		size_t initial_size;
		bool allow_resize = false;
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

	   \param batch: The id of the batch.
	   \param reserve_vertecies[opt]: Tells the backend the amount vertecies we anticipate we will
									  put in it, if kept as 0, is ignored by the backend.

		\return The buffer buffer of the batch.
	 */
	result<buffer> lock_batch(abstract_t batch, size_t reserve_vertecies = 0);

	/**
	   Unlocks the memory associated with a batch (locked with lock_batch).
	 */
	result<void> unlock_batch(abstract_t batch);

}
