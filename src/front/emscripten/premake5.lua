local XSealLibrary = require "xseal/library"


local function _create_project()
    local xSealWeb = XSealLibrary:new {
        name = "XSealWebGL",
        platform = "WebGL"
    }

    xSealWeb:add_source_file_pattern("index.html")
    xSealWeb:add_include_directory("C:/Dev/emsdk/upstream/emscripten/cache/sysroot/include") -- TODO: Understand where emsdk is

    xSealWeb:embed_resource { resources_directory = "resources/" }
    xSealWeb:setup_linking {
        merge = true,
        options = {
            "-s USE_WEBGL2=1",
            "-s FULL_ES3=1",
            "-s WASM=3",
            "-s USE_GLFW=3",
            "-fwasm-exceptions",
            "-error-limit=0"
        },
        links = {
            "seal_resources",
            "seal_gl",
            "fmt",
            "glad_es3",
            "glfw3"
        }
    }

    return xSealWeb
end

return _create_project():generate()
