#!/bin/sh

# Windows Static Build

MXE_DIR="$HOME/mxe-static/usr/x86_64-w64-mingw32.static"
SRC_DIR="../qtgiphy"

PKG_CONFIG_PATH=${MXE_DIR}/lib/pkgconfig cmake ${SRC_DIR} \
                                               -DCMAKE_BUILD_TYPE=Release \
                                               -DCMAKE_TOOLCHAIN_FILE=${SRC_DIR}/cmake/Toolchain-x86_64-w64-mingw32-static.cmake \
                                               -DCMAKE_PREFIX_PATH=${MXE_DIR}/qt6/lib/cmake \
                                               -DENABLE_WIN32_CONSOLE=OFF \
                                               -DBUILD_WITH_QT6=ON \
                                               || exit 1

make -j$(nproc) || exit 1
