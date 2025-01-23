#!/usr/bin/env bash

rm -rf build

COMMON="-GNinja -DBUILD_SHARED_LIBS=OFF"

cmake -Bbuild/linux/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $COMMON
cmake -Bbuild/linux/release -DCMAKE_BUILD_TYPE=Release $COMMON

# Mingw builds
if command -v x86_64-w64-mingw32-cmake &> /dev/null ; then
	echo "x86_64-w64-mingw32-cmake found. Windows build will be created"

	x86_64-w64-mingw32-cmake -Bbuild/windows/debug -DCMAKE_BUILD_TYPE=Debug $COMMON
	x86_64-w64-mingw32-cmake -Bbuild/windows/release -DCMAKE_BUILD_TYPE=Release $COMMON
else
	echo "x86_64-w64-mingw32-cmake not found. Windows builds will not be created!"
fi
