---@diagnostic disable: undefined-global

-- FMT library
project "fmt"
    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

    targetdir ("out/" .. output_dir .. "/%{prj.name}")
    objdir ("obj/" .. output_dir .. "/%{prj.name}")

    includedirs {
        "fmt/include"
    }

    files {
        "fmt/src/format.cc"
    }

	filter "platforms:windows"
		files {
			"fmt/src/os.cc"
		}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Google test --
project "google-test"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("out/" .. output_dir .. "/%{prj.name}")
	objdir ("obj/" .. output_dir .. "/%{prj.name}")

	includedirs {
		"googletest/googletest/include",
		"googletest/googletest",
	}

	files {
		"googletest/googletest/src/gtest-all.cc",
		"googletest/googletest/*.h",
		"googletest/googletest/*.hpp",
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


-- GLFW library
project "glfw"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")
	
	filter { "system:not Emscripten" }
		files
		{
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
		}
	filter {"system:Linux"}
		pic "On"

		systemversion "latest"

		files
		{
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
		}

		defines
		{
			"_GLFW_X11"
		}

	filter {"system:Windows"}
		systemversion "latest"

		files	
		{
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
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

