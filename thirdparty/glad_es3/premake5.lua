---@diagnostic disable: undefined-global
project "glad_es3"
    kind "StaticLib"
    language "C"

    targetdir ("out/" .. output_dir .. "/%{prj.name}")
    objdir ("obj/" .. output_dir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
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
