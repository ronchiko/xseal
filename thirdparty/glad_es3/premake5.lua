local XSealLibrary = require "xseal/library"

local function _create_library()
    local library = XSealLibrary:new {
        name = "GladEs3",
        include_root = "include"
    }

    return library
end

return _create_library():generate()
