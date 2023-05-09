# XSEAL Engine

## Requirements

* Python 3
* Premake
* Emscripten (To build for WebGL)

## How to build locally

Load submodules:

```powershell
git submodule update --init --recursive
```

## Build for windows

```powershell
premake5 vs2022
# Then build via visual studio
```

## Build for web

```powershell
# Enter emsdk development environment
{PATH_TO_EMSDK}\emsdk_env.ps1

# Build (For windows)
./ems.ps1

# Launch on local server
./ems-srv.ps1
```
