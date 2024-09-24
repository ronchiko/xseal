local IPlatform = require "xseal/platforms/platform"
local XSEAL_LIB = require "xseal/enum/libraries"
local XSEAL_ARCH = require "xseal/enum/architecture"
local PROJECT_KIND = require "xseal/solution/project_type"
local Filter = require "xseal/solution/filters"
local SolutionBuilder = require "xseal/solution/solution_builder"


local WindowsPlatform = IPlatform:new { name = "Windows" }

function WindowsPlatform:attach_build_configuration()
    SolutionBuilder:add_definition("SEAL_WINDOWS")
    SolutionBuilder:add_definition("SEAL_GL4")

    SolutionBuilder:set("system", self.name)
    SolutionBuilder:set("architecture", XSEAL_ARCH.X64)
end

function WindowsPlatform:create_boot_project(base_project_name)
    print('Creating windows project...')

    SolutionBuilder:create_project(self:boot_project_name(base_project_name), PROJECT_KIND.Console, self.name)
    SolutionBuilder:setup_project_dependencies({ XSEAL_LIB.Windows, base_project_name })

    SolutionBuilder:filtered(Filter:config("debug"), function()
        SolutionBuilder:set("buildoptions", {
            "/MTd"
        })
    end)
    SolutionBuilder:filtered(Filter:config("release"), function()
        SolutionBuilder:set("buildoptions", {
            "/MT"
        })
    end)
end

function WindowsPlatform:boot_project_name(name)
    return name .. ".exe"
end

return WindowsPlatform
