#include "types/window.hpp"

#include "glfw.hpp"

namespace seal::glfw {
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

	window& window::operator=(window&& other) noexcept
	{
		release_window();

		std::swap(m_Window, other.m_Window);

		return *this;
	}

	window::~window()
	{
		release_window();
	}

	result<window> window::create_window(v2<f32> scale, const std::string& title)
	{
		auto glfw_window = glfwCreateWindow(static_cast<i32>(scale.x),
											static_cast<i32>(scale.y),
											title.c_str(),
											nullptr,
											nullptr);

		seal_glfw_verfiy(glfw_window);
		glfwMakeContextCurrent(glfw_window);

		return window(glfw_window);
	}

	bool window::should_close() const {
		glfwPollEvents();
		return glfwWindowShouldClose(m_Window);
	}
	
	void window::swap_buffers() {
		glfwSwapBuffers(m_Window);
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
