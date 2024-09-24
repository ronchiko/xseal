local SolutionBuilder = require "xseal/solution/solution_builder"
local XSealLibrary = require "xseal/library"
local Filter = require "xseal/solution/filters"

local function _create_fmt_library()
	local fmt = XSealLibrary:new {
		name = "fmt",
		initialize_sources = false,
	}

	fmt:add_include_directory("fmt/include")
	fmt:add_source_file_pattern("fmt/src/format.cc")

	fmt:filtered(Filter:platform("Windows"), function()
		SolutionBuilder:set("files", { "fmt/src/format.cc" })
	end)

	return fmt
end

-- GLFW library
local function _create_glfw_library()
	local glfw = XSealLibrary:new {
		name = "glfw",
		initialize_sources = false,
	}

	glfw:filtered(Filter:platform("not Emscripten"), function()
		SolutionBuilder:set("files", {
			"glfw/include/GLFW/glfw3.h",
			"glfw/include/GLFW/glfw3native.h",
			"glfw/src/glfw_config.h",
			"glfw/src/context.c",
			"glfw/src/init.c",
			"glfw/src/input.c",
			"glfw/src/null_init.c",
			"glfw/src/monitor.c",
			"glfw/src/platform.c",
			"glfw/src/vulkan.c",
			"glfw/src/window.c",
			"glfw/src/null_window.c",
			"glfw/src/null_monitor.c",
			"glfw/src/null_joystick.c",
		})
	end)

	glfw:filtered(Filter:platform("Linux"), function()
		SolutionBuilder:set("pic", "On")
		SolutionBuilder:set("systemversion", "Latest")
		SolutionBuilder:set("defines", { "_GLFW_X11" })

		SolutionBuilder:set("files", {
			"glfw/src/x11_init.c",
			"glfw/src/x11_monitor.c",
			"glfw/src/x11_window.c",
			"glfw/src/xkb_unicode.c",
			"glfw/src/posix_time.c",
			"glfw/src/posix_thread.c",
			"glfw/src/posix_module.c",
			"glfw/src/glx_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c",
			"glfw/src/linux_joystick.c"
		})
	end)

	glfw:filtered(Filter:platform("Windows"), function()
		SolutionBuilder:set("systemversion", "Latest")
		SolutionBuilder:set("defines", { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" })

		SolutionBuilder:set("files", {
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_module.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		})
	end)

	return glfw
end

-- Google test
local function _create_googletest_library()
	local googletest = XSealLibrary:new { name = "google-test", initialize_sources = false }

	googletest:add_include_directory("googletest/googletest/include")
	googletest:add_include_directory("googletest/googletest")
	googletest:add_source_file_pattern("googletest/googletest/src/gtest-all.cc")
	googletest:add_source_file_pattern("googletest/googletest/*.h")
	googletest:add_source_file_pattern("googletest/googletest/*.hpp")

	return googletest
end

include("thirdparty/glad_es2")
include("thirdparty/glad_es3")
include("thirdparty/glad_gl4")

_create_fmt_library()
_create_glfw_library()
_create_googletest_library()
