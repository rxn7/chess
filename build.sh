#!/usr/bin/env bash

RELEASE=0
CLEAN=0

while getopts 'rc' OPTION; do
	case "$OPTION" in
		r)
			RELEASE=1
			;;
		c)
			CLEAN=1
			;;
	esac
done

if [ "$CLEAN" = 1 ] ; then
	printf "\033[32;1;4mCleaning the build directory\n\n\033[0m"
	rm -rf build
	./configure.sh
fi

if [ "$RELEASE" = 1 ] ; then
	printf "\033[32;1;4mBuilding the release version\n\n\033[0m"
	cmake --build build/release
else
	printf "\033[32;1;4mBuilding the debug version\n\n\033[0m"
	cmake --build build/debug
fi
