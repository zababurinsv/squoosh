#!/bin/bash
set -e

apt-get update
apt-get install -qqy yasm

(
  mkdir -p aom-build
  cd aom-build
  cmake ../node_modules/aom \
    -DENABLE_CCACHE=1 \
    -DAOM_TARGET_CPU=generic \
    -DENABLE_DOCS=0 \
    -DENABLE_TESTS=0 \
    -DCONFIG_ACCOUNTING=1 \
    -DCONFIG_INSPECTION=1 \
    -DCONFIG_MULTITHREAD=0 \
    -DCONFIG_RUNTIME_CPU_DETECT=0 \
    -DCONFIG_WEBM_IO=0 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/emsdk_portable/sdk/cmake/Modules/Platform/Emscripten.cmake

  emmake make
)

emcc \
  --bind -O3 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MODULARIZE=1 \
  -s 'EXPORT_NAME="avif_enc"' \
  -I ./node_modules/aom \
  -I ./aom-build \
  -o ./avif_enc.js \
  -x c++ -std=c++11 \
  avif_enc.cpp \
  aom-build/libaom.a
