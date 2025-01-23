#!/usr/bin/env bash

./configure.sh

cmake --build build/linux/release
cmake --build build/windows/release

rm -rf prod
mkdir prod
cd build/linux/release/bin/ && zip -r ../../../../prod/chess-linux.zip ./* && cd -
cd build/windows/release/bin/ && zip -r ../../../../prod/chess-windows.zip ./* && cd -
