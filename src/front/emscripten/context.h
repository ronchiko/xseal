#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "seal/engine/engine.hpp"

namespace seal::ems {
	class context
	{
	public:
		static context& get();

		seal::engine engine;
		GLFWwindow *window;

	private:
		inline static std::unique_ptr<context> g_Context = nullptr;
	};
}
