#pragma once

#include "seal/types/ext/matrix.hpp"
#include "seal/types/ext/vector.hpp"

#include "gl.hpp"

namespace seal::gl {
	struct vao_context
	{
	public:
		using vao_t = GLuint;

		constexpr vao_context(vao_t vao, size_t vertex_size);

		/**
		   Binds a structure to the VAO.
		 */
		template<typename StructureT>
		result<void> bind(u32 location);

	private:
		/**
		   Binds an attribute with a given size.

		   \param attribute_location: The location of the attribute to bind.
		   \param size: The amount of floats the attribute takes.
		 */
		result<void> bind_raw(u32 attribute_location, u32 size);

		vao_t m_Vao = 0;
		size_t m_VertexSize = 0;
		size_t m_Used = 0;
	};
}

constexpr seal::gl::vao_context::vao_context(vao_t vao, size_t vertex_size)
	: m_Vao(vao)
	, m_VertexSize(vertex_size)
	, m_Used(0)
{
	glBindVertexArray(vao);
}

template<typename StructureT>
seal::result<void> seal::gl::vao_context::bind(const u32 location)
{
	if constexpr(seal::is_matrix_v<StructureT>) {
		using matrix = seal::matrix_traits<StructureT>;
		using matrix_element = typename matrix::element_type;

		for(u32 n = 0; n < matrix::height; ++n) {
			bind<glm::vec<matrix::width, matrix_element, matrix::qualifier>>(location + n);
		}
		
		return {};
	} else if constexpr(seal::is_vector_v<StructureT>) {
		using vec = seal::vector_traits<StructureT>;
		constexpr static auto size = vec::size;

		static_assert(std::is_same_v<typename vec::element_type, f32>,
					  "Vector must have a floating element type");

		return bind_raw(location, size);
	} else {
		return seal::fail<seal::failure::NotImplemented>();
	}
}
