---@diagnostic disable: undefined-global

project "seal_ems.js"
    kind "ConsoleApp"

    filter { "system:Emscripten" }
        prebuildcommands {
            "python %{WKS_DIR}/build/embed.py -d resources/ --recurse -o .embed.generated.cpp"
        }
        files {
            "**.cpp",
            "**.hpp",
            "**.h"
        }

        removefiles { "resources/**" }

        includedirs {
            ".",
            "C:/Dev/emsdk/upstream/emscripten/cache/sysroot/include"
        }

        links {
            "seal_core",
            "seal_resources",
            "seal_gl",
            "fmt",
            "glad_es3",
            "glfw3"
        }

        linkoptions {
            "-s USE_WEBGL2=1",
            "-s FULL_ES3=1",
            "-s WASM=3",
            "-s USE_GLFW=3",
            "-fwasm-exceptions",
            "-error-limit=0"
        }

    filter ""
