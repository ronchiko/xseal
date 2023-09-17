#pragma once

#include "seal/api/resource.hpp"

#include "gl.hpp"
#include "gl_id.hpp"

namespace seal::gl {
	class shader
	{
	public:
		/**
		   The type of the shader.
		 */
		enum class type
		{
			Vertex,
			Fragment,
			Compute
		};

		/**
		   Creates a shader from a resource.
		  
		   \param resource: The resource to create the shader from
		   \param type: The type of the shader
		 */
		static shader from_resource(const resource& resource, type type);

		[[nodiscard]]
		constexpr GLuint id() const {
			return m_Id;
		}

	private:
		explicit shader(gl_id id) noexcept
			: m_Id(std::move(id))
		{}

		gl_id m_Id;
	};
}
