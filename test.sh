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
	cmake --build build/release
	ctest --test-dir build/release/test --output-on-failure --rerun-failed
else
	cmake --build build/debug
	ctest --test-dir build/debug/test --output-on-failure --rerun-failed
fi

