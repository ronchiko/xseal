
#include <iostream>

#include <GLFW/glfw3.h>

#include "glfw.hpp"
#include "types/window.hpp"

#include "seal/types/result.hpp"

#include "seal/api/resource.hpp"
#include "seal/api/back/backend.hpp"
#include "seal/render/pipeline.hpp"
#include "seal/render/context.hpp"

extern seal::result<void> register_all_embedded_resources();


seal::result<void> initialize_engine()
{
	seal_verify_result(register_all_embedded_resources());

	seal_verify_result(seal::log::initialize());
	seal::log::debug("Starting to initialize engine");

	seal_glfw_verfiy(glfwInit());

	return {};
}

seal::result<void> invoke_main()
{
	seal_verify_result(initialize_engine());

	auto window = seal::glfw::window::create_window(seal::v2<seal::f32>(512, 512), "My Game");
	seal_verify_result(window);

	seal_verify_result(seal::api::initialize_backend());

	auto pipeline = seal::pipeline::create_graphics({
		seal::load_resource(R"(::\Vertex.glsl)").except("Failed to load resource"), 
		seal::load_resource(R"(::\Fragment.glsl)").except("Failed to load resource"), 
	}).except("Failed to create pipeline");	

	while(!window->should_close()) {
		
		{
			seal::render_context render_ctx{};
			pipeline.bind();
		}

		window->swap_buffers();
	}

	return {};
}

int main()
{
	invoke_main().expect("Failed to run main...");
}
