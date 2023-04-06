#include "seal/api/resource.hpp"

namespace seal {
	inline const char
		*DEFAULT_VERTEX_SHADER = "#version 450 core\n"
								 "layout(location = 0) in vec3 aPos;\n" // the position variable
																		// has attribute position 0
								 "out vec4 vertexColor;\n" // specify a color output to the
														   // fragment shader

								 "void main() {\n"
								 "gl_Position = vec4(aPos, 1.0);\n" // see how we directly give a
																	// vec3 to vec4's constructor
								 "vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // set the output
																			 // variable to a
																			 // dark-red color
								 "}";

	inline const char *DEFAULT_FRAGMENT_SHADER = "#version 450 core\n"
												 "out vec4 FragColor;\n"
												 "in vec4 vertexColor;\n" // the input variable from
																		  // the vertex shader (same
																		  // name and same type)
												 "void main() {\n"
												 "FragColor = vertexColor;\n"
												 "}";

	inline result<void> initialize_builtin_shaders()
	{
		seal_verify_result(seal::add_embedded_resource(R"(Default\Vertex.glsl)",
													   DEFAULT_VERTEX_SHADER,
													   strlen(DEFAULT_VERTEX_SHADER)));
		seal_verify_result(seal::add_embedded_resource(R"(Default\Fragment.glsl)",
													   DEFAULT_FRAGMENT_SHADER,
													   strlen(DEFAULT_FRAGMENT_SHADER)));

		return {};
	}
}
