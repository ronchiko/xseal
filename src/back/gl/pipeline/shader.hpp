#pragma once

#include "seal/api/resource.hpp"

#include "gl.hpp"
#include "gl_id.hpp"

namespace seal::gles3 {
	class shader
	{
	public:
		/**
		   The type of the shader.
		 */
		enum class type
		{
			Vertex,
			Fragement,
			Compute
		};

		/**
		   Creates a shader from a resource.
		  
		   \param rsrc: The resource to create the shader from
		   \return 
		 */
		static result<shader> from_resource(resource rsrc, type type);

		constexpr GLuint id() const {
			return m_Id;
		}

	private:
		inline shader(gl_id id) noexcept
			: m_Id(std::move(id))
		{}

		gl_id m_Id;
	};
}
