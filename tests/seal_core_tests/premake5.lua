local XSealLibrary = require "xseal/library"

local function _create_project()
    local coreTests = XSealLibrary:new {
        name = "XSealCoreTests"
    }

    coreTests:setup_linking {
        links = {
            "XSeal",
            "google-test"
        }
    }

    coreTests:add_include_directory("${wks.location}/thirdparty/googletest/googletest/include")
    coreTests:add_include_directory("${wks.location}/include")
    coreTests:add_include_directory("${wks.location}/tests")

    return coreTests
end

return _create_project():generate()
