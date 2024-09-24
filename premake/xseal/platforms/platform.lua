local IPlatform = { name = nil }

function IPlatform:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    return o
end

function IPlatform:boot_project_name(solution)
    error("Not implemented")
end

-- Attach a boot project to the solution
function IPlatform:create_boot_project(base_project_name, boot_project_name)
    error("Not implemented")
end

function IPlatform:attach_build_configuration()
    error("Not implemented")
end

return IPlatform
