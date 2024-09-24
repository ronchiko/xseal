local PROJECT_KIND = require "xseal/solution/project_type"
local SolutionBuilder = require "xseal/solution/solution_builder"
local Filter = require "xseal/solution/filters"
local SealBuildLocation = require "xseal/enum/build"

local XSealLibrary = {
    name = "",
    platform = nil,

    _project = {},
    _filters = {}
}

--[[
    Adds a property to this library project.
     If the property does not exist, adds it, if the property exists, extends it.

    - name: The name of the property to add.
    - value: The value to add the property
]]
function XSealLibrary:_add_property(name, value)
    if nil == self._project[name] then
        self._project[name] = {}
    end

    if type(value) ~= "table" then
        value = { value }
    end

    for _, entry in ipairs(value) do
        table.insert(self._project[name], entry)
    end
end

function XSealLibrary:_initialize_instance(parameters)
    setmetatable(parameters, { __index = { source_root = nil, include_root = nil, initialize_sources = true } })

    local make_source_pattern = function(pattern)
        if nil ~= parameters.source_root then
            return path.join(parameters.source_root, pattern)
        end

        return pattern
    end

    if not parameters.initialize_sources then
        return
    end

    self:_add_property("includedirs", parameters.include_root or ".")
    -- C++ source files
    self:_add_property("files", make_source_pattern("**.cpp"))
    self:_add_property("files", make_source_pattern("**.c"))
    self:_add_property("files", make_source_pattern("**.cc"))
    self:_add_property("files", make_source_pattern("**.cxx"))

    -- Header files
    self:_add_property("files", make_source_pattern("**.h"))
    self:_add_property("files", make_source_pattern("**.hpp"))
    self:_add_property("files", make_source_pattern("**.inl"))
end

function XSealLibrary:_merge_on_windows(link_names)
    local output_library_path = path.join(SealBuildLocation.Exported, self.name .. ".lib")
    local final_command = "LIB.exe /OUT:" .. output_library_path
    for _, value in ipairs(link_names) do
        final_command = final_command .. " " .. path.join(SealBuildLocation.Exported, value .. ".lib")
    end

    -- self:_add_property("postbuildmessage", "Merging libraries...")
    self:_add_property("postbuildcommands", final_command)
end

--[[
    Creates a new library object.

    Expects a table with the following keys:
    - name: The name of the library
    - platform[optional]: The platform this library supports (If nil, then not platform specific)
]]
function XSealLibrary:new(parameters)
    setmetatable(parameters, { __index = { platform = nil } })

    local o = { name = parameters.name, platform = parameters.platform, _filters = {}, _project = {} }
    setmetatable(o, self)
    self.__index = self

    o._filters = {}
    o:_initialize_instance(parameters)
    return o
end

--[[
    Defines an embedded resources folder for this libraray.

    Expects a table with the following parameters:
    - resources_directory: string to describe the path to where the resources are located.
    - export_name: The name of the generated register function.
    - generated_file_name: The name of the generated resources file (Defaults to .embed.generated.cpp)
]]
function XSealLibrary:embed_resource(parameters)
    setmetatable(parameters, { __index = { export_name = "", generated_file_name = ".embed.generated.cpp" } })

    assert(parameters.resources_directory, "Must pass a resource directory")
    local generated_file_name, export_name = parameters.generated_file_name, parameters.export_name

    self:_add_property("prebuildcommands", {
        "python %{wks.location}/python/embed_files/embed.py -d " .. parameters.resources_directory
        .. " --recurse -o " .. generated_file_name .. " --export " .. export_name
    })
    self:_add_property("files", { generated_file_name })
end

--[[
    Adds a source file pattern to this library

    - pattern: The pattern of the file to add.
]]
function XSealLibrary:add_source_file_pattern(pattern)
    self:_add_property("files", { pattern })
end

--[[
    Adds a source file pattern to this library

    - directory: The include directory to add.
]]
function XSealLibrary:add_include_directory(directory)
    self:_add_property("includedirs", { directory })
end

--[[
    Setups the linking for this library.

    Accepts a table with the following keys:
    - links: An array containing all the names of the libraries to link with.
    - options[optional]: Additional options to pass to the compiler.
    - merge[optional]: Either true or false, if true, unifies all the libraries into a large one after the build.
]]
function XSealLibrary:setup_linking(parameters)
    setmetatable(parameters, { __index = { merge = false, options = {} } })

    -- Setup the premake link parameter
    local link_names = assert(parameters.links, "Required to pass 'links' argument!")
    for _, value in ipairs(link_names) do
        self:_add_property("links", value)
    end

    for _, value in ipairs(parameters.options) do
        self:_add_property("linkoptions", value)
    end

    -- If we dont need to merge, we can just quit.
    if not parameters.merge then
        return
    end

    self:_merge_on_windows(link_names)
end

function XSealLibrary:filtered(filter, action)
    self._filters[filter] = action
end

function tprint(tbl, indent)
    if not indent then indent = 0 end
    local toprint = string.rep(" ", indent) .. "{\r\n"
    indent = indent + 2
    for k, v in pairs(tbl) do
        toprint = toprint .. string.rep(" ", indent)
        if (type(k) == "number") then
            toprint = toprint .. "[" .. k .. "] = "
        elseif (type(k) == "string") then
            toprint = toprint .. k .. "= "
        end
        if (type(v) == "number") then
            toprint = toprint .. v .. ",\r\n"
        elseif (type(v) == "string") then
            toprint = toprint .. "\"" .. v .. "\",\r\n"
        elseif (type(v) == "table") then
            toprint = toprint .. tprint(v, indent + 2) .. ",\r\n"
        else
            toprint = toprint .. "\"" .. tostring(v) .. "\",\r\n"
        end
    end
    toprint = toprint .. string.rep(" ", indent - 2) .. "}"
    return toprint
end

function zprint(value)
    if (type(value) == "number") then
        return value
    elseif (type(value) == "string") then
        return value
    elseif (type(value) == "table") then
        return tprint(value, 0)
    else
        return tostring(value)
    end
end

--[[
    Actually generates the premake project from the library.
]]
function XSealLibrary:generate()
    SolutionBuilder:create_project(self.name, PROJECT_KIND.Library, self.platform)

    print("Generating project " .. self.name)

    for key, value in pairs(self._project) do
        print('============')
        print(key)
        print(zprint(value))
        SolutionBuilder:set(key, value)
    end

    for filter, action in pairs(self._filters) do
        SolutionBuilder:filtered(filter, action)
    end
end

return XSealLibrary
