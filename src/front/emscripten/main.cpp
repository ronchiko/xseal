
#include <emscripten.h>
#include <GLFW/glfw3.h>

#include "seal/api/back/backend.hpp"
#include "seal/render/pipeline.hpp"
#include "seal/render/context.hpp"

#include "context.h"

extern seal::result<void> register_all_embedded_resources();

using namespace seal;

void mainloop() {
	glfwPollEvents();
	auto& ctx = ems::context::get();

	{
		seal::render_context render_ctx{};

		ctx.pipeline.bind();
	}

	glfwSwapBuffers(ctx.window);
}

int main()
{
	seal_verify_result(register_all_embedded_resources());
	
	auto& ctx = ems::context::get();
	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	ctx.window = glfwCreateWindow(1200, 700, "My window", nullptr, nullptr);
	glfwMakeContextCurrent(ctx.window);

	seal::log::info("No shot!");

	api::initialize_backend();

	ctx.pipeline = pipeline::create_graphics(api::pipeline_description::graphics{
		load_resource(R"(::\Vertex.glsl)").except("Failed to load vertex shader"),
		load_resource(R"(::\Fragment.glsl)").except("Failed to load fragment shader"),
	}).except("Failed to create pipeline");

	emscripten_set_main_loop(mainloop, 0, true);

	return 0;
}