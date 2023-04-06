#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "seal/render/pipeline.hpp"

namespace seal::ems {
	class context
	{
	public:
		static context& get();

		seal::pipeline pipeline;
		GLFWwindow *window;

	private:
		inline static std::unique_ptr<context> g_Context = nullptr;
	};
}
