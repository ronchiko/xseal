---@diagnostic disable: undefined-global
project "seal_include"    
    files {
        "**.cpp",
        "**.hpp",
        "**.h",
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")
