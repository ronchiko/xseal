---@diagnostic disable: undefined-global
project "seal_resources"      
    kind "StaticLib"

    prebuildcommands {
        "python %{WKS_DIR}/build/embed.py -d root/ --recurse -o .embed.generated.cpp --export register_global_resources"
    }

    files {
        ".embed.generated.cpp",
        "root/**"
    }

    removefiles "resources/**"

    includedirs {
        "."
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")