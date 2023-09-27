---@diagnostic disable: undefined-global
project "seal_gl"          
    files {
        "**.cpp",
        "**.hpp",
        "**.h",
    }

    includedirs {
        "."
    }

    links {
        "fmt",
        "seal_core"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")

    filter { "platforms:Windows" }
        links {
            "glad_gl4"
        }
        includedirs {
            "%{wks.location}/thirdparty/glad_gl4/include",
        }
    
    filter { "platforms:Emscripten" }
        links {
            "glad_es3"
        }
        includedirs {
            "%{wks.location}/thirdparty/glad_es3/include",
        }

    filter ""