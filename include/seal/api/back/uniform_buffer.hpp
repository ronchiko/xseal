#pragma once

#include "seal/api/tagged_object.hpp"

namespace seal::api
{
	/**
		Allocates a uniform buffer.
		
		\param requested_size: The size we want our uniform buffer to be.
	 */
	abstract_t allocate_uniform_buffer(size_t requested_size);

	/**
		Frees an allocated uniform buffer.

		\param uniform_buffer: The uniform buffer to free.
	 */
	void free_uniform_buffer(abstract_t uniform_buffer);

	/**
		Updates the data inside the uniform data to be something different.

		\param uniform_buffer: The uniform buffer to update.
		\param data: The buffer to update.
	 */
	void update_uniform_buffer(abstract_t uniform_buffer, const void *data);

	void bind_uniform_buffer(abstract_t ubo);
}
