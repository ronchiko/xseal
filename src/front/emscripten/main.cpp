
#include <emscripten.h>
#include <GLFW/glfw3.h>

#include "seal/api/back/backend.hpp"
#include "seal/defenitions.h"
#include "seal/engine/components/transform.hpp"
#include "seal/engine/ecs/system.hpp"

#include "context.h"


using namespace seal;

namespace 
{
	void glfw_window_size_changed(GLFWwindow *window, const int width, const int height)
	{
		api::update_resolution(v2<u32>{ width, height });
	}

	void main_loop()
	{
		glfwPollEvents();
		const auto& ctx = ems::context::get();
		seal_mute_exceptions({ seal::engine::tick(0); });

		glfwSwapBuffers(ctx.window);
	}
}


int main()
{
	auto& ctx = ems::context::get();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	ctx.window = glfwCreateWindow(1200, 700, "XSeal WebHost", nullptr, nullptr);
	glfwMakeContextCurrent(ctx.window);
	glfwSetFramebufferSizeCallback(ctx.window, &glfw_window_size_changed);

	seal::engine::create();

	// TODO: Find dynamically
	seal::api::update_resolution({ 1200, 700 });

	emscripten_set_main_loop(main_loop, 0, true);

	return 0;
}