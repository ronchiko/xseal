require "xseal/ems"

local IPlatform = require "xseal/platforms/platform"
local XSEAL_LIB = require "xseal/enum/libraries"
local XSEAL_ARCH = require "xseal/enum/architecture"
local PROJECT_KIND = require "xseal/solution/project_type"
local SolutionBuilder = require "xseal/solution/solution_builder"


local _WEBGL_SYSTEM_NAME = "WebGL"

local WebGLPlatform = IPlatform:new { name = "WebGL" }

function WebGLPlatform:attach_build_configuration()
    SolutionBuilder:add_definition("SEAL_WEBGL")
    SolutionBuilder:add_definition("SEAL_GLES3")

    SolutionBuilder:set("system", _WEBGL_SYSTEM_NAME)
    SolutionBuilder:set("architecture", XSEAL_ARCH.WebGL)
end

function WebGLPlatform:create_boot_project(base_project_name)
    print('Creating WebGL project...')

    SolutionBuilder:create_project(self:boot_project_name(base_project_name), PROJECT_KIND.Console, self.name)
    SolutionBuilder:setup_project_dependencies({ XSEAL_LIB.WebGL, base_project_name })

    -- Link with external GLFW
    SolutionBuilder:set("linkoptions", { "-lglfw" })
    SolutionBuilder:set("entrypoint", "main")
end

function WebGLPlatform:boot_project_name(name)
    return name .. ".js"
end

return WebGLPlatform
