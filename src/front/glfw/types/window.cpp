#include "seal/api/core/display.hpp"

#include "types/window.hpp"

#include "glfw.hpp"
#include "seal/log/log.hpp"
#include "seal/defenitions.h"

namespace seal::glfw {
	namespace 
	{
		void glfw_window_size_changed(GLFWwindow *window, const int width, const int height)
		{
			api::signal_viewport_change(v2<u32>{ width, height });
		}
	}

	constexpr window::window(nullptr_t) noexcept
		: m_Window(nullptr)
	{}

	window& window::operator=(nullptr_t) noexcept
	{
		release_window();

		return *this;
	}

	window::window(window&& other) noexcept
		: m_Window(other.m_Window)
	{
		other.m_Window = nullptr;
	}

	window& window::operator=(window&& window) noexcept
	{
		release_window();

		std::swap(m_Window, window.m_Window);

		return *this;
	}

	window::~window()
	{
		release_window();
	}

	window window::create_window(const v2<f32> scale, const std::string& title)
	{
		auto glfw_window = glfwCreateWindow(static_cast<i32>(scale.x),
											static_cast<i32>(scale.y),
											title.c_str(),
											nullptr,
											nullptr);

		glfwMakeContextCurrent(glfw_window);
		glfwSetFramebufferSizeCallback(glfw_window, &glfw_window_size_changed);

		return window(glfw_window);
	}

	bool window::should_close() const
	{
		glfwPollEvents();
		return glfwWindowShouldClose(m_Window);
	}

	void window::swap_buffers() const
	{
		glfwSwapBuffers(m_Window);
	}

	v2<u32> window::resolution() const
	{
		int width = 0, height = 0;
		glfwGetWindowSize(m_Window, &width, &height);

		return v2<u32>{ width, height };
	}

	constexpr window::window(GLFWwindow *window) noexcept
		: m_Window(window)
	{}

	void window::release_window() noexcept
	{
		seal_mute_exceptions({
			if(nullptr != m_Window) {
				glfwDestroyWindow(m_Window);
				m_Window = nullptr;
			}
		});
	}
}
