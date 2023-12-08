#!/bin/bash

rm -rf build && mkdir build && cd build \
    && conan install .. \
        --build=missing \
        -s compiler=gcc \
        -s compiler.version=12 \
        -s compiler.libcxx=libstdc++11 \
        --generator=CMakeToolchain \
        -s build_type=Release \
    && cmake .. \
        -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
        -DCMAKE_INSTALL_PREFIX=../install \
        -DCMAKE_C_COMPILER=gcc-12 \
        -DCMAKE_CXX_COMPILER=g++-12 \
        -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . \
    && ctest \
    && cmake --build . --target=install
