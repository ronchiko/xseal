require "xseal/ems"

local CPP_VERSION = require "xseal/enum/cpp"
local PROJECT_TYPE = require "xseal/solution/project_type"
local XSEAL_PLATFORM = require "xseal/enum/platforms"
local XSealVersions = require "xseal/versions"
local SolutionBuilder = require "xseal/solution/solution_builder"


local XSealProject = {
    name = ""
}

local function _complete_project_configuration(project_arguments)
    local configuration = {
        name = project_arguments.name,
        engine_version = project_arguments.engine_version,

        platforms = project_arguments.platforms or {
            XSEAL_PLATFORM.WebGL,
            XSEAL_PLATFORM.Windows,
        },

        cpp_version = project_arguments.cpp_version or CPP_VERSION.Cpp20,
        source_root = project_arguments.source_root or "src",
        headers_root = project_arguments.headers_root or "src",
    }

    assert(configuration.name, "Must provide an project name!")
    assert(configuration.engine_version, "Must provide an engine version!")

    return configuration
end

--[[
    Initializes the platform configurations for each of the platforms.

    @field project_name string
    @field platforms array[string]
]]
function XSealProject:_initialize_platforms(project_name, platforms)
    -- Requires at least one platform to create a project.
    if #platforms <= 0 then
        error("At least one platform is required per project")
    end

    local platform_names = {}
    for _, platform in ipairs(platforms) do
        table.insert(platform_names, platform.name)
    end
    SolutionBuilder:set("platforms", platform_names)

    for _, platform in ipairs(platforms) do
        local boot_project_name = platform:boot_project_name(project_name)
        platform:attach_build_configuration()

        SolutionBuilder:set_startup_project(boot_project_name)
        SolutionBuilder:start_filter({})
    end
end

function XSealProject:_link_with_engine(configuration)
    local engine_path = XSealVersions:get_engine_path(configuration.engine_version)
    print('Linking with engine at: ' .. engine_path)

    SolutionBuilder:set("libdirs", path.join(engine_path, "bin"))

    local include_directories = { path.join(engine_path, "include") }
    if XSealVersions:is_development_version(configuration.engine_version) then
        table.insert(include_directories, engine_path .. "/thirdparty/glfw/include")
        table.insert(include_directories, engine_path .. "/thirdparty/fmt/include")
        table.insert(include_directories, engine_path .. "/thirdparty/glm")
        table.insert(include_directories, engine_path .. "/thirdparty/json/include")
        table.insert(include_directories, engine_path .. "/thirdparty/entt/single_include")
        table.insert(include_directories, engine_path .. "/thirdparty/stb")
    end

    SolutionBuilder:set("includedirs", include_directories)
end

--[[
    Creates the core game project from the configuration.
]]
function XSealProject:_create_core_project(configuration)
    SolutionBuilder:create_project(configuration.name, PROJECT_TYPE.Library, nil)
    SolutionBuilder:set("files", {
        path.join(configuration.source_root, "**.c"),
        path.join(configuration.source_root, "**.cpp"),
        path.join(configuration.headers_root, "**.h"),
        path.join(configuration.headers_root, "**.hpp"),
    })

    SolutionBuilder:set("includedirs", configuration.headers_root)
    SolutionBuilder:set("links", { "XSeal" })
    -- TODO: Third party libraries
end

function XSealProject:_create_export_projects(configuration)
    SolutionBuilder:group("Exports", function()
        for _, platform in ipairs(configuration.platforms) do
            platform:create_boot_project(configuration.name)
        end
    end)
end

--[[
    Creates a new XSeal project.

    @field project_arguments XSealProjectParameters
]]
function XSealProject:create(project_arguments)
    local configuration = _complete_project_configuration(project_arguments)

    SolutionBuilder:create_workspace(configuration.name, configuration.cpp_version)
    self:_initialize_platforms(configuration.name, configuration.platforms)
    self:_link_with_engine(configuration)

    self:_create_core_project(configuration)
    self:_create_export_projects(configuration)
end

return XSealProject
