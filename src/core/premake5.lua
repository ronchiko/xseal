---@diagnostic disable: undefined-global
project "seal_core"
    kind "ConsoleApp"
    
    files {
        "**.cpp",
        "**.hpp",
        "**.h",
    }

    links {
        "glfw",
        "glad",
        "fmt"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")