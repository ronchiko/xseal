local SimpleLibrary = { name = "", path = "" }

function SimpleLibrary:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function SimpleLibrary:path()
    return self.path
end

return SimpleLibrary
