#pragma once

#include <variant>

#include "seal/types.hpp"

#include "gl.hpp"

#include "port/shader_attribute_location.hpp"


namespace seal::gl {
	struct shader_information
	{
		struct graphics
		{
			shader_attribute_location vertex_handle;
			shader_attribute_location uv_handle;
			shader_attribute_location tint_handle;
		};

		std::variant<graphics> current;

		void bind_to_vao(GLuint vao);
	};
}

