#pragma once

#include "seal/types/ext/matrix.hpp"
#include "seal/types/ext/vector.hpp"

#include "gl.hpp"
#include "port/shader_attribute_location.hpp"

namespace seal::gl {

	struct vao_context
	{
	public:
		using vao_t = GLuint;

		constexpr vao_context(vao_t vao, size_t vertex_size);

		vao_context(vao_context&&) = delete;
		vao_context& operator=(vao_context&&) = delete;

		vao_context(const vao_context&) = delete;
		vao_context& operator=(const vao_context&) = delete;

		~vao_context();

		/**
		   Binds a structure to the VAO.
		 */
		template<typename StructureT>
		void bind(shader_attribute_location location);

	private:
		/**
		   Binds an attribute with a given size.

		   \param attribute_location: The location of the attribute to bind.
		   \param size: The amount of floats the attribute takes.
		 */
		void bind_raw(u32 attribute_location, u32 size);

		vao_t m_Vao = 0;
		size_t m_VertexSize = 0;
		size_t m_Used = 0;
	};
}

constexpr seal::gl::vao_context::vao_context(const vao_t vao, const size_t vertex_size)
	: m_Vao(vao)
	, m_VertexSize(vertex_size)
	, m_Used(0)
{
	glBindVertexArray(vao);
}

template<typename StructureT>
void seal::gl::vao_context::bind(const shader_attribute_location location)
{
	// If ok if a shader doesn't need a specific attribute.
	if(INVALID_SHADER_ATTRIBUTE == location) {
		return;
	}

	if constexpr(seal::is_matrix_v<StructureT>) {
		using matrix = seal::matrix_traits<StructureT>;
		using matrix_element = typename matrix::element_type;

		for(u32 n = 0; n < matrix::height; ++n) {
			bind<glm::vec<matrix::width, matrix_element, matrix::qualifier>>(location + n);
		}
		
		return;
	} else if constexpr(seal::is_vector_v<StructureT>) {
		using vec = seal::vector_traits<StructureT>;
		constexpr static auto size = vec::size;

		static_assert(std::is_same_v<typename vec::element_type, f32>,
					  "Vector must have a floating element type");

		return bind_raw(location, size);
	} else {
		throw seal::fail<seal::failure::NotImplemented>();
	}
}
