local SealBuildLocation = require "xseal/enum/build"


local SealLibrary = { name = "" }

function SealLibrary:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function SealLibrary:path()
    return path.join(SealBuildLocation.Exported, self.name)
end

return SealLibrary