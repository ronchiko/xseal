#pragma once

#include <vector>

#include "objects/buffer.hpp"
#include "objects/vertex_array.hpp"
#include "seal/api/back/vertex.hpp"

namespace seal::gl {
	class batch
	{
	public:
		batch() = default;

		static result<batch> create_batch(size_t max_vertex_count);

		/**
		   Pushes a vertex into the batch.
		  
		   \param buffer: The buffer of vertecies to push
		   \param amount: The amount of vertecies to push
		 */
		void push_vertecies(api::vertex *buffer, size_t amount);

		/**
		   Publishes this batch.
		 */
		void publish();

	private:
		batch(vertex_array vao, buffer vbo, buffer ibo, std::vector<api::vertex> vertecies);

		vertex_array m_Vao;
		buffer m_Vbo;
		buffer m_Ibo;

		std::vector<api::vertex> m_Vertecies;
	};

}
