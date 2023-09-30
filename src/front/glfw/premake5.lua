---@diagnostic disable: undefined-global
project "seal_glfw"      
    kind "ConsoleApp"

    filter { "system:Windows" }
        prebuildcommands {
            "python %{WKS_DIR}/scripts/build/embed.py -d resources/ --recurse -o .embed.generated.cpp"
        }

        files {
            "**.cpp",
            "**.hpp",
            "**.h",
            ".embed.generated.cpp"
        }

        removefiles "resources/**"

    includedirs {
        "."
    }

    links {
        "glfw",
        "fmt",
        "seal_gl",
        "seal_core"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")