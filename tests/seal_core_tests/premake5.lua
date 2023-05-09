---@diagnostic disable: undefined-global
project "seal_core_ut"
    kind "ConsoleApp"
    language "C++"
    cdialect "C17"
	cppdialect "C++20"

    targetdir ("out/" .. output_dir .. "/%{prj.name}")
    objdir ("obj/" .. output_dir .. "/%{prj.name}")

    includedirs {
        "%{WKS_DIR}/thirdparty/googletest/googletest/include",
        "%{WKS_DIR}/include",
        "%{WKS_DIR}/tests"
    }

    filter {"platforms:Windows"}
        files {
            "**.cpp",
            "**.hpp"
        }

    links {
        "seal_core",
        "google-test"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
