local XSealLibrary = require "xseal/library"

local function _create_project()
    local xSeal = XSealLibrary:new {
        name = "XSeal"
    }

    xSeal:setup_linking {
        links = {
            "fmt",
            "XSealResources"
        }
    }

    return xSeal
end

return _create_project():generate()
