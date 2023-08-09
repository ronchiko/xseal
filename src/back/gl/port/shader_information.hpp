#pragma once

#include <variant>

#include "seal/types.hpp"

#include "gl.hpp"

namespace seal::gl {
	
	class shader_attribute_location
	{
	public:
		constexpr shader_attribute_location(const u32 location)
			: location(location)
		{}
		
		/**
		   Converts this location to an integer.
		 */
		constexpr operator u32() const {
			return location;
		}

		/**
		   Checks if this location is a valid one.
		 */
		constexpr bool is_error() const;

	private:
		u32 location;
	};


	struct shader_information
	{
		struct graphics
		{
			u32 vertex_handle;
			// u32 uv_handle; TODO: When textures
			u32 tint_handle;
		};

		std::variant<graphics> m_Current;

		result<void> bind_to_vao(GLuint id);
	};
}

namespace seal {
	template<>
	struct error_value<gl::shader_attribute_location>
	{
		constexpr static gl::shader_attribute_location VALUE{ static_cast<u32>(-1) };
	};
}

constexpr bool seal::gl::shader_attribute_location::is_error() const
{
	return error_value_v<shader_attribute_location> != location;
}
