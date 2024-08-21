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
	printf "\033[32;1;4mRunning the release version\n\n\033[0m"
	pushd build/release/bin >/dev/null
else
	printf "\033[32;1;4mRunning the debug version\n\n\033[0m"
	pushd build/debug/bin >/dev/null
fi

./chess
popd >/dev/null
