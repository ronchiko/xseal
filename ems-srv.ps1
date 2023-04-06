pushd src/front/emscripten/bin/Emscripten/Debug

try {
copy ../../../index.html index.html
python -m http.server 8001
} finally {
    popd
}