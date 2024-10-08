
#include <emscripten.h>
#include <GLFW/glfw3.h>

#include "seal/api/core/display.hpp"
#include "seal/xseal.hpp"
#include "seal/defenitions.h"
#include "seal/engine/components/transform.hpp"
#include "seal/engine/ecs/system.hpp"

#include "context.h"


using namespace seal;

namespace 
{
	void glfw_window_size_changed(GLFWwindow *window, const int width, const int height)
	{
		api::signal_viewport_change(v2<u32>{ width, height });
	}

	void main_loop()
	{
		glfwPollEvents();
		const auto& ctx = ems::context::get();
		seal_mute_exceptions({ seal::engine::tick(0); });

		glfwSwapBuffers(ctx.window);
	}
}


void xseal::start_engine()
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
	api::signal_viewport_change({ 1200, 700 });

	emscripten_set_main_loop(main_loop, 0, true);
}