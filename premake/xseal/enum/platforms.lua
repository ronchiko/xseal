local WindowsPlatform = require "xseal/platforms/windows"
local WebGLPlatform = require "xseal/platforms/webgl"

local XSEAL_PLATFORM = {
    Windows = WindowsPlatform,
    WebGL = WebGLPlatform
}

return XSEAL_PLATFORM
