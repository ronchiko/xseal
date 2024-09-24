local XSealVersions = {
    versions = nil
}

--[[
    Gets the version file path according to the script runner.
]]
local function _get_version_file_path()
    if "windows" == os.host() then
        local appdata = os.getenv("AppData")

        return path.join(appdata, "XSeal", "XSealVersions")
    end

    if "linux" == os.host() then
        return path.join("~", ".xseal", "XSealVersions")
    end

    error("XSeal is not supported on this operating system: " .. os.host())
end

function XSealVersions:_get_version_tree(version)
    if nil == self.versions then
        local version_file_path = _get_version_file_path()
        self.versions = assert(json.decode(io.readfile(version_file_path)), "Failed to load engines version file")
    end

    return assert(self.versions[version], "Engine version " .. version .. " is not installed!")
end

--[[
    Gets the global path to a version of the engine
]]
function XSealVersions:get_engine_path(engine_version)
    local version_tree = self:_get_version_tree(engine_version)
    return assert(version_tree.path, "Engine installation of " .. engine_version .. " is corrupt")
end

--[[
    Gets the global path to a version of the engine
]]
function XSealVersions:is_development_version(engine_version)
    local version_tree = self:_get_version_tree(engine_version)
    -- If the dev entry in the tree
    return 0 ~= assert(version_tree.dev, "Engine installation of " .. engine_version .. " is corrupt")
end

return XSealVersions
