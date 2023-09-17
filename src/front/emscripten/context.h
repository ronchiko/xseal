#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "seal/engine/engine.hpp"

namespace seal::ems {
	class context
	{
	public:
		static context& get();

		GLFWwindow *window;

	private:
		inline static std::unique_ptr<context> s_Context = nullptr;
	};
}
