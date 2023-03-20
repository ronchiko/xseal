---@diagnostic disable: undefined-global

require "ems"

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
        symbols "on"
        runtime "Debug"

    filter {"configurations:Release"}
        optimize "on"
        runtime "Release"

    filter {"platforms:Windows"}
        defines { "SEAL_WINDOWS" }
        system "Windows"
        architecture "x64"

    filter {"platforms:Emscripten"}
        defines { "SEAL_WEBGL" }
        system "Emscripten"
        architecture "x64"

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
