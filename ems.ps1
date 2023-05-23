# This file is a hack to enable building emscripten with premake5 generation on windows.
# This works by generating a unix make files as the build backend and manually replacing the
# executable for the C compiler, CXX compiler and linker to the emsripten ones.
# NOTE: You must be inside the emscripten environment to run this (emsdk_env.ps1)

# On unix systems, i believe just generating make files and building with them should do the trick.

premake5 gmake

$configFiles = Get-ChildItem -Recurse . -Include ("Makefile", "*.make")
foreach ($file in $configFiles) {
    $content = Get-Content $file.PSPath
    $content = $content -replace '\$\(CC\)', "emcc"
    $content = $content -replace '\$\(CXX\)', "em++"
    $content = $content -replace '\$\(AR\)', "emar"
    
    Set-Content $file.PSPath $content
}

make config=debug_emscripten