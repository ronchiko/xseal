
#include <iostream>

#include <GLFW/glfw3.h>

#include "glfw.hpp"
#include "types/window.hpp"

#include "seal/types/result.hpp"
#include "seal/types/rotating_queue.hpp"

#include "seal/api/back/backend.hpp"
#include "seal/api/resource.hpp"
#include "seal/render/context.hpp"
#include "seal/render/pipeline.hpp"

#include "seal/engine/components/transform.hpp"
#include "seal/engine/engine.hpp"
#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/ecs/system.hpp"
#include "seal/engine/systems/all.hpp"


seal::result<void> initialize_engine()
{
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

	auto engine = seal::engine::create();
	seal_verify_result(engine);

	while(!window->should_close()) {
		engine->tick(0);
		window->swap_buffers();
	}

	return {};
}

int main()
{
	invoke_main().expect("Failed to run main...");
}
