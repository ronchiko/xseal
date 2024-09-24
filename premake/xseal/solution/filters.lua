local Filter = {}

function Filter:config(config_name)
    return "configurations:" .. config_name
end

function Filter:platform(platform_name)
    return "platforms:" .. platform_name
end

return Filter
