#pragma once

#include "gl/gl.hpp"

namespace seal::gl {

	class Shader
	{
	public:
		enum class Type
		{
			Vertex,
			Fragment,
			Compute,
		};

	private:
		gl::Id m_Shader;
		Type m_Type;
	};
}