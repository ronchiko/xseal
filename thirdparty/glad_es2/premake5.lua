---@diagnostic disable: undefined-global
project "glad_es2"
    kind "StaticLib"
    language "C"

    targetdir("out/" .. output_dir .. "/%{prj.name}")
    objdir("obj/" .. output_dir .. "/%{prj.name}")

    files
    {
        "include/glad/gles2.h",
        "include/KHR/khrplatform.h",
        "src/gles2.c"
    }

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
    runtime "Release"
    optimize "on"
