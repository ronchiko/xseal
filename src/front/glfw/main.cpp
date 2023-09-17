
#include <iostream>

#include <GLFW/glfw3.h>

#include "glfw.hpp"
#include "types/window.hpp"

#include "seal/api/back/backend.hpp"

#include "seal/engine/components/sprite.hpp"
#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/engine.hpp"
#include "seal/types/finally.hpp"

void initialize_engine()
{
	seal::log::initialize();
	seal::log::debug("Starting to initialize engine");

	seal_glfw_verify(glfwInit());
}

void invoke_main()
{
	initialize_engine();

	const auto window = seal::glfw::window::create_window(seal::v2<seal::f32>(512, 512), "My Game");
	seal::engine::create();
	seal::finally stop_engine([]() { seal::engine::stop(); });

	seal::api::update_resolution(window.resolution());

	seal::log::info("Finished loading engine - starting main loop");

	while(!window.should_close()) {
		seal::engine::tick(0);
		window.swap_buffers();
	}
}

int main()
{
	invoke_main();
}
