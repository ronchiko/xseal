local XSealLibrary = require "xseal/library"


local function _create_library()
    local xSealWindows = XSealLibrary:new {
        name = "XSealWindows",
        platform = "Windows"
    }
    xSealWindows:embed_resource { resources_directory = "resources" }
    xSealWindows:setup_linking {
        merge = true,
        links = {
            "glfw",
            "fmt",
            "seal_gl"
        }
    }

    return xSealWindows
end

return _create_library():generate()
