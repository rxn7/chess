#!/usr/bin/env bash

BUILD_TYPE="debug"
PLATFORM="linux"
CLEAN=0

while getopts 'rcw' OPTION; do
	case "$OPTION" in
		r)
			BUILD_TYPE="release"
			;;
		c)
			CLEAN=1
			;;
		w)
			PLATFORM="windows"
			;;
	esac
done

if [ "$CLEAN" = 1 ] ; then
	printf "\033[32;1;4mCleaning the build directory\n\n\033[0m"
	rm -rf build
	./configure.sh
fi

if PLATFORM="linux" ; then
	cp build/linux/$BUILD_TYPE/compile_commands.json .
fi

printf "\033[32;1;4mBuilding the $BUILD_TYPE version for $PLATFORM\n\n\033[0m"
cmake --build build/$PLATFORM/$BUILD_TYPE
