premake5 gmake

$configFiles = Get-ChildItem -Recurse . -Include ("Makefile","*.make")
foreach ($file in $configFiles) {
    $content = Get-Content $file.PSPath
    $content =  $content -replace '\$\(CC\)', "emcc"
    $content =  $content -replace '\$\(CXX\)', "em++"
    $content =  $content -replace '\$\(AR\)', "emar"
    
    Set-Content $file.PSPath $content
}

make config=debug_emscripten