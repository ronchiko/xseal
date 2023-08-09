#include "port/shader_information.hpp"

#include "seal/api/back/vertex.hpp"
#include "seal/types/overload.hpp"

#include "pipeline/vao_context.hpp"

namespace seal::gl {

	result<void> shader_information::bind_to_vao(GLuint vao)
	{
		seal::overload overload{
			[&](const graphics& graphics) -> result<void> {
				vao_context context(vao, sizeof(api::vertex));

				seal_verify_result(context.bind<v3<f32>>(graphics.vertex_handle));
				// seal_verify_result(context.bind<v2<f32>>(graphics.uv_handle)); TODO: When textures
				seal_verify_result(context.bind<v4<f32>>(graphics.tint_handle));
				return {};
			},
			[](const auto& any) -> result<void> {
				return seal::fail<seal::failure::NotImplemented>("VAO binding failed");
			},
		};

		return std::visit(overload, m_Current);
	}
}
