---@diagnostic disable: undefined-global

require "ems"
require "os"

WKS_DIR = os.getcwd()

language "C++"
cppdialect "C++20"
cdialect "C17"

defines { "X_SEAL" }
workspace "XSeal"
    kind "StaticLib"
    staticruntime "on"

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
        "thirdparty/glad_gl4/include",
        "thirdparty/glad_es2/include",
        "thirdparty/glfw/include",
        "thirdparty/glm",
        "thirdparty/entt/single_include"
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
        defines { "SEAL_WINDOWS", "SEAL_ENABLE_EXCEPTIONS", "SEAL_GL4" }
        system "Windows"
        architecture "x64"
        startproject "seal_glfw"

    filter {"platforms:Emscripten"}
        defines { "SEAL_WEBGL", "SEAL_GLES2" }
        system "Emscripten"
        architecture "x86"
        linkoptions {
            "-lglfw",
        }
        removelinks {"glfw", "glad*"}
        startproject "seal_ems"

    filter {}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Base projects
include "src/core"
include "include/seal"

group "Frontends"
    filter {"platforms:Windows"}
        include "src/front/glfw"
    filter {"platforms:Emscripten"}
        include "src/front/emscripten"
    filter {}
group ""

group "Backends"
    include "src/back/gl"
group ""

group "Tests"
    include "tests/seal_core_tests"
group "" 

-- Thirdparty projects
group "Thirdparty"
    include "thirdparty"
    include "thirdparty/glad_gl4"
    include "thirdparty/glad_es2"
group ""
