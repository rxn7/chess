#!/usr/bin/env bash

BUILD_TYPE="debug"
PLATFORM="linux"

while getopts 'rw' OPTION; do
	case "$OPTION" in
		r)
			BUILD_TYPE="release"
			;;
		w)
			PLATFORM="windows"
			;;
	esac
done

printf "\033[32;1;4mRunning the $BUILD_TYPE version for $PLATFORM\n\n\033[0m"
pushd build/$PLATFORM/$BUILD_TYPE/bin >/dev/null

if [ "$PLATFORM" = "windows" ] ; then
	wine chess.exe
else
	./chess
fi

popd >/dev/null
