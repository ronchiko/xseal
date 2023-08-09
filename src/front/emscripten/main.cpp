
#include <emscripten.h>
#include <GLFW/glfw3.h>

#include "seal/api/back/backend.hpp"

#include "seal/engine/components/transform.hpp"
#include "seal/engine/ecs/manager.hpp"
#include "seal/engine/ecs/system.hpp"
#include "seal/engine/systems/all.hpp"

#include "context.h"


using namespace seal;

void mainloop()
{
	glfwPollEvents();
	auto& ctx = ems::context::get();
	seal_mute_exceptions({ ctx.engine.tick(0); });

	glfwSwapBuffers(ctx.window);
}

int main()
{
	auto& ctx = ems::context::get();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	ctx.window = glfwCreateWindow(1200, 700, "XSeal WebHost", nullptr, nullptr);
	glfwMakeContextCurrent(ctx.window);

	auto engine = seal::engine::create();
	seal_verify_result(engine);

	seal::api::update_resolution({ 1200, 700 });

	ctx.engine = std::move(*engine);

	emscripten_set_main_loop(mainloop, 0, true);

	return 0;
}