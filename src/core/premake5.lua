---@diagnostic disable: undefined-global
project "seal_core"
    kind "StaticLib"
    
    files {
        "**.cpp",
        "**.hpp",
        "**.h",
        "**.inl",
    }

    includedirs {
        "."
    }

    links {
        "glfw",
        "fmt"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")