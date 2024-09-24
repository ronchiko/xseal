local SolutionBuilder = require "xseal/solution/solution_builder"

---@diagnostic disable: undefined-global

require "os"

require "xseal/ems"

SolutionBuilder:create_workspace("XSeal", "C++20")

SolutionBuilder:set("staticruntime", "on")
SolutionBuilder:set("includedirs", {
    "src",
    "thirdparty/fmt/include",
    "thirdparty/glfw/include",
    "thirdparty/glm",
    "thirdparty/json/include",
    "thirdparty/entt/single_include",
    "thirdparty/stb"
})

SolutionBuilder:set("flags", {
    "MultiProcessorCompile",
    "NoIncrementalLink",
})

include "src/seal"
include "src/resources"

group "Frontends"

include "src/front/glfw"
include "src/front/emscripten"

group ""

group "Thirdparty"

include "thirdparty"

group ""

group "Backends"

include "src/back/gl"

group ""

group "Tests"
include "tests/seal_core_tests"
group ""
