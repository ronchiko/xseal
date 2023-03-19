---@diagnostic disable: undefined-global
language "C++"
cppdialect "C++20"

defines { "X_SEAL" }
workspace "XSeal"
    startproject "seal_core"

    configurations { 
        "Release", 
        "Debug"
    }
    platforms { 
        "Windows", 
        "Emscripten" 
    }
    
    includedirs {
        "include",
        "thirdparty/fmt/include",
        "thirdparty/glfw/include",
        "thirdparty/glm"
    }

    flags {
		"MultiProcessorCompile"
	}

    filter {"configurations:Debug"}
        defines { "SEAL_DEBUG" }
        buildoptions "/MTd"
        symbols "on"
        runtime "Debug"

    filter {"configurations:Release"}
        buildoptions "/MT"
        optimize "on"
        runtime "Release"

    filter {"platforms:Windows"}
        defines { "SEAL_WINDOWS" }
        system "Windows"
        architecture "x64"

    filter {"platforms:Emscripten"}
        defines { "SEAL_WEBGL" }

    filter {}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture }"

-- Base projects
include "src/core"
include "include/seal"

-- Thirdparty projects
group "Thirdparty"
    include "thirdparty"
    include "thirdparty/glad"
group ""
