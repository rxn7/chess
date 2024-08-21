#!/usr/bin/env bash

RELEASE=0

while getopts 'r' OPTION; do
	case "$OPTION" in
		r)
			RELEASE=1
			;;
	esac
done

if [ "$RELEASE" = 1 ] ; then
	printf "\033[32;1;4mBuilding the release version\n\n\033[0m"
	cmake --build build/release
else
	printf "\033[32;1;4mBuilding the debug version\n\n\033[0m"
	cmake --build build/debug
fi
