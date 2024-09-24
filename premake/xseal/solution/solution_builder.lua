local PROJECT_CONFIG = require "xseal/enum/configuration"
local Filter = require "xseal/solution/filters"
local SealBuildLocation = require "xseal/enum/build"

local SolutionBuilder = {}


--[[
    Sets a global premake value.

    @field parameter string
    @field value any
]]
--
function SolutionBuilder:set(parameter, value)
    -- Since for some reason you cant call premake functions from load functions, and
    -- we cant directly access the entries in _G, we have to iterate over the entire
    -- global scope and look for the specific premake value we are looking for, and
    -- manually invoke it.
    for global_name, global_value in pairs(_G) do
        if parameter == global_name then
            global_value(value)
            return
        end
    end

    error("No premake function named: " .. parameter)
end

--[[
    Adds a new macro definition for the project
]]
function SolutionBuilder:add_definition(name)
    self:set("defines", { name })
end

function SolutionBuilder:start_filter(filters)
    self:set("filter", filters)
end

function SolutionBuilder:filtered(filters, action)
    self:start_filter(filters)
    action()

    -- Finally clear the filter
    self:start_filter({})
end

function SolutionBuilder:group(name, action)
    self:set("group", name)
    action()
    self:set("group", "")
end

--[[
    Creates a new premake project

    @field name string
    @field kind string
    @field filter? string
]]
--
function SolutionBuilder:create_project(name, project_kind, project_platform)
    self:set("project", name)
    self:set("kind", project_kind)

    -- Setup the target and intermiate directories
    self:set("targetdir", SealBuildLocation.Exported)
    self:set("objdir", SealBuildLocation.Object)

    self:set("includedirs", { "." })

    if nil ~= project_platform then
        self:start_filter({ Filter:platform(project_platform) })
    end
end

--[[
    Sets the current projects startup project.

    @field name string
]]
--
function SolutionBuilder:set_startup_project(name)
    self:set("startproject", name)
end

--[[
    Setups the project dependencies for the currect active project.
]]
function SolutionBuilder:setup_project_dependencies(dependencies)
    self:set("links", dependencies)
end

--[[
    Creates a new premake workspace.

    @field workspace string
]]
function SolutionBuilder:create_workspace(workspace_name, cpp_dialect)
    self:set("workspace", workspace_name)
    self:set("language", "C++")
    self:set("cppdialect", cpp_dialect)
    self:set("configurations", { PROJECT_CONFIG.Debug, PROJECT_CONFIG.Release })

    self:filtered({ Filter:config(PROJECT_CONFIG.Debug) }, function()
        self:set("runtime", "Debug")
        self:set("symbols", "on")

        -- Marco for debugging
        self:add_definition("XSEAL_DEBUG")
    end)

    self:filtered({ Filter:config(PROJECT_CONFIG.Release) }, function()
        self:set("runtime", "Release")
        self:set("optimize", "on")
    end)
end

return SolutionBuilder
