---@diagnostic disable: undefined-global
project "seal_core"
    kind "StaticLib"
    
    filter {}
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
        "fmt",
        "seal_resources"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")