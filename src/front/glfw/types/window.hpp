#pragma once

#include <GLFW/glfw3.h>

#include "seal/types.hpp"

namespace seal::glfw {
	class window
	{
	public:
		constexpr window() = default;

		constexpr window(nullptr_t) noexcept;
		window& operator=(nullptr_t) noexcept;

		window(const window&) = delete;
		window& operator=(const window&) = delete;

		window(window&&) noexcept;
		window& operator=(window&& window) noexcept;

		~window();

		/**
			Creates a new windows with a scale and a title.

			\param scale: The scale of the window
			\param title: The title of the window
		 */
		static window create_window(v2<f32> scale, const std::string& title);

		/**
		   Checks if the window should close.
		 */
		bool should_close() const;

		/**
		   Swaps the windows buffers.
		 */
		void swap_buffers() const;

		/**
		   Gets the resolution of this window.
		 */
		v2<u32> resolution() const;

	private:
		explicit constexpr window(GLFWwindow *window) noexcept;

		void release_window() noexcept;

		GLFWwindow *m_Window = nullptr;
	};
}
