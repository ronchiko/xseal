local SealBuildLocation = {
    Exported = "%{wks.location}/.build/%{cfg.buildcfg}-%{cfg.system}",
    Object = "%{wks.location}/.obj/%{cfg.buildcfg}-%{cfg.system}"
}

return SealBuildLocation
