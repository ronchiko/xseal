#include "port/shader_information.hpp"

#include "seal/api/back/vertex.hpp"
#include "seal/types/overload.hpp"

#include "pipeline/vao_context.hpp"

namespace seal::gl {

	void shader_information::bind_to_vao(const GLuint vao)
	{
		seal::overload overload{
			[&](const graphics& graphics) {
				vao_context context(vao, sizeof(api::vertex));

				context.bind<v3<f32>>(graphics.vertex_handle);
				context.bind<v2<f32>>(graphics.uv_handle);
				context.bind<v4<f32>>(graphics.tint_handle);
			},
			[](const auto& any) {
				throw seal::fail<seal::failure::NotImplemented>("VAO binding failed");
			},
		};

		std::visit(overload, current);
	}
}
