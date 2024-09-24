local XSealLibrary = require "xseal/library"

local function _create_library()
    local xSealResources = XSealLibrary:new { name = "XSealResources" }
    xSealResources:add_source_file_pattern("root/**")

    xSealResources:embed_resource {
        resources_directory = "root",
        export_name = "register_global_resources"
    }


    return xSealResources
end

return _create_library():generate()
