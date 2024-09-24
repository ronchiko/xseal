local Filter = require "xseal/solution/filters"
local SolutionBuilder = require "xseal/solution/solution_builder"
local XSealLibrary = require "xseal/library"


local function _create_library()
    local xSealGl = XSealLibrary:new {
        name = "XSealGLBackend"
    }

    xSealGl:setup_linking {
        links = {
            "fmt",
            "XSeal"
        }
    }

    xSealGl:filtered(Filter:platform("Windows"), function()
        SolutionBuilder:set("links", { "glad_gl4" })
        SolutionBuilder:set("includedirs", { "%{wks.location}/thirdparty/glad_gl4/include" })
    end)

    xSealGl:filtered(Filter:platform("WebGL"), function()
        SolutionBuilder:set("links", { "glad_es3" })
        SolutionBuilder:set("includedirs", { "%{wks.location}/thirdparty/glad_es3/include" })
    end)

    return xSealGl
end

return _create_library():generate()
